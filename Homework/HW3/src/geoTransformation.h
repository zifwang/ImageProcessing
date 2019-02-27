// Author: Zifan Wang
/**
 * Geometric Transformation header
*/

#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <utility>
#include <math.h>
#include "image.h"

using namespace std;

/**
 * There are four sub-images: light-house:1-3 
 */


class geoTransformation{
public:
    // image set up
    void init_image(image inputImage);

    /**
     * Set output Image
     */
    void set_0_1_OutputImage(int* tmpImage);

    /**
     * Get input image
    */ 
    unsigned char* getiInputImage();

    /**
     * Get output image 
     */
    unsigned char* output();

    /**
     * Return image color type size(long type) and gray type
     */
    long getImageSize_ori();
    long getImageSize_gray();

    /**
     * Return image Height(int type) and width
     */
    int getImageHeight();
    int getImageWidth();

    /**
     * GEO method
     */
    void methodGEO();

private:
    int imageHeight;                    // Height of input image
    int imageWidth;                     // Width of input image
    int BytesPerPixel;                  // number of color channels
    string filename;                    // file name
    long imageSize_ori;                 // original input image Size
    long imageSize_gray;                // gray scale input image Size
    unsigned char *inputBuffer;         // Input image
    unsigned char *subImage1;           // Input subImage-1
    unsigned char *subImage2;           // Input subImage-2
    unsigned char *subImage3;           // Input subImage-3
    unsigned char *outputBuffer;        // Output image

    /**
     * This function is used to get the sub image size
     */
    unsigned char * readSubImage(string filename, int number);

    /**
     * This function is used to get the sub image's size
     */
    long getSubImageSize(string filename, int number);
    
    /**
     * Padding
     */
    unsigned char* padding(unsigned char* inputImage, int imageHeight, int imageWidth, int paddingSize);


    /**
     * This function is used to find possible corners in the image by haris method
     */
    vector<pair<int,int>> getPossibleCorners(unsigned char* inputImage, int imageHeight, int imageWidth, double RThreshold);

    /**
     * This function is used to find the four corner
     */
    vector<pair<int,int>> limitFourCorners(vector<pair<int,int>> possibleCorners); 

    /**
     * Find 3 holes in the main image
     */
    vector<pair<int,int>> locate3holes(unsigned char* image, vector<pair<int,int>> Corners, int imageHeight, int imageWidth);
};