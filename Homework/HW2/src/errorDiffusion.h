#pragma once

#include "image.h"
#include "half_tonning_algorithm.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>

class errorDiffusion : public half_tonning_algorithm{
public:
    void methodErrorDiffusion(string algorithm){

        unsigned char imageOut[imageHeight][imageWidth];

        double tmp[imageHeight][imageWidth];
        // Change input image type
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                tmp[i][j] = (double)input2DImage[i][j];
            }
        }

        if(algorithm == "steinberg"){
            double error;
            for(int i = 0; i < imageHeight; i++){
                if(i%2 == 0){
                    for(int j = 0; j < imageWidth; j++){
                        // compare to the threshold
                        if(tmp[i][j]>=threshold){
                            error = tmp[i][j]-255;
                            imageOut[i][j] = (unsigned char) 255;
                        }
                        else{
                            error = tmp[i][j]-0;
                            imageOut[i][j] = (unsigned char) 0;
                        }
                        // error dist.
                        if(j+1 < imageWidth) tmp[i][j+1] = tmp[i][j+1] + error*7/16;
                        if(j-1 >= 0 && i+1 < imageHeight) tmp[i+1][j-1] = tmp[i+1][j-1] + error*3/16;
                        if(i+1 < imageHeight) tmp[i+1][j] = tmp[i+1][j] + error*5/16;
                        if(i+1 < imageHeight && j+1 < imageWidth) tmp[i+1][j+1] = tmp[i+1][j+1] + error*1/16;
                    }
                }
                else{
                    for(int j = imageWidth-1; j >= 0; j--){
                        // compare to the threshold
                        if(tmp[i][j]>=threshold){
                            error = tmp[i][j]-255;
                            imageOut[i][j] = (unsigned char) 255;
                        }
                        else{
                            error = tmp[i][j]-0;
                            imageOut[i][j] = (unsigned char) 0;
                        }
                        // error dist.
                        if(j > 0) tmp[i][j-1] = tmp[i][j-1] + error*7/16;
                        if(j > 0 && i+1 < imageHeight) tmp[i+1][j-1] = tmp[i+1][j-1] + error*1/16;
                        if(i+1 < imageHeight) tmp[i+1][j] = tmp[i+1][j] + error*5/16;
                        if(j < imageWidth-1 && i+1 < imageHeight) tmp[i+1][j+1] = tmp[i+1][j+1]+error*3/16;
                    }
                }
            }
        }
        else if(algorithm == "JJN"){
            double error = 0;
            for(int i = 0; i < imageHeight; i++){
                if(i%2 == 0){
                    for(int j = 0; j < imageWidth; j++){
                        if(tmp[i][j] >= threshold) {
                            error = tmp[i][j] - 255;
                            imageOut[i][j] = (unsigned char)255;
                        }else{
                            error = tmp[i][j];
                            imageOut[i][j] = (unsigned char)0;
                        }
                        // error dist.
                        if(j < imageWidth-1) tmp[i][j+1] = tmp[i][j+1] + error * 7/48;
                        if(j < imageWidth-2) tmp[i][j+2] = tmp[i][j+2] + error * 5/48;
                        if(j > 1 && i < imageHeight-1) tmp[i+1][j-2] = tmp[i+1][j-2] + error * 3/48;
                        if(j > 0 && i < imageHeight-1) tmp[i+1][j-1] = tmp[i+1][j-1] + error * 5/48;
                        if(i < imageHeight-1) tmp[i+1][j+0] = tmp[i+1][j+0] + error * 7/48;
                        if(j < imageWidth-1 && i < imageHeight-1) tmp[i+1][j+1] = tmp[i+1][j+1] + error * 5/48;
                        if(j < imageWidth-2 && i < imageHeight-1) tmp[i+1][j+2] = tmp[i+1][j+2] + error * 3/48;
                        if(j > 1 && i < imageHeight-2) tmp[i+2][j-2] = tmp[i+2][j-2] + error * 1/48;
                        if(j > 0 && i < imageHeight-2) tmp[i+2][j-1] = tmp[i+2][j-1] + error * 3/48;
                        if(i < imageHeight-2) tmp[i+2][j+0] = tmp[i+2][j+0] + error * 5/48;
                        if(j < imageWidth-1 && i < imageHeight-2) tmp[i+2][j+1] = tmp[i+2][j+1] + error * 3/48;
                        if(j < imageWidth-2 && i < imageHeight-2) tmp[i+2][j+2] = tmp[i+2][j+2] + error * 1/48;
                    }
                }
                else{
                    for(int j = imageWidth-1; j >= 0; j--){
                        if(tmp[i][j] >= threshold) {
                            error = tmp[i][j] - 255;
                            imageOut[i][j] = (unsigned char)255;
                        }else{
                            error = tmp[i][j];
                            imageOut[i][j] = (unsigned char)0;
                        }
                        // error dist.
                        if(j > 0) tmp[i][j-1] = tmp[i][j-1] + error * 7/48;
                        if(j > 1) tmp[i][j-2] = tmp[i][j-2] + error * 5/48;
                        if(j > 1 && i < imageHeight-1) tmp[i+1][j-2] = tmp[i+1][j-2] + error * 3/48;
                        if(j > 0 && i < imageHeight-1) tmp[i+1][j-1] = tmp[i+1][j-1] + error * 5/48;
                        if(i < imageHeight-1) tmp[i+1][j+0] = tmp[i+1][j+0] + error * 7/48;
                        if(j < imageWidth-1 && i < imageHeight-1) tmp[i+1][j+1] = tmp[i+1][j+1] + error * 5/48;
                        if(j < imageWidth-2 && i < imageHeight-1)tmp[i+1][j+2] = tmp[i+1][j+2] + error * 3/48;
                        if(j > 1 && i < imageHeight-2) tmp[i+2][j-2] = tmp[i+2][j-2] + error * 1/48;
                        if(j > 0 && i < imageHeight-2) tmp[i+2][j-1] = tmp[i+2][j-1] + error * 3/48;
                        if(i < imageHeight-2) tmp[i+2][j+0] = tmp[i+2][j+0] + error * 1/48;
                        if(j < imageWidth-1 && i < imageHeight-2) tmp[i+2][j+1] = tmp[i+2][j+1] + error * 3/48;
                        if(j < imageWidth-2 && i < imageHeight-2) tmp[i+2][j+2] = tmp[i+2][j+2] + error * 1/48;
                    }
                }
            }
        }
        else if(algorithm == "Stucki"){
            double error = 0;
            for(int i = 0; i < imageHeight; i++){
                if(i%2 == 0){
                    for(int j = 0; j < imageWidth; j++){
                        if(tmp[i][j] >= threshold) {
                            error = tmp[i][j] - 255;
                            imageOut[i][j] = (unsigned char)255;
                        }else{
                            error = tmp[i][j];
                            imageOut[i][j] = (unsigned char)0;
                        }
                        // error dist.
                        if(j < imageWidth-1) tmp[i][j+1] = tmp[i][j+1] + error * 8/42;
                        if(j < imageWidth-2) tmp[i][j+2] = tmp[i][j+2] + error * 4/42;
                        if(j > 1 && i < imageHeight-1) tmp[i+1][j-2] = tmp[i+1][j-2] + error * 2/42;
                        if(j > 0 && i < imageHeight-1) tmp[i+1][j-1] = tmp[i+1][j-1] + error * 4/42;
                        if(i < imageHeight-1) tmp[i+1][j+0] = tmp[i+1][j+0] + error * 8/42;
                        if(j < imageWidth-1 && i < imageHeight-1) tmp[i+1][j+1] = tmp[i+1][j+1] + error * 4/42;
                        if(j < imageWidth-2 && i < imageHeight-1) tmp[i+1][j+2] = tmp[i+1][j+2] + error * 2/42;
                        if(j > 1 && i < imageHeight-2) tmp[i+2][j-2] = tmp[i+2][j-2] + error * 1/42;
                        if(j > 0 && i < imageHeight-2) tmp[i+2][j-1] = tmp[i+2][j-1] + error * 2/42;
                        if(i < imageHeight-2) tmp[i+2][j+0] = tmp[i+2][j+0] + error * 4/42;
                        if(j < imageWidth-1 && i < imageHeight-2) tmp[i+2][j+1] = tmp[i+2][j+1] + error * 2/42;
                        if(j < imageWidth-2 && i < imageHeight-2) tmp[i+2][j+2] = tmp[i+2][j+2] + error * 1/42;
                    }
                }
                else{
                    for(int j = imageWidth-1; j >= 0; j--){
                        if(tmp[i][j] >= threshold) {
                            error = tmp[i][j] - 255;
                            imageOut[i][j] = (unsigned char)255;
                        }else{
                            error = tmp[i][j];
                            imageOut[i][j] = (unsigned char)0;
                        }
                        // error dist.
                        if(j > 0) tmp[i][j-1] = tmp[i][j-1] + error * 8/42;
                        if(j > 1) tmp[i][j-2] = tmp[i][j-2] + error * 4/42;
                        if(j > 1 && i < imageHeight-1) tmp[i+1][j-2] = tmp[i+1][j-2] + error * 2/42;
                        if(j > 0 && i < imageHeight-1) tmp[i+1][j-1] = tmp[i+1][j-1] + error * 4/42;
                        if(i < imageHeight-1) tmp[i+1][j+0] = tmp[i+1][j+0] + error * 8/42;
                        if(j < imageWidth-1 && i < imageHeight-1) tmp[i+1][j+1] = tmp[i+1][j+1] + error * 4/42;
                        if(j < imageWidth-2 && i < imageHeight-1)tmp[i+1][j+2] = tmp[i+1][j+2] + error * 2/42;
                        if(j > 1 && i < imageHeight-2) tmp[i+2][j-2] = tmp[i+2][j-2] + error * 1/42;
                        if(j > 0 && i < imageHeight-2) tmp[i+2][j-1] = tmp[i+2][j-1] + error * 2/42;
                        if(i < imageHeight-2) tmp[i+2][j+0] = tmp[i+2][j+0] + error * 4/42;
                        if(j < imageWidth-1 && i < imageHeight-2) tmp[i+2][j+1] = tmp[i+2][j+1] + error * 2/42;
                        if(j < imageWidth-2 && i < imageHeight-2) tmp[i+2][j+2] = tmp[i+2][j+2] + error * 1/42;
                    }
                }
            }
        }
        else{
            cout << "Error inputing error diffusion algorithm." << endl;
            exit(EXIT_FAILURE);
        }


        // set outputBuffer
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                outputBuffer[i*imageWidth+j] = imageOut[i][j];
            }
        }
}



private:
    double threshold = 127;    // Error diffusion threshold

    // Floyd-Steinberg’s error diffusion with the serpentine scanning
    double STEINBERG[3][3] = {0,0,0,0,0,7/16,3/16,5/16,1/16};

    // Error diffusion proposed by Jarvis, Judice, and Ninke (JJN)
    double JJN[5][5] = {{0.0,0.0,0.0,0.0,0.0},
                        {0.0,0.0,0.0,0.0,0.0},
                        {0.0,0.0,0.0,7/48,5/48},
                        {3/48,5/48,7/48,5/48,3/48},
                        {1/48,3/48,5/48,3/48,1/48}};

    // Error diffusion proposed by Stucki
    double STUCKI[5][5] = {{0,0,0,0,0},
                            {0,0,0,0,0},
                            {0,0,0,8/42,4/42},
                            {2/42,4/42,8/42,4/42,2/42},
                            {1/42,2/42,4/42,2/42,1/42}};
};