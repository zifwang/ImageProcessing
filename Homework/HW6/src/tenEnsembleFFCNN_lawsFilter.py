import numpy as np
from numpy import linalg as LA
import keras
from keras import backend as K
from keras.datasets import mnist
import sklearn
import os
import random
import data
import feedforward_model 

if __name__ == '__main__':
    # get data
    x_train, y_train, x_test, y_test = data.get_mnist_data()
    x_train, y_train = data.data_separation(x_train,y_train)
    y_train_oneHot = keras.utils.to_categorical(y_train, 10)
    y_test = keras.utils.to_categorical(y_test, 10)

    x_train_reduced = x_train[0:10000]
    y_train_reduced = y_train[0:10000]
    y_train_reduced_one_hot = keras.utils.to_categorical(y_train_reduced, 10)

    x_train_laws = data.applyLawsFilter(x_train)
    x_train_reduced_laws = data.applyLawsFilter(x_train_reduced)
    # print(x_train_reduced_laws.shape)
    x_test_laws = data.applyLawsFilter(x_test)

    prediction_train = []
    prediction_test = []

    class_weight = np.array([0.9704, 0.9499, 0.9284, 0.9015, 0.8651,
                            0.9615, 0.9549, 0.9380, 0.9190, 0.8774])

    for i in range(0,10):
        print('---------Test %2d------------' % (i+1))
        ffCNN = feedforward_model.feedforwardCNN()
        ffCNN.loadInitParameters(useProvidedParam = False, useMNIST = False, images = x_train_reduced_laws[i], labels = y_train_reduced, kernelSize = '5,5', stride = [1,1],
                            numKernels = '5,15', maxPooling = True, energyPercent = None, numImagesUsed = -1, classUsed = [0,1,2,3,4,5,6,7,8,9], verbose = True)
        leNet5 = ffCNN.feedforward_leNet5()

        score = leNet5.evaluate(x_train_reduced_laws[i],y_train_reduced_one_hot,verbose = 1)
        print('Train loss: %5f'%score[0])
        print('Train accuracy: %5f'%score[1])

        score = leNet5.evaluate(x_test_laws[i],y_test,verbose = 1)
        print('Test loss: %5f'%score[0])
        print('Test accuracy: %5f'%score[1])

        prediction_train.append(class_weight[i]*leNet5.predict(x_train_reduced_laws[i]))
        prediction_test.append(class_weight[i]*leNet5.predict(x_test_laws[i]))

    prediction_test = np.array(prediction_test)
    prediction_train = np.array(prediction_train)

    np.save('prediction_test',prediction_test)
    np.save('prediction_train',prediction_train)

    prediction_test = np.moveaxis(prediction_test,0,2)
    prediction_train = np.moveaxis(prediction_train,0,2)
    prediction_test = prediction_test.reshape(-1,100)
    prediction_train = prediction_train.reshape(-1,100)

    # Train acc
    weight = np.matmul(LA.pinv(prediction_train), y_train_reduced_one_hot).astype(np.float32)
    prediction_train = np.matmul(prediction_train, weight)
    pred_labels = np.argmax(prediction_train, axis=1)
    train_acc = sklearn.metrics.accuracy_score(y_train_reduced, pred_labels)
    print('Training accuracy is {}'.format(train_acc))

    # Ensemble acc
    prediction = np.dot(prediction_test, weight)

    ypred = np.eye(10)[np.argmax(prediction, axis=1)]
    ensemble_acc = np.sum(ypred*y_test)/ypred.shape[0]
    print('ensemble: %5f' %ensemble_acc)

    data.compare_bp_ffCNN(ypred)
"""
1. raw
10000/10000 [==============================] - 1s 148us/step
Train loss: 1.575273
Train accuracy: 0.975800
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.590470
Test accuracy: 0.963300

2. L3L3
10000/10000 [==============================] - 0s 41us/step
Train loss: 1.575328
Train accuracy: 0.974500
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.586673
Test accuracy: 0.962900

3. L3E3
10000/10000 [==============================] - 0s 44us/step
Train loss: 1.607820
Train accuracy: 0.960200
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.623321
Test accuracy: 0.946300

4. L3S3
10000/10000 [==============================] - 0s 45us/step
Train loss: 1.659263
Train accuracy: 0.935200
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.688257
Test accuracy: 0.905000

5. E3L3
10000/10000 [==============================] - 0s 43us/step
Train loss: 1.592883
Train accuracy: 0.965500
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.611247
Test accuracy: 0.953200

6. E3E3
10000/10000 [==============================] - 0s 46us/step
Train loss: 1.621477
Train accuracy: 0.960200
10000/10000 [==============================] - 0s 40us/step
Test loss: 1.645500
Test accuracy: 0.944600

7. E3S3
10000/10000 [==============================] - 0s 47us/step
Train loss: 1.679686
Train accuracy: 0.929100
10000/10000 [==============================] - 0s 40us/step
Test loss: 1.714120
Test accuracy: 0.901700

8. S3L3
10000/10000 [==============================] - 0s 47us/step
Train loss: 1.646985
Train accuracy: 0.942000
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.678941
Test accuracy: 0.923300

9. S3E3
10000/10000 [==============================] - 0s 49us/step
Train loss: 1.663021
Train accuracy: 0.937800
10000/10000 [==============================] - 0s 41us/step
Test loss: 1.702369
Test accuracy: 0.908100

10. S3S3
10000/10000 [==============================] - 0s 49us/step
Train loss: 1.729345
Train accuracy: 0.891500
10000/10000 [==============================] - 0s 42us/step
Test loss: 1.767215
Test accuracy: 0.858100

training acc is 0.9836

ensemble: 0.971400
"""

"""
1.
10000/10000 [==============================] - 1s 149us/step
Train loss: 1.575913
Train accuracy: 0.973500
10000/10000 [==============================] - 0s 37us/step
Test loss: 1.591652
Test accuracy: 0.964500
2.
training acc is 0.9748
10000/10000 [==============================] - 0s 40us/step
Train loss: 1.575113
Train accuracy: 0.974800
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.586902
Test accuracy: 0.963400
3.
10000/10000 [==============================] - 0s 43us/step
Train loss: 1.608882
Train accuracy: 0.958700
10000/10000 [==============================] - 0s 40us/step
Test loss: 1.623321
Test accuracy: 0.946800
4.
10000/10000 [==============================] - 0s 45us/step
Train loss: 1.660169
Train accuracy: 0.936700
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.687276
Test accuracy: 0.904100
5.
10000/10000 [==============================] - 0s 45us/step
Train loss: 1.592942
Train accuracy: 0.966700
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.608757
Test accuracy: 0.954000
6.
10000/10000 [==============================] - 0s 47us/step
Train loss: 1.620585
Train accuracy: 0.962700
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.645313
Test accuracy: 0.941700
7.
10000/10000 [==============================] - 0s 46us/step
Train loss: 1.680515
Train accuracy: 0.928000
10000/10000 [==============================] - 0s 38us/step
Test loss: 1.716451
Test accuracy: 0.897300
8.
10000/10000 [==============================] - 0s 49us/step
Train loss: 1.647325
Train accuracy: 0.941500
10000/10000 [==============================] - 0s 40us/step
Test loss: 1.678121
Test accuracy: 0.920600
9.
10000/10000 [==============================] - 0s 49us/step
Train loss: 1.663892
Train accuracy: 0.936600
10000/10000 [==============================] - 0s 38us/step
Test loss: 1.704194
Test accuracy: 0.908100
10
10000/10000 [==============================] - 0s 50us/step
Train loss: 1.729068
Train accuracy: 0.889400
10000/10000 [==============================] - 0s 41us/step
Test loss: 1.766122
Test accuracy: 0.858300

Training accuracy is 0.9826
ensemble: 0.972700

Number of different prediction:: 288.0
Accuracy in BP of different prediction data: 0.6944
Accuracy in feedforward of different prediction data: 0.205


Number of different prediction:   295
Accuracy in BP of different prediction data: 0.701695
Accuracy in feedforward of different prediction data: 0.206780

Number of different prediction:   289
Accuracy in BP of different prediction data: 0.698962
Accuracy in feedforward of different prediction data: 0.221453
"""