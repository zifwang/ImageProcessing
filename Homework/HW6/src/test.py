import numpy as np
import keras
import data
import saab_training

if __name__ == '__main__':
    train_images, train_labels, test_images, test_labels, class_list = data.import_data('0-9')
    saabTraining = saab_training.saab()
    # parameters = saabTraining.get_kernel_compact(train_images,train_labels,'5,5','5,15',None,10000,class_list,True)
    # feature = saabTraining.get_feature_compact(train_images,parameters,True)
    saabTraining.get_weight_compact(train_images,train_labels,'5,5','5,15',None,10000,class_list,True)