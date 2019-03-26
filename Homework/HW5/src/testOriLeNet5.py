"""
    Author: Zifan Wang
    Used Trained leNet5 to classifier reversed color mnist data
"""

import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
import keras 
from keras.datasets import mnist
from keras.models import Sequential, Model, load_model
from keras.layers import Input
from keras.layers.normalization import BatchNormalization
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.core import Activation, Flatten, Dropout, Dense
from keras import regularizers

# load train data and test data from mnist 
(x_train,y_train),(x_test,y_test) = mnist.load_data()
numData,height,width = x_test.shape
x_test = np.subtract(np.ones((numData,height,width))*255,x_test)    # reverse color
x_test = x_test.reshape(10000, 28, 28, 1)
y_test = keras.utils.to_categorical(y_test,10)

# load my keras model
model = load_model('model_adam_relu.h5')
# Test Set Accuracy
score = model.evaluate(x_test,y_test,verbose=1)
print('Test Loss: ', score[0])
print('Test Accuracy: ', score[1])

"""
    Test Loss:  9.106315967559814
    Test Accuracy:  0.2274
"""