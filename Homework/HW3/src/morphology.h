// Author: Zifan Wang
/**
 * Parent class of morphology processing
 * Include children class: thining, shrinking, skeletonizing
*/
#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

using namespace std;

class morphology{
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

    int* getOriImage_0_1(){
        int* tmpImage = new int[imageSize_gray];
        // The following code set tmpImage values from [0,1] based on threshold 127 and inputBuffer
        if(BytesPerPixel == 1){        
            for(int i = 0; i < imageSize_gray; i++){
                if(int(inputBuffer[i]) > 127){
                    tmpImage[i] = 1;
                }
                else{
                    tmpImage[i] = 0;
                }
           }
        }
        else{
            unsigned char* grayImage = new unsigned char[imageSize_gray];    // Declare a return image
            grayImage = rgb2gray(inputBuffer,imageHeight,imageWidth,BytesPerPixel);
            for(int i = 0; i < imageSize_gray; i++){
                if(int(grayImage[i]) > 127){
                    tmpImage[i] = 1;
                }
                else{
                    tmpImage[i] = 0;
                }
            }
            delete[] grayImage;
        }
        return tmpImage;
    }

    // morphology operation
    int* morphologyOperation(int* inputImage, int imageHeight, int imageWidth, string* stageOneFilter, string* stageTwoFilter, int lengthOne, int lengthTwo){
        int* tmpImage = new int[imageHeight*imageWidth];
        for(int i = 0; i < imageHeight*imageWidth; i++){
            tmpImage[i] = inputImage[i];
        }
        // Stage one mask
        int stageOne[imageHeight*imageWidth];
        // Stage Two
        int stageTwo[imageHeight*imageWidth];
        // iteration counter 
        int iter = 0;
        // get size of stage one and stage two filter
        int lengthStageOne = lengthOne;
        int lengthStageTwo = lengthTwo;

        while(isUpdate){
            iter++;
            // Stage One
            for(int i = 1; i < imageHeight-1; i++){
                for(int j = 1; j < imageWidth-1; j++){
                    if(tmpImage[i*imageWidth+j] == 0){
                        // if middle pixel is 0 -> stageOne set to 0;
                        stageOne[i*imageWidth+j] = 0;
                    }else{
                        X0 = tmpImage[i*imageWidth+j+1];
                        X1 = tmpImage[(i-1)*imageWidth+j+1];
                        X2 = tmpImage[(i-1)*imageWidth+j];
                        X3 = tmpImage[(i-1)*imageWidth+j-1];
                        X4 = tmpImage[(i)*imageWidth+j-1];
                        X5 = tmpImage[(i+1)*imageWidth+j-1];
                        X6 = tmpImage[(i+1)*imageWidth+j];
                        X7 = tmpImage[(i+1)*imageWidth+j+1];
                        X = tmpImage[i*imageWidth+j];
                        // numBound = 2*(X0+X2+X4+X6) + 1*(X1+X3+X5+X7);
                        // if(numBound == 0 || numBound == 12){
                        //     // Check bounds 
                        //     stageOne[i*imageWidth+j] = 0;
                        // }
                        // else{
                            hit_1 = 0;
                            string tmpString = to_string(X3)+to_string(X2)+to_string(X1)+
                                               to_string(X4)+to_string(X)+to_string(X0)+
                                               to_string(X5)+to_string(X6)+to_string(X7);
                            // Loop through stage one mark
                            for(int m1 = 0; m1 < lengthStageOne; m1++){
                                if(tmpString == stageOneFilter[m1]){
                                    hit_1++;
                                }
                            }
                            if(hit_1 != 0){
                                stageOne[i*imageWidth+j] = 1;
                            }
                            else{
                                stageOne[i*imageWidth+j] = 0;
                            }
                        // }
                    }
                }
            }

            // Stage Two:
            for(int i = 1; i < imageHeight-1; i++){
                for(int j = 1; j < imageWidth-1; j++){
                    if(stageOne[i*imageWidth+j] == 0){
                        stageTwo[i*imageWidth+j] = tmpImage[i*imageWidth+j];
                    }
                    else{
                        string stage2String = "";
                        hit_2 = 0;
                        for(int m2_x = i-1; m2_x <= i+1; m2_x++){
                            for(int m2_j = j-1; m2_j <= j+1; m2_j++){
                                if(stageOne[m2_x*imageWidth+m2_j] == 1){
                                    stage2String.append("1");
                                }else{
                                    stage2String.append("0");
                                }
                            }
                        }
                        for(int m2 = 0; m2 < lengthStageTwo; m2++){
                            if(stage2String == stageTwoFilter[m2]){
                                hit_2++;
                            }
                        }
                        if(hit_2 != 0){
                            stageTwo[i*imageWidth+j] = tmpImage[i*imageWidth+j];
                        }else{
                            stageTwo[i*imageWidth+j] = 0;
                        }
                    }
                }
            }

            // flag Update and tmpImage update
            int difference = 0;
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    if(stageTwo[i*imageWidth+j] != tmpImage[i*imageWidth+j]){
                        difference++;
                    }
                    tmpImage[i*imageWidth+j] = stageTwo[i*imageWidth+j];
                }
            }
            cout << "Difference between preivous and now is: " << difference << endl;
            if(difference != 0){
                isUpdate = true;
            }else{
                isUpdate = false;
            }
            cout << "Finish iteration: " << to_string(iter) << endl;
        }
        return tmpImage;
    }

    /**
     * Set output Image
     */
    void set_0_1_OutputImage(int* tmpImage){
        // Set ouput image
        for(int i = 0; i < imageSize_gray; i++){
            outputBuffer[i] = (unsigned char) tmpImage[i]*255;
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

    /**
     * Return morphology operation filter
     */
    string* getShrinkMark(){
        return shrinkMark;
    }
    string* getThinMark(){
        return thinMark;
    }
    string* getSkeletonizeMark(){
        return skeletonizeMark;
    }
    string* getShrinkThinUncondMark(){
        return shrink_thin_uncond_mark;
    }
    string* getSkeletonizeUncondMark(){
        return SkeletonizeUncondMark;
    }


protected:
    int imageHeight;                    // Height of input image
    int imageWidth;                     // Width of input image
    int BytesPerPixel;                  // number of color channels
    long imageSize_ori;                 // original input image Size
    long imageSize_gray;                // gray scale input image Size
    unsigned char *inputBuffer;         // Input image
    unsigned char *outputBuffer;        // Output image

    // Use in the morphology operation
    // Neighborhood pixels
    int X,X0,X1,X2,X3,X4,X5,X6,X7;

    // Number of Bound;
    int numBound;

    // hit or miss
    int hit_1 = 0;
    int hit_2 = 0;

    // This flag is to see whether image is update from last run
    bool isUpdate = true; // set to true if image change

    // Conditional Mark Logic
    // Shrink filter
    string shrinkMark[58] = { // S-Bound1: 4
                              "001010000","100010000","000010100","000010001",
                              // S-Bound2: 4
                              "000011000","010010000","000110000","000010010",
                              // S-Bound3: 8
                              "001011000","011010000","110010000","100110000","000110100","000010110","000010011","000011001",
                              // STK-Bound4: 4
                              "001011001","111010000","100110100","000010111",
                              // ST-Bound5: 4
                              "110011000","010011001","011110000","001011010",
                              // ST-Bound5: 4
                              "011011000","110110000","000110110","000011011",
                              // ST-Bound6: 2
                              "110011001","011110100",
                              // STK-Bound6: 8
                              "111011000","011011001","111110000","110110100","100110110","000110111","000011111","001011011",
                              // STK-Bound7: 4
                              "111011001","111110100","100110111","001011111",
                              // STK-Bound8: 4
                              "011011011","111111000","110110110","000111111",
                              // STK-Bound9: 8
                              "111011011","011011111","111111100","111111001","111110110","110110111","100111111","001111111",
                              // STK-Bound10: 4
                              "111011111","111111101","111110111","101111111"
                            };
    // Thin Filter
    string thinMark[46] = {"010011000","010110000","000110010","000011010","001011001",
                            "111010000","100110100","000010111","110011000","010011001","011110000","001011010","011011000","110110000",
                            "000110110","000011011","110011001","011110100","111011000","011011001","111110000","110110100","100110110",
                            "000110111","000011111","001011011","111011001","111110100","100110111","001011111","011011011","111111000",
                            "110110110","000111111","111011011","011011111","111111100","111111001","111110110","110110111","100111111",
                            "001111111","111011111","111111101","111110111","101111111" 
                        };
    // skeletonize Filter
    string skeletonizeMark[40] = { 
                              // TK-Bound4: 4
                              "010011000","010110000","000110010","000011010",
                              // STK-Bound4: 4
                              "001011001","111010000","100110100","000010111",
                              // STK-Bound6: 8
                              "111011000","011011001","111110000","110110100","100110110","000110111","000011111","001011011",
                              // STK-Bound7: 4
                              "111011001","111110100","100110111","001011111",
                              // STK-Bound8: 4
                              "011011011","111111000","110110110","000111111",
                              // STK-Bound9: 8
                              "111011011","011011111","111111100","111111001","111110110","110110111","100111111","001111111",
                              // STK-Bound10: 4
                              "111011111","111111101","111110111","101111111",
                              // K-Bound11: 4
                              "111111011","111111110","110111111","011111111"
                            };

    // Unconditional Mark Logic
    // For shrink and thin
    string shrink_thin_uncond_mark[173] = { 
        // Spur 2
        "001010000",
        "100010000",
        // Single 4-connection 2
        "000010010",
        "000011000",
        // L Cluster 8
        "001011000",
        "011010000",
        "110010000",
        "100110000",
        "000110100",
        "000010110",
        "000010011",
        "000011001",
        // 4-connected Offset 4
        "011110000",
        "110011000",
        "010011001",
        "001011010",
        // Spur corner Cluster 12
        "011010100",
        "011011100",
        "001011100",
        "100110001",
        "110110001",
        "110010001",
        "001110100",
        "001110110",
        "001010110",
        "100010011",
        "100011011",
        "100011001",
        // Corner Cluster 1+4+9+10+4+1 = 29
        // 1
        "110110000",
        // 4
        "111110000",
        "110111000",
        "110110100",
        "110110010",
        // 9
        "111111000",
        "111110100",
        "111110010",
        "111110001",
        "110111010",
        "110111001",
        "110110110",
        "110110101",
        "110110011",
        // 10
        "111111100",
        "111111010",
        "111111001",
        "111110110",
        "111110101",
        "111110011",
        "110111110",
        "110111101",
        "110111011",
        "110110111",
        // 4
        "111111110",
        "111111101",
        "111110111",
        "110111111",
        // 1
        "111111111",
        // Tee Branch 24
        "010111000",
        "010111100",
        "011111000",
        "010111001",
        "011111001",
        "000111010",
        "001111010",
        "000111011",
        "001111011",
        "100111010",
        "000111110",
        "100111110",
        "011110010",
        "010110010",
        "010110110",
        "010110011",
        "010110111",
        "010011010",
        "010011110",
        "010011011",
        "010011111",
        "110011010",
        "011011010",
        "111011010",
        // Vee Branch 88
        // 42
        "101010100",
        "101010010",
        "101010001",
        "101010110",
        "101010101",
        "101010011",
        "101010111",
        "111010100",
        "111010010",
        "111010001",
        "111010110",
        "111010101",
        "111010011",
        "111010111",
        "101110100",
        "101110010",
        "101110001",
        "101110110",
        "101110101",
        "101110011",
        "101110111",
        "101011100",
        "101011010",
        "101011001",
        "101011110",
        "101011101",
        "101011011",
        "101011111",
        "111011100",
        "111011001",
        "111011110",
        "111011101",
        "111011011",
        "111011111",
        "101111100",
        "101111010",
        "101111001",
        "101111110",
        "101111101",
        "101111011",
        "101111111",
        "111111011",
         // 18
        "100010101",
        "100011100",
        "100011101",
        "110010101",
        "110011100",
        "110011101",
        "100110101",
        "100111100",
        "100111101",
        "100010111",
        "100011110",
        "100011111",       
        "110111100",
        "110010111",
        "110011110",
        "110011111",
        "100110111",
        "100111111",
         // 22
        "001010101",
        "010010101",
        "011010101",
        "001110101",
        "010110101",
        "011110101",
        "001011101",
        "010011101",
        "011011101",
        "001010111",
        "010010111",
        "011010111",
        "001111101",
        "010111101",
        "011111101",
        "001110111",
        "011110111",
        "001011111",
        "011011111",
        "001111111",
        "010111111",
        "011111111",
         // 6
        "001110001",
        "011110001",
        "001111001",
        "001110011",
        "011110011",
        "011111011",
         // Diagonal Branch 4
        "010011100",
        "010110001",
        "001110010",
        "100011010"
    };
    // For skeletonizing
    string SkeletonizeUncondMark[444]={
                        "000010001","000010100","001010000","100010000","000010010","000011000","000110000","010010000","010011000","010110000","000011010","000110010",
                        "110110000","110110001","110111000","110111001","111110000","111110001","111111000","111111001",
                        "110110010","110110011","110111010","110111011","111110010","111110011","111111010","111111011",
                        "110110100","110110101","110111100","110111101","111110100","111110101","111111100","111111101",
                        "110110110","110110111","110111110","110111111","111110110","111110111","111111110","111111111",
                        "000011011","001011011","010011011","011011011","000011111","001011111","010011111","011011111",
                        "000111011","001111011","010111011","011111011","000111111","001111111","010111111","011111111",
                        "100011011","101011011","110011011","111011011","100011111","101011111","110011111","111011111",
                        "100111011","101111011","110111011","111111011","100111111","101111111","110111111","111111111",
                        "010110010","010110011","010111010","010111011","011110010","011110011","011111010","011111011",
                        "010110110","010110111","010111110","010111111","011110110","011110111","011111110","011111111",
                        "110110010","110110011","110111010","110111011","111110010","111110011","111111010","111111011",
                        "110110110","110110111","110111110","110111111","111110110","111110111","111111110","111111111",
                        "010111000","010111001","011111000","011111001","010111010","010111011","011111010","011111011",
                        "010111100","010111101","011111100","011111101","010111110","010111111","011111110","011111111",
                        "110111000","110111001","111111000","111111001","110111010","110111011","111111010","111111011",
                        "110111100","110111101","111111100","111111101","110111110","110111111","111111110","111111111",
                        "010011010","010011011","011011010","011011011","010011110","010011111","011011110","011011111",
                        "010111010","010111011","011111010","011111011","010111110","010111111","011111110","011111111",
                        "110011010","110011011","111011010","111011011","110011110","110011111","111011110","111011111",
                        "110111010","110111011","111111010","111111011","110111110","110111111","111111110","111111111",
                        "000111010","000111011","001111010","001111011","010111010","010111011","011111010","011111011",
                        "000111110","000111111","001111110","001111111","010111110","010111111","011111110","011111111",
                        "100111010","100111011","101111010","101111011","110111010","110111011","111111010","111111011",
                        "100111110","100111111","101111110","101111111","110111110","110111111","111111110","111111111",
                        "100010101","100011100","101010100","100011101","101010101","101011100","101011101","100010111",
                        "100011110","101010110","100011111","101010111","101011110","101011111","110010101","110011100",
                        "111010100","110011101","111010101","111011100","111011101","110010111","110011110","111010110",
                        "110011111","111010111","111011110","111011111","100110101","100111100","101110100","100111101",
                        "101110101","101111100","101111101","100110111","100111110","101110110","100111111","101110111",
                        "101111110","101111111","110110101","110111100","111110100","110111101","111110101","111111100",
                        "111111101","110110111","110111110","111110110","110111111","111110111","111111110","111111111",
                        "101010100","101010010","101010110","101010001","101010101","101010011","101010111","101011100",
                        "101011010","101011110","101011001","101011101","101011011","101011111","111010100","111010010",
                        "111010110","111010001","111010101","111010011","111010111","111011100","111011010","111011110",
                        "111011001","111011101","111011011","111011111","101110100","101110010","101110110","101110001",
                        "101110101","101110011","101110111","101111100","101111010","101111110","101111001","101111101",
                        "101111011","101111111","111110100","111110010","111110110","111110001","111110101","111110011",
                        "111110111","111111100","111111010","111111110","111111001","111111101","111111011","111111111",
                        "001010101","001110001","001110101","101010001","101010101","101110001","101110101","001011101",
                        "001111001","001111101","101011001","101011101","101111001","101111101","001010111","001110011",
                        "001110111","101010011","101010111","101110011","101110111","001011111","001111011","001111111",
                        "101011011","101011111","101111011","101111111","011010101","011110001","011110101","111010001",
                        "111010101","111110001","111110101","011011101","011111001","011111101","111011001","111011101",
                        "111111001","111111101","011010111","011110011","011110111","111010011","111010111","111110011",
                        "111110111","011011111","011111011","011111111","111011011","111011111","111111011","111111111",
                        "001010101","010010101","011010101","100010101","101010101","110010101","111010101","001011101",
                        "010011101","011011101","100011101","101011101","110011101","111011101","001010111","010010111",
                        "011010111","100010111","101010111","110010111","111010111","001011111","010011111","011011111",
                        "100011111","101011111","110011111","111011111","001110101","010110101","011110101","100110101",
                        "101110101","110110101","111110101","001111101","010111101","011111101","100111101","101111101",
                        "110111101","111111101","001110111","010110111","011110111","100110111","101110111","110110111",
                        "111110111","001111111","010111111","011111111","100111111","101111111","110111111","111111111",
                        "001110010","001110011","101110010","101110011","110010001","111010001","110010101","111010101",
                        "010011100","010011101","110011100","110011101","100011010","101011010","100011110","101011110" 
                    };


    /**
     * This function is to convert input image from rgb to gray scale
     * Arguments: [(unsigned char *inputImage),(int imageHeight), (int imageWidth), (int BytesPerPixel)]
     * Return: unsigned char* grayImage
     */
    unsigned char* rgb2gray(unsigned char *inputImage,int imageHeight, int imageWidth,int BytesPerPixel){
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

};