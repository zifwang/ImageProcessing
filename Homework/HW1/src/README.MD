# EE569 Homework Assignment #1
# Date: January 22, 2018
# Name: Zifan Wang
# ID: 9505-5872-96
# email: zifanw@usc.edu
#
# Compiled on macOS Mojave Version10.14.2
Lauguage: c++
Complier: g++

Problem 1:
    A. Bilinear Demosaicing:
        File name: bilinearDemosaicing.cpp.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. 
        Output: A color image (out_BD.raw).
        Function: Change gray scale image to color image.
        Run Code: 
        g++ -o outBD bilinearDemosaicing.cpp
        ./outBD image_ori.raw out_BD.raw Byte_per_pixel ImageHeight ImageWidth
    
    B. MHC Demosaicing:
        File name: MHC-Demosaicing.cpp.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. 
        Output: A color image (out_MHC.raw).
        Function: Change gray scale image to color image.
        Run Code: 
        g++ -o outMHC MHC-Demosaicing.cpp
        ./outMHC image_ori.raw out_MHC.raw Byte_per_pixel ImageHeight ImageWidth
    
    C. Transfer-function Based Histogram Equalization:
        File name: tfbHistEqu.cpp.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. 
        Output: Contrasted image (out_tfbHEBD.raw).
        Function: Change gray scale image to image with contrast adjustment.
        Run Code: 
        g++ -o outTFBHE tfbHistEqu.cpp
        ./outTFBHE image_ori.raw out_tfbHE.raw Byte_per_pixel ImageHeight ImageWidth
    
    D. Cumulative Probability Based Histogram Equalization:
        File name: cpbHistEqu.cpp.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. 
        Output: Contrasted image (out_cpbHE.raw).
        Function: Change gray scale image to image with contrast adjustment.
        Run Code: 
        g++ -o outCPBHE cpbHistEqu.cpp
        ./outCPBHE image_ori.raw out_cpbHE.raw Byte_per_pixel ImageHeight ImageWidth

Problem 2:
    A. Uniform Weight Filter
        File name: uniformWeightFilter.cpp.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. FilterHeight. 
        Output: noisy free image (out_UWF.raw).
        Function: Change noisy gray scale image to noise-free image.
        Run Code: 
        g++ -o outUWF uniformWeightFilter.cpp
        ./outUWF image_ori.raw out_UWF.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight
    
    B. Gaussian Filter:
        File name: gaussianFilter.cpp.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. FilterHeight. STD.
        Output: noisy free image (out_GF.raw).
        Function: Change noisy gray scale image to noise-free image.
        Run Code: 
        g++ -o outGF gaussianFilter.cpp
        ./outGF image_ori.raw out_GF.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight STD
    
    C. Bilateral Filter:
        File name: bilaterFilter.cpp.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. FilterHeight. STD. sigma_c. sigma_s.
        Output: noisy free image (out_BF.raw).
        Function: Change noisy gray scale image to noise-free image.
        Run Code: 
        g++ -o outBF bilaterFilter.cpp
        ./outBF image_ori.raw out_BF.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight STD sigma_c sigma_s

    D. Nonlocal Mean Filter:
        File name: nonLocalMeanFilter.cpp.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. FilterHeight. STD. sigma_c. sigma_s. SearchingWindow. FP.
        Output: noisy free image (out_NLM.raw).
        Function: Change noisy gray scale image to noise-free image.
        Run Code: 
        g++ -o outNLM nonLocalMeanFilter.cpp
        ./outNLM image_ori.raw out_NLM.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight STD sigma_c sigma_s SearchingWindow FP
    
    E. Median Filter:
        File name: medianFilter.cpp.
        Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. FilterHeight. 
        Output: noisy free image (out_MF.raw).
        Function: Change noisy gray scale image to noise-free image.
        Run Code: 
        g++ -o outMF medianFilter.cpp
        ./outMF image_ori.raw out_MF.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight

    F. PSNR Calculation:
        File name: psnr.cpp.
        Input: a gray-scale raw image (image_ori.raw). noisy image (image_mod.raw) 
        Output: outputPSNR.txt
        Function: Calculat PSNR value.
        Run Code: 
        g++ -o outPSNR psnr.cpp
        ./outPSNR image_ori.raw image_mod.raw 

    G. Color Image Denoising:
        Function: Denoise color image.
        Input: image_ori.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight STD sigma_c sigma_s
        Output: out_MF.raw and out_BF.raw
        Run code: (Apply median filter first then bilateral filter, and feel free to change order)
        Order One:
            g++ -o outMF medianFilter.cpp
            ./outMF image_ori.raw out_MF.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight
            g++ -o outBF bilaterFilter.cpp
            ./outBF out_MF.raw out_BF.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight STD sigma_c sigma_s
        Order Two: 
            g++ -o outBF bilaterFilter.cpp
            ./outBF image_ori.raw out_BF.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight STD sigma_c sigma_s
            g++ -o outMF medianFilter.cpp
            ./outMF out_BF.raw out_MF.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight

    H. Short Noise:
        1. Gaussian Filter: 
            File name: gaussianFilter.cpp.
            Input: a gray-scale raw image (image_ori.raw). Byte_per_pixel. ImageHeight. ImageWidth. FilterHeight. STD.
            Output: noisy free image (out_GF.raw).
            Function: Change noisy gray scale image to noise-free image.
            Run Code: 
            g++ -o outGF gaussianFilter.cpp
            ./outGF image_ori.raw out_GF.raw Byte_per_pixel ImageHeight ImageWidth FilterHeight STD
        
        2. BM3D Filter:
            File name: BM3D.m (This is a matlab file run it in matlab)
            INPUT ARGUMENTS (OPTIONAL):
                1) y (matrix M x N): Noise-free image (needed for computing PSNR), replace with the scalar 1 if not available.
                2) z (matrix M x N): Noisy image (intensities in range [0,1] or [0,255])
                3) sigma (double)  : Std. dev. of the noise (corresponding to intensities in range [0,255] even if the range of z is [0,1])
                4) profile (char)  : 'np' --> Normal Profile 
                                     'lc' --> Fast Profile
                5) print_to_screen : 0 --> do not print output information (and do not plot figures)
                                     1 --> print information and plot figures
            OUTPUTS:
                1) PSNR (double)   : Output PSNR (dB), only if the original image is available, otherwise PSNR = 0                                               
                2) y_est (matrix M x N): Final estimate (in the range [0,1])
            Run code. [PSNR,y_est] = BM3D(y,z,sigma,profile,print_to_screen);

            File name: readraw.m (This is a matlab file run it in matlab)
            INPUT ARGUMENTS: raw image (image_ori.raw)
            OUTPUT: a 2-D array (image_array)
            Run Code: image_array = readraw(image_ori.raw);

            File name: writeraw.m (This is a matlab file run it in matlab)
            INPUT ARGUMENTS: a 2-D array (image_array)
            OUTPUT: raw image (image_ori.raw)
            Run Code: writeraw(G, 'image_ori.raw');