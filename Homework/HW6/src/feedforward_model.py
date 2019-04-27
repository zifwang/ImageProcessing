"""
    Author: Zifan Wang
    The feedforwardCNN class will use saab coefficients in the CNN. 
    Reconstruct code from https://github.com/davidsonic/Interpretable_CNN/blob/master/adv_attack/mnist_ff_model.py
"""
import keras
import pickle
import numpy as np
from keras.datasets import mnist
from keras.layers import Conv2D, MaxPool2D, Dense, Flatten, Dropout, Subtract
from keras.layers.normalization import BatchNormalization
from keras.models import Sequential
from keras import optimizers
from keras.models import load_model
import os
from keras import backend as K
K.tensorflow_backend._get_available_gpus()
import saab_training
import data

class feedforwardCNN():
    """
        Constructor
    """
    def __init__(self,pcaName=None,weightName=None,biasName=None):
        self.pca_name = pcaName
        self.weight_name = weightName
        self.bias_name = biasName
        self.pca_params = 0
        self.weight = 0
        self.biases = 0
        self.trainingSaab = saab_training.saab()

    """
        Check whether provided name is valid
    """
    def isFileProvided(self):
        if(self.pca_name and self.weight_name and self.bias_name):
            exists_pca = os.path.exists(self.pca_name)
            exists_weight = os.path.exists(self.weight_name)
            exists_bias = os.path.exists(self.bias_name)
            if exists_pca and exists_weight and exists_bias:
                return True

        return False

    """
        Load init. parameters
    """
    def loadInitParameters(self,useProvidedParam,useMNIST,images,labels,kernelSize,stride,numKernels,maxPooling,energyPercent,numImagesUsed,classUsed,verbose):
        """
            Arguments:
                useProvidedParam: (type bool) choose to use provided parameters
                useMNIST: (type bool) choose to use MNIST data set
                images: (type np) the input image dataset with shape = [numImages,imageHeight,imageWidth,imageChannel]
                train_labels: (type np) the input labels corresponding to the input image with shape = [num_images]
                kernelSize: (type string) kernel size of each stage. the length defines how many stages conducted
                numKernels: (type string) number of kernels for each stage. the length should be equal to kernel_size if it is set. 
                energyPercent: (type float) the percent of energy to be kept in all PCA stages. If numKernels is set, energyPercent will be ignored
                numImagesUsed: (type int) the number of subset of training images
                classUsed: (type list) the classes of training images
                verbose: (type bool) whether to print out some messages during training process
        """
        if self.isFileProvided and useProvidedParam:
            # get pca_params
            myfile = open(self.pca_name,'rb')
            self.pca_params = pickle.load(myfile)
            # get weight in FC layer
            myfile = open(self.weight_name,'rb')
            self.weight = pickle.load(myfile)
            # get bias in FC layer
            myfile = open(self.bias_name,'rb')
            self.biases = pickle.load(myfile)
        else:
            train_images = images
            train_labels = labels
            class_list = classUsed
            if useMNIST:
                train_images, train_labels, _, _, class_list = data.import_data('0-9')
            pcaParameters, _, weights, bias = self.trainingSaab.get_weight_compact(train_images,train_labels,
                                                                                        kernelSize,stride,numKernels,maxPooling,energyPercent,numImagesUsed,class_list,verbose)
            self.pca_params = pcaParameters
            self.weight = weights
            self.biases = bias
    
    """
        get weight in conv1 layer
    """
    def getInitWeightConv1(self, shape, dtype=None):
        weight = self.pca_params['Layer_0/kernel'].astype(np.float32)
        weight = np.moveaxis(weight, 1, 0)
        
        return weight.reshape(shape)
    
    """
        get bias in conv1 layer
    """
    def getInitBiasConv1(self, shape, dtype=None):
        bias = (-1*np.matmul(self.pca_params['Layer_0/feature_expectation'],
                               np.transpose(self.pca_params['Layer_0/kernel']))).reshape(shape)
        
        return bias
    
    """
        get weight in conv2 layer
    """
    def getInitWeightConv2(self, shape, dtype=None):
        weight = self.pca_params['Layer_1/kernel'].astype(np.float32)

        weight = np.moveaxis(weight, 1,0)
        
        weight = weight.reshape(6,5,5, 16)
        
        weight = np.moveaxis(weight, 0, 2)

        return weight.reshape(shape)

    
    """
        get bias in conv2 layer
    """
    def getInitBiasConv2(self, shape, dtype=None):
        weight = np.transpose(self.pca_params['Layer_1/kernel'].astype(np.float32))
        tmp_bias = (-1 * np.matmul(self.pca_params['Layer_1/feature_expectation'].astype(np.float32), weight)).reshape(shape)

        bias = np.zeros(150)
        bias = bias + 1/np.sqrt(150)*self.pca_params['Layer_1/bias'].astype(np.float32)

        bias1 = np.matmul(bias, weight)

        bias2 = np.zeros(shape, dtype=np.float32)
        bias2[0] = -1
        bias2 = bias2 * self.pca_params['Layer_1/bias'].astype(np.float32)

        bias_final = tmp_bias + bias1 + bias2
        
        return bias_final.reshape(shape)
    
    """
        get weight in fc1 layer
    """
    def weight_init_fc_1(self, shape, dtype=None):
        return self.weight['0 LLSR weight'].reshape(shape)

    """
        get bias in fc1 layer
    """
    def bias_init_fc_1(self, shape, dtype=None):
        return self.biases['0 LLSR bias'].reshape(shape)

    """
        get weight in fc2 layer
    """
    def weight_init_fc_2(self, shape, dtype=None):
        return self.weight['1 LLSR weight'].reshape(shape)

    """
        get bias in fc2 layer
    """
    def bias_init_fc_2(self, shape, dtype=None):
        return self.biases['1 LLSR bias'].reshape(shape)

    """
        get weight in fc3 layer
    """
    def weight_init_fc_3(self, shape, dtype=None):
        return self.weight['2 LLSR weight'].reshape(shape)

    """
        get bias in fc3 layer
    """
    def bias_init_fc_3(self, shape, dtype=None):
        return self.biases['2 LLSR bias'].reshape(shape)
    
    """
        leNet5
    """
    def feedforward_leNet5(self):
        # print(kernelSize)
        leNet5 = Sequential()
        leNet5.add(Conv2D(6,
                     activation=None,
                     kernel_size=5,
                     strides=1,
                     padding='valid',
                     data_format='channels_last',
                     kernel_initializer=self.getInitWeightConv1,
                     input_shape=(32, 32, 1),
                     bias_initializer=self.getInitBiasConv1) ) # bias?
        leNet5.add(MaxPool2D(pool_size=2,
                            strides=2))
        leNet5.add(Conv2D(16,
                        activation=None,
                        kernel_size=5,
                        strides=1,
                        padding='valid',
                        data_format='channels_last',
                        kernel_initializer=self.getInitWeightConv2,
                        bias_initializer=self.getInitBiasConv2))
        leNet5.add(MaxPool2D(pool_size=2,
                            strides=2))
        leNet5.add(Flatten())
        leNet5.add(Dense(120,
                        activation='relu',
                        kernel_initializer=self.weight_init_fc_1,
                        bias_initializer=self.bias_init_fc_1))
        leNet5.add(Dense(84,
                        activation='relu',
                        kernel_initializer=self.weight_init_fc_2,
                        bias_initializer=self.bias_init_fc_2))
        leNet5.add(Dense(10,
                        activation='softmax',
                        kernel_initializer=self.weight_init_fc_3,
                        bias_initializer=self.bias_init_fc_3))
        leNet5.compile(loss='categorical_crossentropy',
                    optimizer=keras.optimizers.SGD(lr=0.001),
                    metrics=['accuracy'])

        return leNet5
