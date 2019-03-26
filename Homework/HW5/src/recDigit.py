"""
    Author: Zifan Wang
    Train LeNet-5 using MNIST Dataset
"""

import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
import keras 
from keras.datasets import mnist
from keras.models import Sequential, Model
from keras.layers import Input
from keras.layers.normalization import BatchNormalization
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.core import Activation, Flatten, Dropout, Dense
from keras import regularizers

def load_data_mnist():
    # load train data and test data from mnist 
    (x_train,y_train),(x_test,y_test) = mnist.load_data()
    numData,height,width = x_train.shape
    x_train_reversed = np.subtract(np.ones((numData,height,width))*255,x_train)    # reverse color
    X_train = np.append(x_train,x_train_reversed,axis=0)
    X_train = X_train.reshape(120000, 28, 28, 1)         # ori: (60000, 28, 28) -> now: (60000, 28, 28, 1) 
    y_train = keras.utils.to_categorical(y_train,10)    # Ten class: 0-9. change from 0-9 to onehot label. Size(60000,10)
    Y_train = np.append(y_train,y_train,axis=0)
    numData,height,width = x_test.shape
    x_test_reversed = np.subtract(np.ones((numData,height,width))*255,x_test)
    X_test = np.append(x_test,x_test_reversed,axis=0)
    X_test = X_test.reshape(20000, 28, 28, 1)
    y_test = keras.utils.to_categorical(y_test,10)
    Y_test = np.append(y_test,y_test,axis=0)

    return X_train, Y_train, X_test, Y_test

def Model(cnnModel,input_size):
    """
        Function to build my network
        Arguments: keras cnnModel & input_size: image size
        Return: model after set up
    """
    # Convolutional Layer
    cnnModel.add(Conv2D(filters = 32, kernel_size=(3,3), strides=(1,1), padding='same', activation='relu', input_shape = input_size))
    cnnModel.add(MaxPooling2D(pool_size=(2,2)))
    cnnModel.add(Conv2D(filters = 64, kernel_size=(3,3), strides=(1,1), padding='same', activation='relu'))
    cnnModel.add(Conv2D(filters = 64, kernel_size=(3,3), strides=(1,1), padding='same', activation='relu'))
    cnnModel.add(MaxPooling2D(pool_size=(2,2)))
    # Fully Connected Layer
    cnnModel.add(Flatten())
    cnnModel.add(Dense(128))
    cnnModel.add(Activation('relu'))
    cnnModel.add(Dense(64))
    cnnModel.add(Activation('relu'))
    cnnModel.add(Dense(32))
    cnnModel.add(Activation('relu'))
    cnnModel.add(Dense(10))
    cnnModel.add(Activation('softmax'))

    cnnModel.compile(optimizer='RMSprop',loss='categorical_crossentropy',metrics=['accuracy'])

    return cnnModel


if __name__ == "__main__":
    # Load Data
    x_train, y_train, x_test, y_test = load_data_mnist()
    # Build model
    model = Sequential()
    model = Model(model,(28,28,1))
    # Display model setting
    print(model.summary())

    hist = model.fit(x=x_train,
          y=y_train, 
          batch_size=32, 
          epochs=20, 
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
    model.save('model.h5')
    print('Save Model to Disk')

    # Test Set Accuracy
    score = model.evaluate(x_test,y_test,verbose=1)
    print('Test Loss: ', score[0])
    print('Test Accuracy: ', score[1])