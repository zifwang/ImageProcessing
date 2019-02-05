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
        unsigned char imageOut[BytesPerPixel][imageHeight][imageWidth];

        double tmpImage[BytesPerPixel][imageHeight][imageWidth];
        for(int color = 0; color < BytesPerPixel; color++){
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    tmpImage[color][imageHeight][imageWidth] = double(inputBuffer[color*imageHeight*imageWidth+i*imageWidth+j]);
                }
            }
        }

        // apply the Floyd-Steinberg error diffusion algorithm to quantize each channel separately.
        for(int color = 0; color < BytesPerPixel; color++){
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    
                }
            }
        }









        // set outputBuffer
        for(int color = 0; color < BytesPerPixel; color++){
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    outputBuffer[color*imageHeight*imageWidth+i*imageWidth+j] = imageOut[i][j][color];
                }
            }
        }

    }
}