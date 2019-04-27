# EE569 Homework Assignment #6
# Date: April 26, 2019
# Name: Zifan Wang
# ID: 9505-5872-96
# email: zifanw@usc.edu
#
# Compiled on Ubuntu 18.04.1 LTS

Lauguage: python3.6
Tensorflow: 1.13.1
Keras: 2.2.4

Core codes:
    data.py: handle data preparation
    saab_training.py: calculate saab parameters. Reconstruct code from https://github.com/davidsonic/Interpretable_CNN/tree/master/MNIST_FF
    feedforward_model.py: use saab coefficients in the CNN. Reconstruct code from https://github.com/davidsonic/Interpretable_CNN/blob/master/adv_attack/mnist_ff_model.py

Problem 1: Understanding of feedforward-designed CNN
    No Code Requried

Problem 2: Image reconstructions from Saab coefficients
    File name: saab_reconstruction.py. (will give PSNR value and reconstructed image)
    Run Code:
        python3 saab_reconstruction.py

Problem 3: Handwritten digits recognition using ensembles of feedforward design
    a. Individual FF-CNN on the MNIST dataset
        File name: singleLeNet5_FF.py 
        Return: Ensemble Accuracy.
        Run Code:
            python3 singleLeNet5_FF.py
    b. Train ten different FF-CNNs 
        File name: tenEnsembleFFCNN_fixedParam.py
        Return: Ensemble Accuracy.
        Run Code: 
            python3 tenEnsembleFFCNN_fixedParam.py
    c. Train ten different FF-CNNs with diversity increasing
        File name: tenEnsembleFFCNN_fixedParam.py
        Return: Ensemble Accuracy.
        Run Code: 
            python3 tenEnsembleFFCNN_lawsFilter.py
    