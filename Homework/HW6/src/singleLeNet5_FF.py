"""
    Author: Zifan Wang
    Individual FF-CNN on the MNIST dataset
"""
from __future__ import print_function
import numpy as np
import keras
from keras import backend as K
from keras.datasets import mnist
from keras.utils import to_categorical
import feedforward_model 
import data

if __name__ == '__main__':
    ffCNN = feedforward_model.feedforwardCNN()
    ffCNN.loadInitParameters(useProvidedParam = False, useMNIST = True, images = None, labels = None, kernelSize = '5,5', stride = [1,1],
                            numKernels = '5,15', maxPooling = True, energyPercent = None, numImagesUsed = 10000, classUsed = [0,1,2,3,4,5,6,7,8,9], verbose = True)
    leNet5 = ffCNN.feedforward_leNet5()
    train_images, train_labels, test_images, test_labels, class_list = data.import_data('0-9')
    train_labels = to_categorical(train_labels, 10)
    test_labels = to_categorical(test_labels, 10)

    score = leNet5.evaluate(train_images,train_labels,verbose = 1)
    print('Train loss: %5f'%score[0])
    print('Train accuracy: %5f'%score[1])

    score = leNet5.evaluate(test_images,test_labels,verbose = 1)
    print('Test loss: %5f'%score[0])
    print('Test accuracy: %5f'%score[1])
    

"""
kernelSize = '5,5', numKernels = '6,16'
Train loss: 1.584306
Train accuracy: 0.968800
test loss: 1.578614
test accuracy: 0.970200

kernelSize = '5,5', numKernels = '6,16'
Train loss: 1.585382
Train accuracy: 0.968717
Test loss: 1.579180
Test accuracy: 0.969300

kernelSize = '5,5', numKernels = '6,16'
Train loss: 1.584608
Train accuracy: 0.968950
Test loss: 1.578849
Test accuracy: 0.970300

kernelSize = '5,5', numKernels = '6,16'
Train loss: 1.584542
Train accuracy: 0.968067
Test loss: 1.577719
Test accuracy: 0.969800

kernelSize = '5,5', numKernels = '6,16'
Train loss: 1.585144
Train accuracy: 0.968433
Test loss: 1.579197
Test accuracy: 0.968400
"""