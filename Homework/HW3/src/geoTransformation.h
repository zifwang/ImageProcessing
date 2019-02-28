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

    /**
    * Image rotation
    */  
    double* bilinearInterpolation_rotation(double* img, int imgHeight, int imgWidth);
    double* rotated_image_unsigned(vector<pair<int,int>> corners, unsigned char* subImage, int subImgHeight, int subImgWidth, double angle);
    double* rotated_image_90(vector<pair<int,int>> corners, double* subImage, int subImgHeight, int subImgWidth);
    
    /**
     * Image Scaling
     */
    double* scaling_image_doubled(vector<pair<int,int>> &corners, double* subImage, int subImgHeight, int subImgWidth, int targetSize);

    /**
    * Corner coordinates update
    */  
    vector<pair<int,int>> update_corners(vector<pair<int,int>> corners,double angle);
    vector<pair<int,int>> update_corners_90(vector<pair<int,int>> corners);

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

    int width_finder(vector<pair<int,int>> corners);
    int height_finder(vector<pair<int,int>> corners);
    int find_x_min(vector<pair<int,int>> corners);
    int find_y_min(vector<pair<int,int>> corners);
    int mod_sub_image_width(vector<pair<int,int>> corners, int subImgHeight, int subImgWidth, int targetSize){
        int height_min = subImgHeight;
        int height_max = 0;
        int width_min = subImgWidth;
        int width_max = 0;
        for(int i = 0; i < corners.size(); i++){
            // cout << "ORI: " << corners[i].first << " " << corners[i].second << endl;
            if(corners[i].second > height_max){
                height_max = corners[i].second;
            }
            if(corners[i].second < height_min){
                height_min = corners[i].second;
            }
            if(corners[i].first > width_max){
                width_max = corners[i].first;
            }
            if(corners[i].first < width_min){
                width_min = corners[i].first;
            }
        }
        int sub_imageWidth = width_max - width_min;
        int sub_imageHeight = height_max - height_min;
        // scaling parameter
        double scale = double(targetSize)/sub_imageHeight;
        // double ratio = double(1)/scale;
        return int(subImgWidth*scale);
    }
    int mod_sub_image_height(vector<pair<int,int>> corners, int subImgHeight, int subImgWidth, int targetSize){
        int height_min = subImgHeight;
        int height_max = 0;
        int width_min = subImgWidth;
        int width_max = 0;
        for(int i = 0; i < corners.size(); i++){
            // cout << "ORI: " << corners[i].first << " " << corners[i].second << endl;
            if(corners[i].second > height_max){
                height_max = corners[i].second;
            }
            if(corners[i].second < height_min){
                height_min = corners[i].second;
            }
            if(corners[i].first > width_max){
                width_max = corners[i].first;
            }
            if(corners[i].first < width_min){
                width_min = corners[i].first;
            }
        }
        int sub_imageWidth = width_max - width_min;
        int sub_imageHeight = height_max - height_min;
        // scaling parameter
        double scale = double(targetSize)/sub_imageHeight;
        // double ratio = double(1)/scale;
        return int(subImgHeight*scale);
    }

};