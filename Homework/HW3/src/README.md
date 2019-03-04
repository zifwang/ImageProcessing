# EE569 Homework Assignment #3
# Date: March 03, 2019
# Name: Zifan Wang
# ID: 9505-5872-96
# email: zifanw@usc.edu
#
# Compiled on Ubuntu 18.04.1 LTS
Lauguage: c++
Complier: g++


The makefile is used to run the code
After type make in terminal. type "./HW3 help" to know detail of Runing code
Problem 1:
    A. GeoTransformation: no white line in the boundary.
        File name: geoTransformation.cpp/h.
        Input: image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "GeoTransformation"
        Output: A output image (out.raw).
        Function: Fill holes.
        Run Code: 
        make
        ./HW3 image_ori.raw out.raw GeoTransformation ImageHeight ImageWidth Byte_per_pixel
    
    B. Spatial Warping:
        File name: main.m, readraw.m, spatialWarp.m.
        Input: image_1_path(spatial warp),image_1_height,image_1_width,image_2_path(lens distortion),image_2_height,image_2_width. 
        Output: Displayed the output image.
        Function: Warp the image
        Run Code: 
        Open matlab to run code.
    
    C. Lens Distortion:
        File name: lensUndistortion.h
        Input: image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "LensUndistortion"
        Output: Corrected image
        Function: Remove lens Distortion
        Run Code: 
        ./HW3 image_ori.raw out.raw LensUndistortion ImageHeight ImageWidth Byte_per_pixel

Problem 2:
    A. Shrink
        File name: morophology.h.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "Shrink"
        Output: shrinked image
        Function: Shrink objects
        Run Code: 
        make
        ./HW3 image_ori.raw out.raw Shrink ImageHeight ImageWidth Byte_per_pixel
    
    B. Thin:
        File name: morophology.h.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "Thin"
        Output: thinned image.
        Function: thin objects.
        Run Code: 
        make
        ./HW3 image_ori.raw out.raw Thin ImageHeight ImageWidth Byte_per_pixel
    
    C. Skeletonzie:
        File name: morophology.h.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "Skeletonzie"
        Output: Skeletonized image(out.raw).
        Function: Skeletonized objects.
        Run Code: 
        make
        ./HW3 image_ori.raw out.raw Skeletonzie ImageHeight ImageWidth Byte_per_pixel

    D. DetectionDefect:
        File name: defectDetection.h.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "DetectionDefect"
        Output: Defects removed image.
        Function: Detect defects and remove them.
        Run Code: 
        make
        ./HW3 image_ori.raw out.raw DetectionDefect ImageHeight ImageWidth Byte_per_pixel
    
    E. ObjectAnalysis: without using opencv library.
        File name: objectAnalysis.h.
        Input: a color raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "ObjectAnalysis"
        Output: (out.raw).
        Function: Shrink, Thin objects images and the image after preprocessing.
        Run Code: 
        make
        ./HW3 image_ori.raw out.raw ObjectAnalysis ImageHeight ImageWidth Byte_per_pixel

