# EE569 Homework Assignment #4
# Date: March 19, 2019
# Name: Zifan Wang
# ID: 9505-5872-96
# email: zifanw@usc.edu
#
# Compiled on Ubuntu 18.04.1 LTS
Lauguage: c++
Complier: g++
OpenCV: 3.4.6

The makefile is used to run the code
After type make in terminal. type "./HW4 help" to know detail of Runing code
If there are problems while making the file, it can be the OpenCV library error. Use OpenCV: 3.4.6 library.

Problem 1:

    A. Texture Classification: no white line in the boundary.
        File name: textureClassification.cpp/h.
        Input: imageIn (file location+texture.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "TC"
        Output: A imageOut (out.raw).
        Run Code: 
        make
        ./HW4 imageIn imageOut TC ImageHeight ImageWidth Byte_per_pixel
    
    B. Texture Segmentation:
        File name: textureSegmentation.cpp/h.
        Input: image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "TS"
        Output: A output image (out.raw).
        Function: Texture Segmentation
        Run Code: 
        make
        ./HW4 image_ori.raw out.raw GeoTransformation ImageHeight ImageWidth Byte_per_pixel
    
    C. Advanced Texture Segmentation:
        File name: textureSegmentation.cpp/h.
        Input: image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "TS"
        Output: A output image (out.raw).
        Function: Texture Segmentation
        Run Code: 
        make
        ./HW4 image_ori.raw out.raw GeoTransformation ImageHeight ImageWidth Byte_per_pixel

Problem 2:

    A. SIFT understanding: No Code required
    
    B. SIFT Image Matching:
        File name: SIFT_Experiment.h.
        Input: imageIn (File location+river.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "SIFT"
        Output: a matched keypoint image(out.jpg).
        Function: Image Matching.
        Run Code: 
        make
        ./HW4 imageIn raw out.jpg SIFT ImageHeight ImageWidth Byte_per_pixel

    B. Bag of Words Digit Classification:
        File name: morophology.h.
        Input: training image location (File location). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "BOW". numClass. numImagePerClass
        Output: thinned image.
        Function: Bag of Words Digit Classification.
        Run Code: 
        make
        ./HW4 location out.raw BOW ImageHeight ImageWidth Byte_per_pixel numClass numImagePerClass
    
