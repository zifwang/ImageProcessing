#pragma once 

#include "image.h"
#include "half_tonning_algorithm.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>


using namespace std;

class separableErrorDiffusion : public half_tonning_algorithm{
public:
    void methodSeparableErrorDiffusion(){
        unsigned char imageOut[imageHeight][imageWidth][BytesPerPixel];

        double tmpImage[imageHeight][imageWidth][BytesPerPixel];
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                for(int color = 0; color < BytesPerPixel; color++){
                    /**
                    * Read image by oneD array: first by row, then col, last color
                    * in the format: Row: 0, Col: 0, Color: 0 <- first element in 1D array
                    *                Row: 0, Col: 0, Color: 1 <- Second element
                    *                Row: 0, Col: 0, Color: 2 <- third element
                    *                Row: 0, Col: 1, Color: 0 <- fourth element
                    *                Row: 0, Col: 1, Color: 1 <- fifth element
                    *                Row: 0, Col: 1, Color: 2 <- sixth element
                    */
                    tmpImage[i][j][color] = 1 - double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color])/255;
                }
            }
        }

        double error;
        // apply the Floyd-Steinberg error diffusion algorithm to quantize each channel separately.
        for(int color = 0; color < BytesPerPixel; color++){
            for(int i = 0; i < imageHeight; i++){
                if(i%2 == 0){
                    for(int j = 0; j < imageWidth; j++){
                        // compare to the threshold
                        if(tmpImage[i][j][color] >= threshold){
                            error = tmpImage[i][j][color] - 1;
                            imageOut[i][j][color] = (unsigned char) 0;
                        }
                        else{
                            error = tmpImage[i][j][color] - 0;
                            imageOut[i][j][color] = (unsigned char) 255;
                        }
                        // error dist.
                        if (j < imageWidth-1) tmpImage[i][j+1][color] = tmpImage[i][j+1][color] + error*7.0/16;
                        if (j > 0 && i < imageHeight-1) tmpImage[i+1][j-1][color] = tmpImage[i+1][j-1][color] + error*3.0/16;
                        if (i <imageHeight-1) tmpImage[i+1][j][color] = tmpImage[i+1][j][color] + error*5.0/16;
                        if (j < imageWidth-1 && i < imageHeight-1) tmpImage[i+1][j+1][color] = tmpImage[i+1][j+1][color] + error*1.0/16;
                    }
                }
                else{
                    for(int j = imageWidth-1; j >= 0; j--){
                        // compare to the threshold
                        if(tmpImage[i][j][color] >= threshold){
                            error = tmpImage[i][j][color] - 1;
                            imageOut[i][j][color] = (unsigned char) 0;
                        }
                        else{
                            error = tmpImage[i][j][color] - 0;
                            imageOut[i][j][color] = (unsigned char) 255;
                        }
                        // error dist.
                        if(j > 0) tmpImage[i][j-1][color] = tmpImage[i][j-1][color] + error*7/16;
                        if(j > 0 && i+1 < imageHeight) tmpImage[i+1][j-1][color] = tmpImage[i+1][j-1][color] + error*1/16;
                        if(i+1 < imageHeight) tmpImage[i+1][j][color] = tmpImage[i+1][j][color] + error*5/16;
                        if(j < imageWidth-1 && i+1 < imageHeight) tmpImage[i+1][j+1][color] = tmpImage[i+1][j+1][color] + error*3/16;
                    }
                }
            }
        }

        // set outputBuffer
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                for(int color = 0; color < BytesPerPixel; color++){
                    outputBuffer[i*BytesPerPixel*imageWidth+j*BytesPerPixel+color] = imageOut[i][j][color];
                }
            }
        }

    }

private:
    double threshold = 0.5;             // Error diffusion threshold
};