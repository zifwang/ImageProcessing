"""
    Author: Zifan Wang
    Train ten different FF-CNNs
"""
import numpy as np
import keras
from keras import backend as K
from keras.datasets import mnist
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

    prediction = np.zeros((10000,10))

    # kernelSizeList = ['3,3','5,3','3,5','5,5']

    for i in range(0,10):
        print('---------Test %2d------------' % (i+1))
        ffCNN = feedforward_model.feedforwardCNN()
        ffCNN.loadInitParameters(useProvidedParam = False, useMNIST = False, images = x_train[5000*i:5000*(i+1)], labels = y_train[5000*i:5000*(i+1)], kernelSize = '5,5', stride = [1,1],
                            numKernels = '5,15', maxPooling = True, energyPercent = None, numImagesUsed = -1, classUsed = [0,1,2,3,4,5,6,7,8,9], verbose = True)
        leNet5 = ffCNN.feedforward_leNet5()

        score = leNet5.evaluate(x_train[5000*i:5000*(i+1)],y_train_oneHot[5000*i:5000*(i+1)],verbose = 1)
        print('Train loss: %5f'%score[0])
        print('Train accuracy: %5f'%score[1])

        score = leNet5.evaluate(x_test,y_test,verbose = 1)
        print('Test loss: %5f'%score[0])
        print('Test accuracy: %5f'%score[1])

        predict = leNet5.predict(x_test)
        prediction +=  np.array(predict)

    ypred = np.eye(10)[np.argmax(prediction, axis=1)]
    acc = np.sum(ypred*y_test)/ypred.shape[0]
    print('ensemble: %5f' %acc)

"""
1. 5000
Train loss: 1.562480
Train accuracy: 0.982600
10000/10000 [==============================] - 0s 38us/step
Test loss: 1.601746
Test accuracy: 0.958400

2. 10000
Train loss: 1.568452
Train accuracy: 0.984400
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.596869
Test accuracy: 0.960500

3. 15000
Train loss: 1.567628
Train accuracy: 0.982200
10000/10000 [==============================] - 0s 36us/step
Test loss: 1.595877
Test accuracy: 0.962000

4. 20000
Train loss: 1.566277
Train accuracy: 0.986200
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.590126
Test accuracy: 0.963200

5. 25000
Train loss: 1.563839
Train accuracy: 0.984000
10000/10000 [==============================] - 0s 39us/step
Test loss: 1.599580
Test accuracy: 0.964700

6. 30000
Train loss: 1.563584
Train accuracy: 0.985200
10000/10000 [==============================] - 0s 38us/step
Test loss: 1.592695
Test accuracy: 0.962200

7. 35000
Train loss: 1.568057
Train accuracy: 0.984200
10000/10000 [==============================] - 0s 34us/step
Test loss: 1.585557
Test accuracy: 0.967800

8. 40000
Train loss: 1.565550
Train accuracy: 0.984400
10000/10000 [==============================] - 0s 40us/step
Test loss: 1.593589
Test accuracy: 0.963000

9. 45000
Train loss: 1.567855
Train accuracy: 0.980600
10000/10000 [==============================] - 0s 41us/step
Test loss: 1.587995
Test accuracy: 0.961100

10. 50000
Train loss: 1.574363
Train accuracy: 0.978200
10000/10000 [==============================] - 0s 41us/step
Test loss: 1.588729
Test accuracy: 0.960100

ensemble: acc: ensemble: 0.972100
"""