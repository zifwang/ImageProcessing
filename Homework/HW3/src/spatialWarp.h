// Author: Zifan Wang
/**
 * Spatial Warping header
*/

#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>    // std::max
#include <math.h>
#include "image.h"

using namespace std;

class spatialWarp{
public:
    // image set up
    void init_image(image inputImage){
        imageHeight = inputImage.getImageHeight();
        imageWidth = inputImage.getImageWidth();
        BytesPerPixel = inputImage.getBytesPerPixel();
        imageSize_ori = imageHeight*imageWidth*BytesPerPixel;
        imageSize_gray = imageHeight*imageWidth;
        inputBuffer = inputImage.getInputImage();
        // allocate memory to contain the whole file:
        outputBuffer = (unsigned char*) malloc (sizeof(unsigned char)*imageSize_gray);
        if (outputBuffer == NULL) {
            fputs ("Memory error",stderr); 
            exit (EXIT_FAILURE);
        }
    }

    /**
     * Get input image
    */ 
    unsigned char* getiInputImage(){
        return inputBuffer;
    }

    /**
     * Get output image 
     */
    unsigned char* output(){
        return outputBuffer;
    }

    /**
     * Return image color type size(long type) and gray type
     */
    long getImageSize_ori(){
        return imageSize_ori;
    }
    long getImageSize_gray(){
        return imageSize_gray;
    }

    /**
     * Return image Height(int type) and width
     */
    int getImageHeight(){
        return imageHeight;
    }
    int getImageWidth(){
        return imageWidth;
    }

private:
    int imageHeight;                    // Height of input image
    int imageWidth;                     // Width of input image
    int BytesPerPixel;                  // number of color channels
    long imageSize_ori;                 // original input image Size
    long imageSize_gray;                // gray scale input image Size
    unsigned char *inputBuffer;         // Input image
    unsigned char *outputBuffer;        // Output image

    // bilinearInterpolation
    double bilinearInterpolation(double x, double y){
        double pixelVal = 0.0;
        int x_int = (int)x;
        int y_int = (int)y;
        pixelVal = (double)inputBuffer[x_int*imageWidth+y_int]*(y_int+1-y)*(x_int+1-x)
                 + (double)inputBuffer[x_int*imageWidth+y_int+1]*(y-y_int)*(x_int+1-x)
                 + (double)inputBuffer[(x_int+1)*imageWidth+y_int]*(x-x_int)*(y_int+1-y)
                 + (double)inputBuffer[(x_int+1)*imageWidth+y_int+1]*(x-x_int)*(y-y_int);
        return pixelVal;
    }

    // 
};