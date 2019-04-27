
import numpy as np
import cv2
import pickle
import saab_training
import data
from numpy import linalg as LA
from skimage.util.shape import view_as_windows

# img = cv2.imread('../HW6_images/4.png',0)

def get_parameters(fileName):
    """
        Function used to get saab parameters
        Argument:
            fileName: location of pca_params_compact
        Return:
            parameter: dictionary
    """
    try:
        fr = open(fileName,'rb')
        params = pickle.load(fr)
        fr.close()
    except:
        print('File ' + fileName + ' not found')

    return params

def extract_feature(fileName,pca_params,n1,n2):
    """
        Function to read image and extract features from this input image
        window_shape in view_as_window is set to 4*4, and step is set to 4 for non-overlapping
        Argument:
            fileName: location of image
            pca_params: a dictionary contains kernel
            n1,n2: number of kernel in each stage
        Return:
            features: (type np) features extracted from image according to pca_params
            img: input image
    """
    # Get kernel and bais
    kernel0 = np.array(pca_params['Layer_0/kernel'])
    kernel1 = np.array(pca_params['Layer_1/kernel'])
    bias1 = pca_params['Layer_1/bias'].astype(np.float32)
    # print(bias1)
    # print('kernel0 shape: ',kernel0.shape)
    # print('kernel1 shape: ',kernel1.shape)
    # print('bias1 shape',bias1.shape)

    # Read image
    try:
        img = cv2.imread(fileName,0)
    except:
        print('File ' + fileName + ' not found')
    # img = img/255.
    # Extract features
    features = view_as_windows(img,(4,4),step=(4,4)).reshape(8,8,1*4**2)
    # print(features.shape)
    features = np.dot(features,np.transpose(kernel0))
    # print(features.shape)
    features = view_as_windows(features.copy(),(4,4,1),step=(4,4,1))
    # print(features.shape)
    features = features.reshape(2,2,n1*16)
    # print(features.shape)
    features = features + 1/np.sqrt(n1*n2) * bias1
    # print(features.shape)
    features = np.dot(features,np.transpose(kernel1))
    # print(features.shape)

    return img,features

def inv_transformation(features,pca_params,n1,n2):
    """
        Function to inverse saab transformation
        Argument:
            features: 
        Return:
            outImageGray: output image with gray scale from inverse saab transformation
            normalizedOut: normalized output image
    """
    # Get kernel and bais
    kernel0 = np.array(pca_params['Layer_0/kernel'])
    kernel1 = np.array(pca_params['Layer_1/kernel'])
    bias1 = pca_params['Layer_1/bias'].astype(np.float32)

    # Do inverse transformation
    inv_features = np.dot(features,LA.pinv(np.transpose(kernel1)))
    inv_features = inv_features - 1/np.sqrt(n1*n2) * bias1
    inv_features = inv_features.reshape(2,2,n1,16)
    inv_features = np.moveaxis(inv_features,2,3)

    # image reconstruct
    # stage 1
    outImageGray = np.zeros((8,8,n1))
    for i in range (0,2):
        for j in range (0,2):
            for k in range (0,16):
                outImageGray[i*4+k//4,j*4+k%4] = inv_features[i,j,k]
    inv_features = outImageGray
    inv_features = np.dot(inv_features,LA.pinv(np.transpose(kernel0)))
    # stage 0
    outImageGray = np.zeros((32,32))
    for i in range (0,8):
        for j in range (0,8):
            for k in range (0,16):
                outImageGray[i*4+k//4,j*4+k%4] = inv_features[i,j,k]

    # normalize output image
    normalizedOut = 255*(outImageGray-np.min(outImageGray))/(np.max(outImageGray)-np.min(outImageGray))
    
    return outImageGray,normalizedOut

def PSNR(oriImage,recImage):
    """
        Function to calculate PSNR value (high - same) or (low - not same)
        Argument:
            oriImage:
            recImage:
        Return:
            psnr
    """
    mse = np.mean(np.square(recImage-oriImage))

    return 10*np.log10(255*255/mse)

if __name__ == '__main__':
    n1 = 5
    n2 = 15
    num_kernels = str(n1)+','+ str(n2)
    # params = get_parameters('pca_params_compact.pkl')
    train_images, train_labels, test_images, test_labels, class_list = data.import_data('0-9')
    mySaab = saab_training.saab()
    params = mySaab.get_kernel_compact(train_images,train_labels,'4,4',[4,4],num_kernels,False,None,10000,class_list,True)
    img, feature = extract_feature(fileName='/home/zifwang/Documents/ImageProcessing/Homework/HW6/HW6_images/1.png',pca_params = params,n1=n1+1,n2=n2+1)
    outImageGray,normalizedOut = inv_transformation(features=feature,pca_params=params,n1=n1+1,n2=n2+1)
    psnr = PSNR(img,outImageGray)
    print('PSNR: ',psnr)
    cv2.imwrite('reconstruct.png',normalizedOut)