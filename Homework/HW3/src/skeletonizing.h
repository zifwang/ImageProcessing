// Author: Zifan Wang
/**
 * Implement the skeletonzing algorithm.
 * 
 * 
*/

#pragma once 

#include <string>
#include <iostream>
#include <cstdlib>
#include "morphology.h"
#include "image.h"

using namespace std;

class skeletonzing : public morphology{
public:
    void methodSkeletonzing(){
        // Declare a tmpImage pointer 
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

        cout << "Finish normalize image" << endl;
        
        // Stage one mask
        int stageOne[imageSize_gray];
        // Stage Two
        int stageTwo[imageSize_gray];
        // iteration counter 
        int iter = 0;

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
                        hit_1 = 0;
                        string tmpString = to_string(X3)+to_string(X2)+to_string(X1)+
                                           to_string(X4)+to_string(X)+to_string(X0)+
                                           to_string(X5)+to_string(X6)+to_string(X7);

                            // Loop through stage one mark
                        for(int m1 = 0; m1 < 40; m1++){
                            if(tmpString == skeletonizeMark[m1]){
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
                        for(int m2 = 0; m2 < 444; m2++){
                            if(stage2String == SkeletonizeUncondMark[m2]){
                                hit_2++;
                            }
                        }
                        if(hit_2 != 0){
                            stageTwo[i*imageWidth+j] = tmpImage[i*imageWidth+j];;
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
        // Left bridge operation to do
        


        // Set ouput image
        for(int i = 0; i < imageSize_gray; i++){
            outputBuffer[i] = (unsigned char) tmpImage[i]*255;
        }
    }





private:
    // Neighborhood pixels
    int X,X0,X1,X2,X3,X4,X5,X6,X7;

    // Number of Bound;
    int numBound;

    // hit or miss
    int hit_1 = 0;
    int hit_2 = 0;

    // This flag is to see whether image is update from last run
    bool isUpdate = true; // set to true if image change

    // Conditional Look-Up-Table
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

    // Unconditional Look-Up-Table
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