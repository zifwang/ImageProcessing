# EE569 Homework Assignment #2
# Date: Feb 12, 2019
# Name: Zifan Wang
# ID: 9505-5872-96
# email: zifanw@usc.edu
#
# Compiled on Ubuntu 18.04.1 LTS
Lauguage: c++
Complier: g++
OS must have openCV lib.

After type make in terminal. type ./HW2 help to know detail of Runing code
Problem 1:
    A. Sobel Edge Detector:
        File name: soeblEdgeDetector.cpp/h.
        Input: image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "SOBEL"
        Output: A output edge map image (out.raw).
        Function: Display Edge Map
        Run Code: 
        make
        ./HW2 image_ori.raw out.raw SOBEL ImageHeight ImageWidth Byte_per_pixel
    
    B. Canny Edge Detector:
        File name: main.cpp.
        Input: image (image_ori.jpg). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "CANNY"
        Output: Edge Map (out.jpg).
        Function: Return Edge Map
        Run Code: 
        g++ -o outMHC MHC-Demosaicing.cpp
        ./outMHC image_ori.jpg out.jpg CANNY ImageHeight ImageWidth Byte_per_pixel
    
    C. Structured Edge:
        File name: SE_Detector.m & readraw.m
        Input: image (image_ori.jpg). Byte_per_pixel. ImageHeight. ImageWidth. 
        Output: Edge map
        Function: edge map
        Run Code: 
        Directly run in matlab by click run.
    
    D. Performance Evaluation:
        File name: PE.m
        Input: image (jpg.raw). Byte_per_pixel. ImageHeight. ImageWidth. 
        Output: Performance Evaluation
        Function: calculate the F scores.
        Run Code: 
        Directly run in matlab by click run.

Problem 2:
    A. Random thresholding
        File name: RT.h.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "RT"
        Output: half-tone image(out.raw).
        Function: change image to a printable type.
        Run Code: 
        make
        ./HW2 image_ori.raw out.raw RT ImageHeight ImageWidth Byte_per_pixel
    
    B. Dithering Matrix:
        File name: DM.h.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "DM"
        Output: half-tone image(out.raw).
        Function: change image to a printable type.
        Run Code: 
        make
        ./HW2 image_ori.raw out.raw DM ImageHeight ImageWidth Byte_per_pixel
    
    C. Floyd-Steinberg’s error diffusion:
        File name: errorDiffusion.h.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "FSED"
        Output: half-tone image(out.raw).
        Function: change image to a printable type.
        Run Code: 
        make
        ./HW2 image_ori.raw out.raw FSED ImageHeight ImageWidth Byte_per_pixel

    D. Error diffusion proposed by Jarvis, Judice, and Ninke (JJN):
        File name: errorDiffusion.h.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "JJNED"
        Output: half-tone image(out.raw).
        Function: change image to a printable type.
        Run Code: 
        make
        ./HW2 image_ori.raw out.raw JJNED ImageHeight ImageWidth Byte_per_pixel
    
    E. Error diffusion proposed by Stucki:
        File name: errorDiffusion.h.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "STUCKIED"
        Output: half-tone image(out.raw).
        Function: change image to a printable type.
        Run Code: 
        make
        ./HW2 image_ori.raw out.raw STUCKIED ImageHeight ImageWidth Byte_per_pixel

    F. Separable Error Diffusion:
        File name: separableErrorDiffusion.h
        Input: a color raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "SED"
        Output: half-tone image(out.raw).
        Function: change color image to a printable type.
        Run Code: 
        make
        ./HW2 image_ori.raw out.raw SED ImageHeight ImageWidth Byte_per_pixel

    G. MBVQ-based Error diffusion:
        File name: separableErrorDiffusion.h
        Input: a color raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. Algorithm = "MBVQ"
        Output: half-tone image(out.raw).
        Function: change color image to a printable type.
        Run Code: 
        make
        ./HW2 image_ori.raw out.raw MBVQ ImageHeight ImageWidth Byte_per_pixel
