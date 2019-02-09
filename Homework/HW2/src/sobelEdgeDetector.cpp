#include "sobelEdgeDetector.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

using namespace std;

// Constructor: init. all private variable
sobelEdgeDetector::sobelEdgeDetector(image inputImage){
    imageHeight = inputImage.getImageHeight();
    imageWidth = inputImage.getImageWidth();
    BytesPerPixel = inputImage.getBytesPerPixel();
    imageSize = imageHeight*imageWidth*BytesPerPixel;
    inputBuffer = inputImage.getInputImage();
    // allocate memory to contain the whole file:
    outputBuffer = (unsigned char*) malloc (sizeof(unsigned char)*imageHeight*imageWidth*BytesPerPixel);
    if (outputBuffer == NULL) {
        fputs ("Memory error",stderr); 
        exit (EXIT_FAILURE);
    }
}

/**
* This is the function to convert grb image to gray scale
*/
unsigned char* sobelEdgeDetector::rgb2gray(unsigned char *inputImage,int imageHeight, int imageWidth,int BytesPerPixel){
    unsigned char* grayImage;    // Declare a return image
    grayImage = (unsigned char*) malloc (sizeof(unsigned char)*imageHeight*imageWidth);

    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            // rgb2gray converts RGB values to grayscale values by forming a weighted sum of the R, G, and B components:
            // 0.2989 * R + 0.5870 * G + 0.1140 * B 
            double tmp = 0.2989*double(inputImage[i*BytesPerPixel*imageWidth+j*BytesPerPixel+0])+
                            0.5870*double(inputImage[i*BytesPerPixel*imageWidth+j*BytesPerPixel+1])+
                            0.1140*double(inputImage[i*BytesPerPixel*imageWidth+j*BytesPerPixel+2]);
            // cout << tmp << endl;
            if(tmp > 255) grayImage[i*imageWidth+j] = (unsigned char) 255;
            else if(tmp < 0) grayImage[i*imageWidth+j] = (unsigned char) 0;
            else grayImage[i*imageWidth+j] = (unsigned char) tmp;
        }
    }
    return grayImage;
}

/**
* This function is to implement the sobel edge detection method 
*/
void sobelEdgeDetector::methodSobelEdgeDetector(){
    unsigned char* tmp = rgb2gray(inputBuffer,imageHeight,imageWidth,BytesPerPixel);
    

    double tmpImage2D[imageHeight][imageWidth];
    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            tmpImage2D[i][j] = double(tmp[i*imageWidth+j]);
        }
    }

    double tmpExtendedImage[imageHeight+3-1][imageWidth];
    double extendedImage[imageHeight+3-1][imageWidth+3-1];
    
    // Image extension
    for(int i = 0; i < imageHeight+3-1; i++){
        for(int j = 0; j < imageWidth; j++){
            if(i < 3/2){
                int actHeight = 3/2 - i - 1;
                tmpExtendedImage[i][j] = tmpImage2D[actHeight][j];
            }
            else if(i > imageHeight+3/2-1){
                int actHeight = imageHeight - (i-imageHeight) + 3/2-1;
                tmpExtendedImage[i][j] = tmpImage2D[actHeight][j];
            }
            else tmpExtendedImage[i][j] = tmpImage2D[i-3/2][j];
        }
    }
    for(int i = 0; i < imageHeight+3-1; i++){
        for(int j = 0; j < imageWidth+3-1; j++){
            if(j < 3/2){
                int actWidth = 3/2 - j -1;
                extendedImage[i][j] = tmpExtendedImage[i][actWidth];
            }
            else if(j > imageWidth+3/2-1){
                int actWidth = imageWidth - (j-imageWidth) + (3/2-1);
                extendedImage[i][j] = tmpExtendedImage[i][actWidth];
            }
            else extendedImage[i][j] = tmpExtendedImage[i][j-3/2];
            
        }
    }
    // Remove tmp pointer
    delete[] tmp;

    
    // double gxImage[imageHeight][imageWidth];
    // double gyImage[imageHeight][imageWidth];
    double gradiantMap[imageHeight][imageWidth];
    double maxGD = -300, minGD = 300;
    // double maxX = -300, minX = 300, maxY = -300, minY = 300, maxGD = -300, minGD = 300;
    // Apply Gx & Gy mask
    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            double sumX = 0;
            double sumY = 0;
            for(int fh = -1; fh <= 1; fh++){
                for(int fw = -1; fw <= 1; fw++){
                    sumX = sumX + extendedImage[i-3/2+fh][j-3/2+fw]*Gx[fh+1][fw+1];
                    sumY = sumY + extendedImage[i-3/2+fh][j-3/2+fw]*Gy[fh+1][fw+1];
                }
            }
            sumX = sumX/4;
            sumY = sumY/4;
            double gd = sqrt(pow(sumX,2)+pow(sumY,2));
            if(gd > maxGD) maxGD = gd;
            if(gd < minGD) minGD = gd;
            // if(sumX > maxX) maxX = sumX;
            // if(sumX < minX) minX = sumX;
            // if(sumY > maxY) maxY = sumY;
            // if(sumY < minY) minY = sumY;
            
            // gxImage[i][j] = sumX;
            // gyImage[i][j] = sumY;
            gradiantMap[i][j] = gd;
        }
    }


    // normalize
    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            gradiantMap[i][j] = 255*(gradiantMap[i][j]-minGD)/(maxGD-minGD);
        }
    }

    // Apply thresholding 
    int height[imageHeight*imageWidth];   // first row height, second row width
    int width[imageHeight*imageWidth]; 
    double threshold = 0.075;
    int boundary = int((1-threshold)*imageHeight*imageWidth);
    // cout << boundary << endl;
    int position = 0;
    for(int k = 0; k < 256; k++){
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                if(int(gradiantMap[i][j]) == k){
                    height[position] = i;
                    width[position] = j;
                    position++;
                }
            }
        }
    }
    unsigned char imageOut[imageHeight][imageWidth];
    for(int i = 0; i < boundary; i++){
        // background
        imageOut[height[i]][width[i]] = (unsigned char)255;
    }
    for(int i = boundary; i < imageHeight*imageWidth; i++){
        // edge
        imageOut[height[i]][width[i]] = (unsigned char)0;
    }
    

    // set outputBuffer
    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            outputBuffer[i*imageWidth+j] = (unsigned char)imageOut[i][j];
        }
    }
}


/**
* Get input image
*/
unsigned char* sobelEdgeDetector::getInputImage(){
    return inputBuffer;
}

/**
* Get output image 
*/
unsigned char* sobelEdgeDetector::output(){
    return outputBuffer;
}

/**
* Return gray image size(long type)
*/
long sobelEdgeDetector::getImageSize(){
    return imageHeight*imageWidth;
}

/**
* Return image size(long type)
*/
long sobelEdgeDetector::getGrayImageSize(){
    return imageSize;
}

/**
     * Return image Height(int type) and width
     */
int sobelEdgeDetector::getImageHeight(){
    return imageHeight;
}
int sobelEdgeDetector::getImageWidth(){
    return imageWidth;
}