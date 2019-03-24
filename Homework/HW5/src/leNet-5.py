"""
    Author: Zifan Wang
    Train LeNet-5 using MNIST Dataset
"""

import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
import keras 
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers.normalization import BatchNormalization
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.core import Activation, Flatten, Dropout, Dense
from keras import regularizers


# load train data and test data from mnist 
(x_train,y_train),(x_test,y_test) = mnist.load_data()
print(x_train.shape)
print(y_train.shape)
print(x_test.shape)
print(y_test.shape)

