"""
    Author: Zifan Wang
    Train LeNet-5 using MNIST Dataset
"""

import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
import keras 
from keras import optimizers
from keras.datasets import mnist
from keras.models import Sequential, Model
from keras.layers import Input
from keras.layers.normalization import BatchNormalization
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.core import Activation, Flatten, Dropout, Dense
from keras import regularizers

def leNet5_model(input_size,kernelInitializer,kernelRegularizer):
    """
        leNet5 model: using keras 
        Arguments: input_size: image size (height,width,channel)
                   kernelInitializer: 'random_uniform','he_normal','he_uniform','glorot_normal','glorot_uniform','Identity'
                   kernelRegularizer: True || False. True -> use l2.regularizer. False -> do not use l2. regularizer.
        Return: model(myleNet5)
    """
    X_in = Input(input_size)
    if kernelInitializer == '' and kernelRegularizer == False:
        X = Conv2D(filters=6,kernel_size=5,strides=(1,1),padding='same',activation='relu',name='conv1')(X_in)
        X = MaxPooling2D(pool_size=(2,2),name='maxpool1')(X)
        X = Conv2D(filters=16,kernel_size=5,strides=(1,1),padding='valid',activation='relu',name='conv2')(X)
        X = MaxPooling2D(pool_size=(2,2),name='maxpool2')(X)
        X = Flatten(name='flat1')(X)
        X = Dense(units=120,activation='relu',name='fullConnect1')(X)
        X = Dense(units=84,activation='relu',name='fullConnect2')(X)
        X = Dense(units=10,activation='softmax',name='output')(X)
        model = Model(inputs=X_in,outputs=X,name='myLeNet5')

    elif kernelInitializer != '' and kernelRegularizer == False:
        X = Conv2D(filters=6,kernel_size=5,strides=(1,1),padding='same',activation='relu',name='conv1')(X_in)
        X = MaxPooling2D(pool_size=(2,2),name='maxpool1')(X)
        X = Conv2D(filters=16,kernel_size=5,strides=(1,1),padding='valid',activation='relu',name='conv2')(X)
        X = MaxPooling2D(pool_size=(2,2),name='maxpool2')(X)
        X = Flatten(name='flat1')(X)
        X = Dense(units=120,activation='relu',kernel_initializer=kernelInitializer,name='fullConnect1')(X)
        X = Dense(units=84,activation='relu',kernel_initializer=kernelInitializer,name='fullConnect2')(X)
        X = Dense(units=10,activation='softmax',kernel_initializer=kernelInitializer,name='output')(X)
        model = Model(inputs=X_in,outputs=X,name='myLeNet5')

    elif kernelInitializer == '' and kernelRegularizer == True:
        X = Conv2D(filters=6,kernel_size=5,strides=(1,1),padding='same',activation='relu',kernel_regularizer=regularizers.l2(0.0001),name='conv1')(X_in)
        X = MaxPooling2D(pool_size=(2,2),name='maxpool1')(X)
        X = Conv2D(filters=16,kernel_size=5,strides=(1,1),padding='valid',activation='relu',kernel_regularizer=regularizers.l2(0.0001),name='conv2')(X)
        X = MaxPooling2D(pool_size=(2,2),name='maxpool2')(X)
        X = Flatten(name='flat1')(X)
        X = Dense(units=120,activation='relu',kernel_regularizer=regularizers.l2(0.0001),name='fullConnect1')(X)
        X = Dense(units=84,activation='relu',kernel_regularizer=regularizers.l2(0.0001),name='fullConnect2')(X)
        X = Dense(units=10,activation='softmax',kernel_regularizer=regularizers.l2(0.0001),name='output')(X)
        model = Model(inputs=X_in,outputs=X,name='myLeNet5')

    else:
        X = Conv2D(filters=6,kernel_size=5,strides=(1,1),padding='same',activation='relu',kernel_regularizer=regularizers.l2(0.0001),name='conv1')(X_in)
        X = MaxPooling2D(pool_size=(2,2),name='maxpool1')(X)
        X = Conv2D(filters=16,kernel_size=5,strides=(1,1),padding='valid',activation='relu',kernel_regularizer=regularizers.l2(0.0001),name='conv2')(X)
        X = MaxPooling2D(pool_size=(2,2),name='maxpool2')(X)
        X = Flatten(name='flat1')(X)
        X = Dense(units=120,activation='relu',kernel_initializer=kernelInitializer,kernel_regularizer=regularizers.l2(0.0001),name='fullConnect1')(X)
        X = Dense(units=84,activation='relu',kernel_initializer=kernelInitializer,kernel_regularizer=regularizers.l2(0.0001),name='fullConnect2')(X)
        X = Dense(units=10,activation='softmax',kernel_initializer=kernelInitializer,kernel_regularizer=regularizers.l2(0.0001),name='output')(X)
        model = Model(inputs=X_in,outputs=X,name='myLeNet5')
    
    return model




# load train data and test data from mnist 
(x_train,y_train),(x_test,y_test) = mnist.load_data()
x_train = x_train.reshape(60000, 28, 28, 1)         # ori: (60000, 28, 28) -> now: (60000, 28, 28, 1) 
y_train = keras.utils.to_categorical(y_train,10)    # Ten class: 0-9. change from 0-9 to onehot label. Size(60000,10)
x_test = x_test.reshape(10000, 28, 28, 1)
y_test = keras.utils.to_categorical(y_test,10)

myModel = leNet5_model((28,28,1),'random_uniform',False)        # Changeble
sgd = optimizers.SGD(lr=0.001, decay=1e-6, momentum=0.9, nesterov=True)
myModel.compile(optimizer=sgd,loss='categorical_crossentropy',metrics=['accuracy'])

# Display model Summary
print(myModel.summary())

hist = myModel.fit(x=x_train,
          y=y_train, 
          batch_size=32, 
          epochs=15, 
          verbose=1,
          validation_split=0.1,
          shuffle=True
          )

# Compute mean and variance of training & validation accuracy
print('Mean training accuracy: ',np.mean(hist.history['acc']))
print('Mean validation accuracy: ',np.mean(hist.history['val_acc']))
print('Variance training accuracy: ',np.var(hist.history['acc']))
print('Variance validation accuracy: ',np.var(hist.history['val_acc']))

# Plot training & validation accuracy values
plt.plot(hist.history['acc'])
plt.title('Model Train Accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy')
plt.show()

plt.plot(hist.history['val_acc'])
plt.title('Model Validation Accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy')
plt.show()


# Save Model
myModel.save('model.h5')
print('Save Model to Disk')

# Test Set Accuracy
score = myModel.evaluate(x_test,y_test,verbose=1)
print('Test Loss: ', score[0])
print('Test Accuracy: ', score[1])


"""
1. 0.001_false_true
54000/54000 [==============================] - 5s 97us/step - loss: 0.0109 - acc: 0.9990 - val_loss: 0.0533 - val_acc: 0.9900
Mean training accuracy:  0.9902716049382717
Mean validation accuracy:  0.9877777777777778
Variance training accuracy:  0.00016850693491845785
Variance validation accuracy:  1.1835802469135698e-05
Save Model to Disk
10000/10000 [==============================] - 0s 32us/step
Test Loss:  0.04078451611176133
Test Accuracy:  0.991

2. 0.001_true_true
54000/54000 [==============================] - 5s 100us/step - loss: 0.0146 - acc: 0.9974 - val_loss: 0.0564 - val_acc: 0.9910
Mean training accuracy:  0.9895814814814815
Mean validation accuracy:  0.9875444444444443
Variance training accuracy:  0.0001960108276177408
Variance validation accuracy:  6.51654320987657e-06
Save Model to Disk
10000/10000 [==============================] - 0s 33us/step
Test Loss:  0.054514804042875765
Test Accuracy:  0.9875

3. 0.01_true_true
54000/54000 [==============================] - 5s 99us/step - loss: 2.3317 - acc: 0.1120 - val_loss: 2.3333 - val_acc: 0.1050
Mean training accuracy:  0.1910098765432099
Mean validation accuracy:  0.16215555555555555
Variance training accuracy:  0.0463308769943606
Variance validation accuracy:  0.045015983209876556
Save Model to Disk
10000/10000 [==============================] - 0s 33us/step
Test Loss:  2.3311051441192627
Test Accuracy:  0.1135

4. 0.0001_true_true
54000/54000 [==============================] - 5s 100us/step - loss: 0.0364 - acc: 0.9907 - val_loss: 0.0619 - val_acc: 0.9837
Mean training accuracy:  0.9738419753086418
Mean validation accuracy:  0.980811111111111
Variance training accuracy:  0.0007700460387136106
Variance validation accuracy:  5.729950617283946e-05
Save Model to Disk
10000/10000 [==============================] - 0s 35us/step
Test Loss:  0.0545536430940032
Test Accuracy:  0.9843

5. 0.001_true_false
54000/54000 [==============================] - 5s 95us/step - loss: 0.0081 - acc: 0.9974 - val_loss: 0.0617 - val_acc: 0.9877
Mean training accuracy:  0.9895382716049383
Mean validation accuracy:  0.9859444444444444
Variance training accuracy:  0.00015771209266880027
Variance validation accuracy:  1.251728395061725e-05
Save Model to Disk
10000/10000 [==============================] - 0s 31us/step
Test Loss:  0.039339677639823274
Test Accuracy:  0.9903
"""






"""
SGD without kernelRegularizer:
    Mean training accuracy:  0.9917611111111111
    Mean validation accuracy:  0.9883916666666666
    Variance training accuracy:  0.0001958636796982166
    Variance validation accuracy:  6.9590972222222465e-06
    Final training accuracy:  0.9987
    Final validation accuracy: 0.9890
    Test accuracy: 0.9895
RMS without kernelRegularizer: 
    Mean training accuracy:  0.9778314814814815
    Mean validation accuracy:  0.981725
    Variance training accuracy:  5.756330246913581e-05
    Variance validation accuracy:  1.2611874999999946e-05
    Final training accuracy:  0.9781
    Final validation accuracy: 0.9852
    Test accuracy: 0.9824
Adam without kernelRegularizer:
    Mean training accuracy:  0.9899064814814815
    Mean validation accuracy:  0.9871583333333334
    Variance training accuracy:  8.971843192729753e-05
    Variance validation accuracy:  4.070763888888899e-06
    Final training accuracy:  0.9950
    Final validation accuracy: 0.9850
    Test accuracy: 0.9864

SGD with kernelRegularizer:
    Mean training accuracy:  0.9912564814814815
    Mean validation accuracy:  0.986525
    Variance training accuracy:  0.00019374548268175583
    Variance validation accuracy:  1.1192430555555505e-05
    Final training accuracy:  0.9982
    Final validation accuracy: 0.9913
    Test accuracy: 0.989
RMS with kernelRegularizer: 
    Mean training accuracy:  0.9844342592592591
    Mean validation accuracy:  0.9838583333333334
    Variance training accuracy:  6.466712877229072e-05
    Variance validation accuracy:  1.0709097222222301e-05
    Final training accuracy:  0.9886
    Final validation accuracy: 0.9878
    Test accuracy: 0.9862
Adam with kernelRegularizer:
    Mean training accuracy:  0.9888101851851852
    Mean validation accuracy:  0.986825
    Variance training accuracy:  0.00010129858967764084
    Variance validation accuracy:  4.501319444444466e-06
    Final training accuracy:  0.9945
    Final validation accuracy: 0.9862
    Test accuracy: 0.9873
"""
