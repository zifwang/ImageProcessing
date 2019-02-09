#pragma once

#include "image.h"
#include <iostream>
#include <string>

using namespace std;

class sobelEdgeDetector{
public:
    /**
    * Constructor 
    */
    sobelEdgeDetector(image inputImage);
    
    /**
    * This is the function to convert grb image to gray scale
    */
    unsigned char* rgb2gray(unsigned char *inputImage,int imageHeight, int imageWidth, int BytesPerPixel);

    /**
    * This function is to implement the sobel edge detection method 
    */
    void methodSobelEdgeDetector();

    /**
    * Get input image
    */
    unsigned char* getInputImage();

    /**
    * Get output image 
    */
    unsigned char* output();

    /**
    * Return image size(long type)
    */
    long getImageSize();
    long getGrayImageSize();

    /**
    * Return image Height(int type) and width
    */
    int getImageHeight();
    int getImageWidth();

private:
    int imageHeight;                    // Height of input image
    int imageWidth;                     // Width of input image
    int BytesPerPixel;                  // number of color channels
    long imageSize;                     // input image Size
    unsigned char *inputBuffer;         // Input image
    unsigned char *outputBuffer;        // Output image
    // Define the 3*3 sobel mask
    double Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    double Gy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

};