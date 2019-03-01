// Author: Zifan Wang
/**
 * lensDistortion header
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

class lensUndistortion{
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

    // lens distortion correction
    void lens_distortion_correction(double fx, double fy){
        double corrected_image[imageHeight][imageWidth];
        double center_u = imageHeight/2;
        double center_v = imageWidth/2;
        double R = max(center_u,center_v);
        
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                double x_u_c = (double)i - center_u;
                double y_u_c = (double)j - center_v;
                pair<double,double> oldPoint = points_old(x_u_c, y_u_c, R, fx, fy);
                double x_d = oldPoint.first;
                double y_d = oldPoint.second;
                if(x_d+center_u < 0 || x_d+center_u > imageHeight-1 || y_d+center_v < 0 || y_d+center_v > imageWidth-1){
                    continue;
                }
                corrected_image[i][j] = bilinearInterpolation(x_d+center_u,y_d+center_v);
            }
        }

        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                outputBuffer[i*imageWidth+j] = (unsigned char)corrected_image[i][j];
            }
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

    // Distortion coeff:
    double k_1 = -0.3536;
    double k_2 = 0.1730;
    double k_3 = 0;

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

    // Transformation of points
    pair<double,double> points_old(double x, double y, double R, double fx, double fy){
        // Implement the relationship between the actual image and its distortion in the camera coordinate system
        // x_d = x(1 + k_1 * r^2 + k_2 * r_4 + k_3 * r_6 )
        // y_d = y(1 + k_1 * r^2 + k_2 * r_4 + k_3 * r_6 )
        // r^2 = x^2 + y^2
        // Argument: point(x,y) -> undistorted pixel location
        // Return: x_d, y_d: undistorted pixel location
        double f_min = 0;
        double f_max = 0;
        if(fx >= fy){
            f_min = fy;
            f_max = fx;
        }
        else{
            f_min = fx;
            f_max = fy;
        }
        double r = sqrt(x*x+y*y)/f_min;
        R = R/f_max;
        
        double x_old = x*(1 + k_1*pow(r,2) + k_2*pow(r,4) + k_3*pow(r,6))/(1 + k_1*pow(R,2) + k_2*pow(R,4) + k_3*pow(R,6));
        double y_old = y*(1 + k_1*pow(r,2) + k_2*pow(r,4) + k_3*pow(r,6))/(1 + k_1*pow(R,2) + k_2*pow(R,4) + k_3*pow(R,6));
        return make_pair(x_old,y_old);
    }

};