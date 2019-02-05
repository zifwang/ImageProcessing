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
        for(int color = 0; color < BytesPerPixel; color++){
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    tmpImage[i][j][color] = (double)inputBuffer[color*imageHeight*imageWidth+i*imageWidth+j];
                }
            }
        }

        double error;
        // apply the Floyd-Steinberg error diffusion algorithm to quantize each channel separately.
        for(int color = 0; color < BytesPerPixel; color++){
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    // compare to the threshold
                    if(tmpImage[i][j][color] >= 127){
                        error = tmpImage[i][j][color] - 255;
                        imageOut[i][j][color] = (unsigned char) 255;
                    }
                    else{
                        error = tmpImage[i][j][color] - 0;
                        imageOut[i][j][color] = (unsigned char) 0;
                    }
                    // error dist.
                    if (j+1<imageWidth){tmpImage[i][j+1][color]=tmpImage[i][j+1][color]+error*7/16;}
                    if (j-1>=0&&i+1<imageHeight){tmpImage[i+1][j-1][color]=tmpImage[i+1][j-1][color]+error*3/16;}
                    if (i+1<imageHeight){tmpImage[i+1][j][color]=tmpImage[i+1][j][color]+error*5/16;}
                    if (i+1<imageHeight&&j+1<imageWidth){tmpImage[i+1][j+1][color]=tmpImage[i+1][j+1][color]+error*1/16;}
                }
            }
                // else{
                //     for(int j = imageWidth-1; j >= 0; j--){
                //         // compare to the threshold
                //         if(tmpImage[color][i][j] >= threshold){
                //             error = tmpImage[color][i][j] - 255;
                //             imageOut[color][i][j] = (unsigned char) 255;
                //         }
                //         else{
                //             error = tmpImage[color][i][j] - 0;
                //             imageOut[color][i][j] = (unsigned char) 0;
                //         }
                //         // error dist.
                //         if(j > 0) tmpImage[color][i][j-1] = tmpImage[color][i][j-1] + error*7/16;
                //         if(j > 0 && i+1 < imageHeight) tmpImage[color][i+1][j-1] = tmpImage[color][i+1][j-1] + error*1/16;
                //         if(i+1 < imageHeight) tmpImage[color][i+1][j] = tmpImage[color][i+1][j] + error*5/16;
                //         if(j < imageWidth-1 && i+1 < imageHeight) tmpImage[color][i+1][j+1] = tmpImage[color][i+1][j+1]+error*3/16;
                //     }
                // }
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

private:
    double threshold = 127;             // Error diffusion threshold
};