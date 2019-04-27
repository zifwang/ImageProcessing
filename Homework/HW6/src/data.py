import keras
from keras.datasets import mnist
from keras.models import load_model
import numpy as np
import saab_training
import cv2

def get_data_for_class(images, labels, cls):
	if type(cls)==list:
		idx=np.zeros(labels.shape, dtype=bool)
		for c in cls:
			idx=np.logical_or(idx, labels==c)
	else:
		idx=(labels==cls)
	return images[idx], labels[idx]

def import_data(use_classes):
	(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
	train_images=train_images.reshape(-1,28,28,1)
	test_images=test_images.reshape(-1,28,28,1)
	train_images=train_images/255.
	test_images=test_images/255.

	train_images=np.float32(train_images)
	test_images=np.float32(test_images)

	# zeropadding
	train_images=np.pad(train_images, ((0,0),(2,2),(2,2),(0,0)), mode='constant')
	test_images=np.pad(test_images,  ((0,0),(2,2),(2,2),(0,0)), mode='constant')
	# print(train_images.shape) # 60000*32*32*1

	if use_classes!='0-9':
		mySaab = saab_training.saab()
		class_list=mySaab.parse_list_string(use_classes)
		train_images, train_labels=get_data_for_class(train_images, train_labels, class_list)
		test_images, test_labels=get_data_for_class(test_images, test_labels, class_list)
		# print(class_list)
	else:
		class_list=[0,1,2,3,4,5,6,7,8,9]
		
	return train_images, train_labels, test_images, test_labels, class_list

def get_mnist_data():
	(x_train, y_train), (x_test, y_test) = mnist.load_data()
	x_train = x_train.reshape(-1, 28, 28, 1)
	x_test = x_test.reshape(-1, 28, 28, 1)
	x_train = x_train/255.
	x_test = x_test/255.

	x_train = np.float32(x_train)
	x_test = np.float32(x_test)

	x_train=np.pad(x_train, ((0,0),(2,2),(2,2),(0,0)), mode='constant')
	x_test=np.pad(x_test,  ((0,0),(2,2),(2,2),(0,0)), mode='constant')

	return x_train, y_train, x_test, y_test

def data_separation(xTrain,yTrain):
	counter = np.zeros((10))
	index = np.array([0,1,2,3,4,5,6,7,8,9])*5000

	x_train = np.zeros_like(xTrain)
	y_train = np.zeros_like(yTrain)

	for i in range(0,xTrain.shape[0]):
		if counter[yTrain[i]] > 4999:
			continue
		position = int(counter[yTrain[i]]//500)
		x_train[index[position]] = xTrain[i]
		y_train[index[position]] = yTrain[i]

		index[position] = index[position] + 1
		counter[yTrain[i]] = counter[yTrain[i]] + 1

	return x_train, y_train

def applyLawsFilter(xTrain):
	kernel = np.array([[1,2,1],[-1,0,1],[-1,2,-1]])
	
	filter_bank = []
	for i in range(0,3):
		for j in range(0,3):
			filter_bank.append(np.dot(kernel[i].reshape(3,1), kernel[j].reshape(1,3)).reshape(3,3))
	filter_bank = np.array(filter_bank)
	
	x_train = []
	x_train.append(xTrain)

	for i in range(0,9):
		tmpImage = np.zeros_like(xTrain)
		for k in range(0,xTrain.shape[0]):
			tmpImage_1 = cv2.filter2D(xTrain[k], -1, filter_bank[i])
			tmpImage[k] = tmpImage_1.reshape(32,32,1)
		x_train.append(tmpImage)

	x_train = np.array(x_train).reshape(10,-1,32,32,1)

	return x_train

def compare_bp_ffCNN(y_ff_test):
	(_, _), (test_x, test_y) = mnist.load_data()
	test_x = test_x.reshape(-1,28,28,1)
	test_y = keras.utils.to_categorical(test_y,10)

	bpCNN = load_model('model_adam_relu.h5')
	bpPredict = bpCNN.predict(test_x)
	bpPredict = np.eye(10)[np.argmax(bpPredict, axis=1)]

	bp_ff = bpPredict - y_ff_test
	ff_bp = y_ff_test - bpPredict

	bp_ff[bp_ff < 0] = 0
	ff_bp[ff_bp < 0] = 0

	numDifPred = np.sum(bp_ff)
	accBP = np.sum(bp_ff*test_y)/np.sum(bp_ff)
	accFF = np.sum(ff_bp*test_y)/np.sum(ff_bp)

	print('Number of different prediction: %5d' % (numDifPred))
	print('Accuracy in BP of different prediction data: %5f' % accBP)
	print('Accuracy in feedforward of different prediction data: %5f' % accFF)