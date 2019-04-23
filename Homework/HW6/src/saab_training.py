import numpy as np
import matplotlib.pyplot as plt
import pickle
import sklearn
import keras
import os
from skimage.util.shape import view_as_windows
from skimage.measure import block_reduce
from sklearn.decomposition import PCA
from sklearn.cluster import KMeans
from sklearn.metrics.pairwise import euclidean_distances
from numpy import linalg as LA
from itertools import product

class saab:
    def relu(self,x):
        """
            Relu function
            Argument: x input array
            Return: y
        """
        return np.maximum(0,x)

    def save_data(self,parameters,fileName):
        """
            Use to save data
            Arguments:
                parameters: data you want to save
                fileName: name of file
            Return: no
        """
        try:    
            fw = open(fileName,'wb')
        except:
            print('Error opening ' + fileName)
        pickle.dump(parameters,fw)
        fw.close()

    def parse_list_string(self,stringList):
        """
            Convert the input string to list
            Argument: 
                    stringList: string
            return: 
                    result: list
        """
        elem_groups = stringList.split(',')
        result = []                                 # a return result """
        for group in elem_groups:
            term = group.split('-')
            if len(term) == 1:
                result.append(int(term[0]))
            else:
                result += range(int(term[0]),int(term[1])+1)
        return result

    def window_process_1(self,samples,kernelSize,stride):
        """
            Create patches: imageChannel after kernelSize
            Arguments:
                samples: (type np) input image dataset. samples.shape = [numSamples,imageHeight,imageWidth,imageChannel]
                kernelSize: (type: int) windows' size. type: int
                stride: (type: int) number of steps to move window
            return:
                patches: flattened, with shape [numSamples,outHeight,outWidth,imageChannel*kernelSize^2]
        """
        numSamples,imageHeight,imageWidth,imageChannel = samples.shape              # get shape
        outHeight = (imageHeight - kernelSize) // stride + 1                        # set output image height
        outWidth = (imageWidth - kernelSize) // stride + 1                          # set output image width
        patches = view_as_windows(samples,(1,kernelSize,kernelSize,imageChannel), step = (1,stride,stride,imageChannel))       # create patches
        patches = patches.reshape(numSamples,outHeight,outWidth,imageChannel*kernelSize*kernelSize)                            # reshape

        return patches
    
    def window_process_2(self,samples,kernelSize,stride):
        """
            Create patches: imageChannel before kernelSize
            Arguments:
                samples: (type np) input image dataset. samples.shape = [numSamples,imageChannel,imageHeight,imageWidth]
                kernelSize: (type: int) windows' size. type: int
                stride: (type: int) number of steps to move window
            return:
                patches: (type np) flattened, with shape [numSamples,outHeight,outWidth,imageChannel*kernelSize^2]
        """
        numSamples,imageChannel,imageHeight,imageWidth = samples.shape              # get shape
        outHeight = (imageHeight - kernelSize) // stride + 1                        # set output image height
        outWidth = (imageWidth - kernelSize) // stride + 1                          # set output image width
        patches = view_as_windows(samples,(1,imageChannel,kernelSize,kernelSize), step = (1,imageChannel,stride,stride))         # create patches
        patches = patches.reshape(numSamples,outHeight,outWidth,imageChannel*kernelSize*kernelSize)                              # reshape

        return patches
    
    def window_process_3(self,samples,kernelSize,stride):
        """
            Create patches: according to kernelSize
            Arguments:
                samples: (type np) input image dataset. samples.shape = [numSamples,imageChannel,imageHeight,imageWidth]
                kernelSize: (type: int) windows' size. 
                stride: (type: int) number of steps to move window
            return:
                patches: (type np) flattened, with shape [numSamples,outHeight,outWidth,imageChannel*kernelSize^2]
        """
        numSamples,imageChannel,imageHeight,imageWidth = samples.shape              # get shape
        outHeight = (imageHeight - kernelSize) // stride + 1                        # set output image height
        outWidth = (imageWidth - kernelSize) // stride + 1                          # set output image width
        idx_1 = range(0,imageHeight-kernelSize+1,1)                                 # create a list of number from 0 to image height - kernel size +1.
        idx_2 = [i+kernelSize for i in idx_1]                                       # create a list of number from kernel size to image height+1
        # set up output patches
        data_lattice = np.array([samples[:,:,i:j,k:l] for ((i,j),(k,l)) in product(zip(idx_1,idx_2),zip(idx_1,idx_2))])
        # remove axis of data_lattice
        patches = np.moveaxis(data_lattice,1,0)
        # reshape
        patches = patches.reshape(numSamples,outHeight,outWidth,imageChannel*kernelSize*kernelSize)

        return patches

    def remove_mean(self,features,axis):
        """
            Remove mean from an input dataset
            Arguments:
                features: (type np) input dataset
                axis: (type int) the axis used to comput mean. must be 0 or 1
            Return:
                removedMeanFeatures: (type np) a array without feature mean
                featureMean: (type np) a feature mean array
        """
        featureMean = np.mean(features,axis=axis,keepdims=True)
        removedMeanFeatures = features-featureMean

        return removedMeanFeatures,featureMean
    
    def select_balanced_subset(self,images,labels,numImagesUsed,classUsed):
        """
            Select equal number of images from each used class
            Arguments:
                images: (type np) input images with shape [numImages,:,:,:]
                labels: (type np) input labels corresponding to the input images
                numImagesUsed: (type int) number of images used 
                classUsed: (type list) class to used. The number of classes is corresphonding to the input images which can be classified into several classes.
            Returns:
                selectedImages: (type np) shuffled and selected images based on original images & labels and numImagesUsed & classUsed
                selectedLabels: (type np) select labels corresponding to selectedImages
        """
        assert(images.shape[0] == labels.shape[0])              # number of images does not match number of labels
        # Shuffle the input images & labels
        numImages = images.shape[0]
        shuffle_idx = np.random.permutation(numImages)
        images = images[shuffle_idx]
        labels = labels[shuffle_idx]

        # Create output images & labels
        numClasses = len(classUsed)                            # get number of classes from input class_used list
        numPerClass = int(numImagesUsed/numClasses)           # get number of images per class
        selectedImages = np.zeros((numImagesUsed,images.shape[1],images.shape[2],images.shape[3]))
        selectedLabels = np.zeros(numImagesUsed)
        for i in range (numClasses):
            imagesInClass = images[labels == i]
            selectedImages[i*numPerClass:(i+1)*numPerClass] = imagesInClass[:numPerClass]
            selectedLabels[i*numPerClass:(i+1)*numPerClass] = np.ones((numPerClass))*i
        
        # Shuffle the output images & labels
        shuffle_idx = np.random.permutation(numPerClass*numClasses)
        selectedImages = selectedImages[shuffle_idx]
        selectedLabels = selectedLabels[shuffle_idx]

        return selectedImages,selectedLabels
    
    def find_kernels_pca(self,samples,numKernels=None,energyPercent=None,verbose=None):
        """
            Do the PCA based on the input samples.
            If numKernels is not set, will use energyPercent
            If neither numKernels nor energyPercent is set, will preserve all kernels.
            Arguments;
                samples: (type np) input samples [numSamples, featureDimension]
                numKernels: (type int) number of kernels to be preserved
                energyPercent: (type double) the percent of energy to be preserved
                verbose: (type bool) whether to print out some information message 
            Returns:
                kernels: (type np) kernels calculated from PCA.components_
                sampleMean: (type np) means from PCA.mean_
        """
        # Set up pca based on numKernels
        if numKernels:
            numComponents = numKernels
            pca = PCA(n_components=numComponents,svd_solver='full')
        else:
            pca = PCA(n_components=samples.shape[1],svd_solver='full')
        
        # Fit Samples
        pca.fit(samples)

        # Compute the number of kernels corresponding to preserved energy
        if energyPercent:
            energy = np.cumsum(pca.explained_variance_ratio_)
            numComponents = np.sum(energy < energyPercent) + 1

        # Get kernels & sampleMean
        kernels = pca.components_[:numComponents,:]
        sampleMean = pca.mean_
        
        # Print to check out 
        if(verbose):
            print("Number of kernels: %d" % numComponents)
            print("Energy percent: %f" % np.cumsum(pca.explained_variance_ratio_)[numComponents - 1])
        return kernels,sampleMean

    def multi_stage_saab_transform(self,
                                   images = None,
                                   labels = None,
                                   kernelSize = None,
                                   numKernels = None,
                                   energyPercent = None,
                                   numImagesUsed = None,
                                   classUsed = None,
                                   verbose = None):
        """
            Do the PCA training
            Arguments:
                images: (type np) the input image dataset with shape = [numImages,imageHeight,imageWidth,imageChannel]
                labels: (type np) the input labels corresponding to the input image with shape = [num_images]
                kernelSize: (type list) kernel size of each stage. the length defines how many stages conducted
                numKernels: (type list) number of kernels for each stage. the length should be equal to kernel_size if it is set. 
                energyPercent: (type float) the percent of energy to be kept in all PCA stages. If numKernels is set, energyPercent will be ignored
                numImagesUsed: (type int) the number of subset of training images
                classUsed: (type list) the classes of training images
                verbose: (type bool) whether to print out some messages during training process
            Returns:
                parameters: (type dictionary). A dictionary contains all important parameters of PCA.
                            keys: Layer_%d/feature_expectation
                                  Layer_%d/Kernel
                                  Layer_%d/pca_mean
                                  Layer_%d/bias
                                  num_layers
                                  kernel_size
        """
        parameters = {}                 # parameter dictionary creation
        numTotalImages = images.shape[0]
        
        # Set up training images 
        if numImagesUsed < numTotalImages and numImagesUsed > 0:
            assert(classUsed != None)                                                                           # classUsed parameter can not be empty
            sampleImages,_ = self.select_balanced_subset(images,labels,numImagesUsed,classUsed)
        else:
            sampleImages = images
        
        # Get numSamples & numLayers
        numSamples = sampleImages.shape[0]
        assert(kernelSize != None)                                                                              # kernelSize parameter can not be empty
        numLayers = len(kernelSize)
        # Set ('num_layers',numLayers) & ('kernel_size',kernelSize) pair in parameters dictionary
        parameters['num_layers'] = numLayers
        parameters['kernel_size'] = kernelSize

        # Training
        for i in range (numLayers):
            # Create Patches
            samplePatches = self.window_process_2(sampleImages,kernelSize[i],1)                                 # Overlapping
            _,h,w,_ = samplePatches.shape
            # Flatten
            samplePatches = samplePatches.reshape([-1,samplePatches.shape[-1]])

            # Remove feature mean
            centeredSamplePatches, featureExpectation = self.remove_mean(samplePatches, axis=0)

            # Remove patch mean
            trainingData,_ = self.remove_mean(centeredSamplePatches, axis=0)

            # Compute PCA kernel
            if not numKernels is None:
                numKernel = numKernels[i]
            kernels, mean = self.find_kernels_pca(trainingData,numKernel,energyPercent)

            # Add DC kernel
            numChannels = samplePatches.shape[1]
            dcKernel = 1/np.sqrt(numChannels) * np.ones((1,numChannels))
            kernels = np.concatenate((dcKernel,kernels),axis=0)

            if i == 0:
                # Transform to get data for the next stage
                transformed = np.matmul(centeredSamplePatches,np.transpose(kernels))
            else:
                # Compute bias term
                bias = LA.norm(samplePatches,axis=1)
                bias = np.max(bias)
                # Store bias
                parameters['Layer_%d/bias' % i] = bias
                # Add bias
                centeredSamplePatchesWithBias = centeredSamplePatches + 1/np.sqrt(numChannels) * bias
                # Transform to get data for the next stage
                transformed = np.matmul(centeredSamplePatchesWithBias,np.transpose(kernels))
                # remove bias
                e = np.zeros((1,kernels.shape[0]))
                e[0,0] = 1
                transformed -= bias * e
            
            # Update SampleImages. Reshape: place back as a 4-D feature map
            sampleImages = transformed.reshape(numSamples,h,w,-1)
            sampleImages = np.moveaxis(sampleImages,3,1)

            # Max-pooling
            sampleImages = block_reduce(sampleImages,(1,1,2,2),np.max)

            # Save parameters in layer i
            parameters['Layer_%d/feature_expectation' % i] = featureExpectation
            parameters['Layer_%d/Kernel' % i] = kernels
            parameters['Layer_%d/pca_mean' % i] = mean

            # Print method
            if(verbose):
                print('--------stage %d --------' % i)
                print('training_data.shape: {}'.format(trainingData.shape))
                print('Sample patches shape after flatten:', samplePatches.shape)
                print('Kernel shape:', kernels.shape)
                print('Transformed shape:', transformed.shape)
                print('Sample images shape:', sampleImages.shape)
        
        return parameters

    def initialize(self,sampleImages,parameters,verbose=None):
        """

            Arguments:
                sampleImages: (type np) input samples [numSamples, featureDimension] 
                parameters: (type dictionary). A dictionary contains all important parameters of PCA.
                            keys: Layer_%d/feature_expectation
                                  Layer_%d/Kernel
                                  Layer_%d/pca_mean
                                  Layer_%d/bias
                                  num_layers
                                  kernel_size
                verbose: (type bool) whether to print out some messages during initialize process
            Return:
                sampleImages: (type np)
        """
        numLayers = parameters['num_layers']
        kernelSize = parameters['kernel_size']
        
        for i in range(numLayers):
            # Extract parameters
            featureExpectation = parameters['Layer_%d/feature_expectation' % i].astype(np.float32)
            kernels = parameters['Layer_%d/Kernel' % i].astype(np.float32)

            # Create patches
            if(i == 0):
                samplePatches = self.window_process_2(sampleImages,kernelSize[i],1)
            elif(i == 1):
                samplePatches = self.window_process_3(sampleImages,kernelSize[i],1)
            
            _,h,w,_ = samplePatches.shape

            # Flatten
            samplePatches = samplePatches.reshape([-1,samplePatches.shape[-1]])

            # Remove feature mean (Set E(X)=0 for each dimension)
            samplePatchesCenterd = samplePatches - featureExpectation

            if(i == 0):
                transformed = np.matmul(samplePatchesCenterd,np.transpose(kernels))
            else:
                bias = parameters['Layer_%d/bias' % i].astype(np.float32)
                # Add bias
                samplePatchesCenterdWithBias = samplePatchesCenterd + 1/np.sqrt(150)*bias
                # Transform to get data for the next stage
                transformed = np.matmul(samplePatchesCenterdWithBias,np.transpose(kernels))
                # Remove bias
                e = np.zeros((1,kernels.shape[0]),dtype=np.float32)
                e[0,0] = 1
                transformed -= bias*e
            
            # Update sampleImages. Reshape: place back as 4D feature map
            sampleImages = transformed.reshape(sampleImages.shape[0],h,w,-1)
            sampleImages = np.moveaxis(sampleImages,3,1)

            # Max-pooling
            sampleImages = block_reduce(sampleImages,(1,1,2,2),np.max)

            if(verbose):
                print('--------stage %d --------' % i)
                print('Sample patches shape after flatten:', samplePatches.shape)
                print('Kernel shape:', kernels.shape)
                print('Transformed shape:', transformed.shape)
                print('Sample images shape:', sampleImages.shape)

        return sampleImages
    
    def get_kernel_compact(self,
                           images = None,
                           labels = None,
                           kernelSize = None,
                           numKernels = None,
                           energyPercent = None,
                           numImagesUsed = None,
                           classUsed = None,
                           verbose = None):
        """
            Do the PCA training
            Arguments:
                images: (type np) the input image dataset with shape = [numImages,imageHeight,imageWidth,imageChannel]
                labels: (type np) the input labels corresponding to the input image with shape = [num_images]
                kernelSize: (type string) kernel size of each stage. the length defines how many stages conducted
                numKernels: (type string) number of kernels for each stage. the length should be equal to kernel_size if it is set. 
                energyPercent: (type float) the percent of energy to be kept in all PCA stages. If numKernels is set, energyPercent will be ignored
                numImagesUsed: (type int) the number of subset of training images
                classUsed: (type list) the classes of training images
                verbose: (type bool) whether to print out some messages during training process
            Returns:
                parameters: (type dictionary). A dictionary contains all important parameters of PCA.
                            keys: Layer_%d/feature_expectation
                                  Layer_%d/Kernel
                                  Layer_%d/pca_mean
                                  Layer_%d/bias
                                  num_layers
                                  kernel_size
        """
        # Get parameters of each layer
        assert(kernelSize is not None)                        # No argument kernelSize found
        kernel_sizes = self.parse_list_string(kernelSize)

        if(numKernels is not None):
            num_kernels = self.parse_list_string(numKernels)
        else:
            num_kernels = None

        energy_percent = energyPercent
        use_num_images = numImagesUsed

        trainingImage = np.moveaxis(images,3,1)

        print('Getting Kernel')
        # Training
        pcaParameters = self.multi_stage_saab_transform(images=trainingImage,labels=labels,kernelSize=kernel_sizes,numKernels=num_kernels,energyPercent=energy_percent,numImagesUsed=use_num_images,classUsed=classUsed,verbose=verbose)

        # Save pcaParameters
        self.save_data(pcaParameters,'pca_params_compact.pkl')

        return pcaParameters

    def get_feature_compact(self,trainingImages,parameters,verbose):
        """
            Get feature
            Arguments:
                trainingImages: (type np) input images [numSamples, featureDimension] 
                parameters: (type dictionary). A dictionary contains all important parameters of PCA.
                            keys: Layer_%d/feature_expectation
                                  Layer_%d/Kernel
                                  Layer_%d/pca_mean
                                  Layer_%d/bias
                                  num_layers
                                  kernel_size
                verbose: (type bool) whether to print out some messages during initialize process
            Return:
                feat: (type dictionary). A dictionary contains feature. key -- feature. value -- images
        """
        train_images = np.moveaxis(trainingImages,3,1)
        print('Getting feature')
        feature = self.initialize(sampleImages=train_images,parameters=parameters,verbose=verbose)
        feature = feature.reshape(feature.shape[0], -1)
        feat = {}
        feat['feature'] = feature

        self.save_data(feat,'feat_compact.pkl')

        return feat

    def get_weight_compact(self,
                           images = None,
                           train_labels = None,
                           kernelSize = None,
                           numKernels = None,
                           energyPercent = None,
                           numImagesUsed = None,
                           classUsed = None,
                           verbose = None):
        """
            Arguments:
                images: (type np) the input image dataset with shape = [numImages,imageHeight,imageWidth,imageChannel]
                train_labels: (type np) the input labels corresponding to the input image with shape = [num_images]
                kernelSize: (type string) kernel size of each stage. the length defines how many stages conducted
                numKernels: (type string) number of kernels for each stage. the length should be equal to kernel_size if it is set. 
                energyPercent: (type float) the percent of energy to be kept in all PCA stages. If numKernels is set, energyPercent will be ignored
                numImagesUsed: (type int) the number of subset of training images
                classUsed: (type list) the classes of training images
                verbose: (type bool) whether to print out some messages during training process
            Returns:
        """
        # Get feat_compact.pkl
        exists = os.path.exists('feat_compact.pkl')
        if not exists:
            # Get kernel
            pcaParameters = self.get_kernel_compact(images=images, labels=train_labels, kernelSize=kernelSize, numKernels=numKernels, energyPercent=energyPercent, numImagesUsed=numImagesUsed, classUsed=classUsed, verbose=verbose)
            # Get sample images
            feat = self.get_feature_compact(trainingImages=images, parameters = pcaParameters, verbose = verbose)
        else:
            # load feature
            fr = open('feat_compact.pkl', 'rb')
            feat = pickle.load(fr)
            fr.close()
        # Get feature
        feature = feat['feature']

        if(verbose):
            print('feature type: ', feature.dtype)
            print("S4 shape:", feature.shape)

        # Reshape and move axis
        feature=feature.reshape(60000, 16, 5, 5)
        feature=np.moveaxis(feature, 1, 3)
        feature=feature.reshape(-1, 5*5*16)


        num_clusters = [120, 84, 10]
        use_classes = 10
        weights = {}
        bias = {}
        for k in range(len(num_clusters)):
            if k != len(num_clusters) - 1:
                # Kmeans
                kmeans = KMeans(n_clusters=num_clusters[k]).fit(feature)
                pred_labels = kmeans.labels_
                num_clas = np.zeros((num_clusters[k], use_classes), dtype=np.float32)
                for i in range(num_clusters[k]):
                    for t in range(use_classes):
                        for j in range(feature.shape[0]):
                            if pred_labels[j] == i and train_labels[j] == t:
                                num_clas[i, t] += 1
                acc_train = np.sum(np.amax(num_clas, axis=1)) / feature.shape[0]
                if(verbose):
                    print(k, ' layer Kmean (just ref) training acc is {}'.format(acc_train))

                # Compute centroids
                clus_labels = np.argmax(num_clas, axis=1)
                centroid = np.zeros((num_clusters[k], feature.shape[1]), dtype=np.float32)
                for i in range(num_clusters[k]):
                    t = 0
                    for j in range(feature.shape[0]):
                        if pred_labels[j] == i and clus_labels[i] == train_labels[j]:
                            if t == 0:
                                feature_test = feature[j].reshape(1, -1)
                            else:
                                feature_test = np.concatenate((feature_test, feature[j].reshape(1, -1)), axis=0)
                            t += 1
                    centroid[i] = np.mean(feature_test, axis=0, keepdims=True)

                # Compute one hot vector
                t = 0
                labels = np.zeros((feature.shape[0], num_clusters[k]), dtype=np.float32)
                for i in range(feature.shape[0]):
                    if clus_labels[pred_labels[i]] == train_labels[i]:
                        labels[i, pred_labels[i]] = 1
                    else:
                        distance_assigned = euclidean_distances(feature[i].reshape(1, -1),
                                                                centroid[pred_labels[i]].reshape(1, -1))
                        cluster_special = [j for j in range(num_clusters[k]) if clus_labels[j] == train_labels[i]]
                        distance = np.zeros(len(cluster_special))
                        for j in range(len(cluster_special)):
                            distance[j] = euclidean_distances(feature[i].reshape(1, -1),
                                                            centroid[cluster_special[j]].reshape(1, -1))
                        labels[i, cluster_special[np.argmin(distance)]] = 1

                # least square regression
                A = np.ones((feature.shape[0], 1), dtype=np.float32)
                feature = np.concatenate((A, feature), axis=1)
                weight = np.matmul(LA.pinv(feature), labels)
                feature = np.matmul(feature, weight)
                if(verbose):
                    print ('weight {}  dtype: {} '.format(i, weight.dtype))
                    print ('weights save....')
                weights['%d LLSR weight' % k] = weight[1:weight.shape[0]]
                if(verbose):
                    print ('weights saved!')
                bias['%d LLSR bias' % k] = weight[0].reshape(1, -1)
                if(verbose):
                    print(k, ' layer LSR weight shape:', weight.shape)
                    print(k, ' layer LSR output shape:', feature.shape)

                pred_labels = np.argmax(feature, axis=1)
                num_clas = np.zeros((num_clusters[k], use_classes), dtype=np.float32)
                for i in range(num_clusters[k]):
                    for t in range(use_classes):
                        for j in range(feature.shape[0]):
                            if pred_labels[j] == i and train_labels[j] == t:
                                num_clas[i, t] += 1
                acc_train = np.sum(np.amax(num_clas, axis=1)) / feature.shape[0]
                if(verbose):
                    print(k, ' layer LSR training acc is {}'.format(acc_train))

                # Relu
                feature = self.relu(feature)
                # for i in range(feature.shape[0]):
                #     for j in range(feature.shape[1]):
                #         if feature[i, j] < 0:
                #             feature[i, j] = 0

            else:
                # least square regression
                labels = keras.utils.to_categorical(train_labels, 10)
                A = np.ones((feature.shape[0], 1), dtype=np.float32)
                feature = np.concatenate((A, feature), axis=1)
                weight = np.matmul(LA.pinv(feature), labels).astype(np.float32)
                if(verbose):
                    print ('weight {}  dtype: {} '.format(i, weight.dtype))
                feature = np.matmul(feature, weight)
                weights['%d LLSR weight' % k] = weight[1:weight.shape[0]]
                bias['%d LLSR bias' % k] = weight[0].reshape(1, -1)
                if(verbose):    
                    print(k, ' layer LSR weight shape:', weight.shape)
                    print(k, ' layer LSR output shape:', feature.shape)

                pred_labels = np.argmax(feature, axis=1)
                acc_train = sklearn.metrics.accuracy_score(train_labels, pred_labels)
                if(verbose):
                    print('training acc is {}'.format(acc_train))

        # save data
        fw = open('llsr_weights_compact_v2.pkl', 'wb')
        pickle.dump(weights, fw, protocol=2)
        fw.close()
        fw = open('llsr_bias_compact_v2.pkl', 'wb')
        pickle.dump(bias, fw, protocol=2)
        fw.close()


