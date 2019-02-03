#pragma once

#include "image.h"
#include "half_tonning_algorithm.h"
#include <iostream>
#include <string>

class errorDiffusion : public half_tonning_algorithm{
public:
    void methodErrorDiffusion(string algorithm){
        double* oriInputImage = normalizeImage(inputBuffer,imageHeight,imageWidth);
        unsigned char ouputImage[imageHeight][imageWidth];

        // if(algorithm == "steinberg"){
        //     double** extendedInputImage = imageExtension_double(oriInputImage,imageHeight,imageWidth,3);
        //     double error = 0;
        //     for(int i = 0; i < imageHeight; i++){
        //         if(i%2 == 0){
        //             for(int j = 0; j < imageWidth; j++){
        //                 if(extendedInputImage[i+1][j+1] > threshold) {
        //                     error = extendedInputImage[i+1][j+1] -1;
        //                     ouputImage[i][j] = (unsigned char)255;
        //                 }else{
        //                     error = extendedInputImage[i+1][j+1];
        //                     ouputImage[i][j] = (unsigned char)0;
        //                 }
        //                 // error dist.
        //                 extendedInputImage[i+1][j+1+1] = extendedInputImage[i+1][j+1+1] * STEINBERG[1][2]*error;
        //                 extendedInputImage[i+1+1][j+1-1] = extendedInputImage[i+1+1][j+1-1] * STEINBERG[2][0]*error;
        //                 extendedInputImage[i+1+1][j+1+0] = extendedInputImage[i+1+1][j+1+0] * STEINBERG[2][1]*error;
        //                 extendedInputImage[i+1+1][j+1+1] = extendedInputImage[i+1+1][j+1+1] * STEINBERG[2][2]*error;
        //             }
        //         }
        //         else{
        //             for(int j = imageWidth-1; j >= 0; j--){
        //                 if(extendedInputImage[i+1][j+1] > threshold) {
        //                     error = extendedInputImage[i+1][j+1] -1;
        //                     ouputImage[i][j] = (unsigned char)255;
        //                 }else{
        //                     error = extendedInputImage[i+1][j+1];
        //                     ouputImage[i][j] = (unsigned char)0;
        //                 }
        //                 // error dist.
        //                 extendedInputImage[i+1][j+1-1] = extendedInputImage[i+1][j+1-1] * STEINBERG[1][2]*error;
        //                 extendedInputImage[i+1+1][j+1-1] = extendedInputImage[i+1+1][j+1-1] * STEINBERG[2][2]*error;
        //                 extendedInputImage[i+1+1][j+1+0] = extendedInputImage[i+1+1][j+1+0] * STEINBERG[2][1]*error;
        //                 extendedInputImage[i+1+1][j+1+1] = extendedInputImage[i+1+1][j+1+1] * STEINBERG[2][0]*error;
        //             }
        //         }
        //     }
        // }
        // else if(algorithm == "JJN"){
        //     double** extendedInputImage = imageExtension_double(oriInputImage,imageHeight,imageWidth,5);
        //     double error = 0;

        //     for(int i = 0; i < imageHeight; i++){
        //         if(i%2 == 0){
        //             for(int j = 0; j < imageWidth; j++){
        //                 if(extendedInputImage[i+2][j+2] > threshold) {
        //                     error = extendedInputImage[i+2][j+2] -1;
        //                     ouputImage[i][j] = (unsigned char)255;
        //                 }else{
        //                     error = extendedInputImage[i+2][j+2];
        //                     ouputImage[i][j] = (unsigned char)0;
        //                 }
        //                 // error dist.
        //                 extendedInputImage[i+2][j+2+1] = extendedInputImage[i+2][j+2+1] + error * JJN[2][3];
        //                 extendedInputImage[i+2][j+2+2] = extendedInputImage[i+2][j+2+2] + error * JJN[2][4];
        //                 extendedInputImage[i+2+1][j+2-2] = extendedInputImage[i+2+1][j+2-2] + error * JJN[3][0];
        //                 extendedInputImage[i+2+1][j+2-1] = extendedInputImage[i+2+1][j+2-1] + error * JJN[3][1];
        //                 extendedInputImage[i+2+1][j+2+0] = extendedInputImage[i+2+1][j+2+0] + error * JJN[3][2];
        //                 extendedInputImage[i+2+1][j+2+1] = extendedInputImage[i+2+1][j+2+1] + error * JJN[3][3];
        //                 extendedInputImage[i+2+1][j+2+2] = extendedInputImage[i+2+1][j+2+2] + error * JJN[3][4];
        //                 extendedInputImage[i+2+2][j+2-2] = extendedInputImage[i+2+2][j+2-2] + error * JJN[4][0];
        //                 extendedInputImage[i+2+2][j+2-1] = extendedInputImage[i+2+2][j+2-1] + error * JJN[4][1];
        //                 extendedInputImage[i+2+2][j+2+0] = extendedInputImage[i+2+2][j+2+0] + error * JJN[4][2];
        //                 extendedInputImage[i+2+2][j+2+1] = extendedInputImage[i+2+2][j+2+1] + error * JJN[4][3];
        //                 extendedInputImage[i+2+2][j+2+2] = extendedInputImage[i+2+2][j+2+2] + error * JJN[4][4];
        //             }
        //         }
        //         else{
        //             for(int j = imageWidth-1; j >= 0; j++){
        //                 if(extendedInputImage[i+2][j+2] > threshold) {
        //                     error = extendedInputImage[i+2][j+2] -1;
        //                     ouputImage[i][j] = (unsigned char)255;
        //                 }else{
        //                     error = extendedInputImage[i+2][j+2];
        //                     ouputImage[i][j] = (unsigned char)0;
        //                 }
        //                 // error dist.
        //                 extendedInputImage[i+2][j+2-1] = extendedInputImage[i+2][j+2-1] + error * JJN[2][3];
        //                 extendedInputImage[i+2][j+2-2] = extendedInputImage[i+2][j+2-2] + error * JJN[2][4];
        //                 extendedInputImage[i+2+1][j+2-2] = extendedInputImage[i+2+1][j+2-2] + error * JJN[3][4];
        //                 extendedInputImage[i+2+1][j+2-1] = extendedInputImage[i+2+1][j+2-1] + error * JJN[3][3];
        //                 extendedInputImage[i+2+1][j+2+0] = extendedInputImage[i+2+1][j+2+0] + error * JJN[3][2];
        //                 extendedInputImage[i+2+1][j+2+1] = extendedInputImage[i+2+1][j+2+1] + error * JJN[3][1];
        //                 extendedInputImage[i+2+1][j+2+2] = extendedInputImage[i+2+1][j+2+2] + error * JJN[3][0];
        //                 extendedInputImage[i+2+2][j+2-2] = extendedInputImage[i+2+2][j+2-2] + error * JJN[4][4];
        //                 extendedInputImage[i+2+2][j+2-1] = extendedInputImage[i+2+2][j+2-1] + error * JJN[4][3];
        //                 extendedInputImage[i+2+2][j+2+0] = extendedInputImage[i+2+2][j+2+0] + error * JJN[4][2];
        //                 extendedInputImage[i+2+2][j+2+1] = extendedInputImage[i+2+2][j+2+1] + error * JJN[4][1];
        //                 extendedInputImage[i+2+2][j+2+2] = extendedInputImage[i+2+2][j+2+2] + error * JJN[4][0];
        //             }
        //         }
        //     }
        // }
        // else if(algorithm == "STUCKI"){
        //     double** extendedInputImage = imageExtension_double(oriInputImage,imageHeight,imageWidth,5);
        //     double error = 0;

        //     for(int i = 0; i < imageHeight; i++){
        //         if(i%2 == 0){
        //             for(int j = 0; j < imageWidth; j++){
        //                 if(extendedInputImage[i+2][j+2] > threshold) {
        //                     error = extendedInputImage[i+2][j+2] -1;
        //                     ouputImage[i][j] = (unsigned char)255;
        //                 }else{
        //                     error = extendedInputImage[i+2][j+2];
        //                     ouputImage[i][j] = (unsigned char)0;
        //                 }
        //                 // error dist.
        //                 extendedInputImage[i+2][j+2+1] = extendedInputImage[i+2][j+2+1] + error * STUCKI[2][3];
        //                 extendedInputImage[i+2][j+2+2] = extendedInputImage[i+2][j+2+2] + error * STUCKI[2][4];
        //                 extendedInputImage[i+2+1][j+2-2] = extendedInputImage[i+2+1][j+2-2] + error * STUCKI[3][0];
        //                 extendedInputImage[i+2+1][j+2-1] = extendedInputImage[i+2+1][j+2-1] + error * STUCKI[3][1];
        //                 extendedInputImage[i+2+1][j+2+0] = extendedInputImage[i+2+1][j+2+0] + error * STUCKI[3][2];
        //                 extendedInputImage[i+2+1][j+2+1] = extendedInputImage[i+2+1][j+2+1] + error * STUCKI[3][3];
        //                 extendedInputImage[i+2+1][j+2+2] = extendedInputImage[i+2+1][j+2+2] + error * STUCKI[3][4];
        //                 extendedInputImage[i+2+2][j+2-2] = extendedInputImage[i+2+2][j+2-2] + error * STUCKI[4][0];
        //                 extendedInputImage[i+2+2][j+2-1] = extendedInputImage[i+2+2][j+2-1] + error * STUCKI[4][1];
        //                 extendedInputImage[i+2+2][j+2+0] = extendedInputImage[i+2+2][j+2+0] + error * STUCKI[4][2];
        //                 extendedInputImage[i+2+2][j+2+1] = extendedInputImage[i+2+2][j+2+1] + error * STUCKI[4][3];
        //                 extendedInputImage[i+2+2][j+2+2] = extendedInputImage[i+2+2][j+2+2] + error * STUCKI[4][4];
        //             }
        //         }
        //         else{
        //             for(int j = imageWidth-1; j >= 0; j++){
        //                 if(extendedInputImage[i+2][j+2] > threshold) {
        //                     error = extendedInputImage[i+2][j+2] -1;
        //                     ouputImage[i][j] = (unsigned char)255;
        //                 }else{
        //                     error = extendedInputImage[i+2][j+2];
        //                     ouputImage[i][j] = (unsigned char)0;
        //                 }
        //                 // error dist.
        //                 extendedInputImage[i+2][j+2-1] = extendedInputImage[i+2][j+2-1] + error * STUCKI[2][3];
        //                 extendedInputImage[i+2][j+2-2] = extendedInputImage[i+2][j+2-2] + error * STUCKI[2][4];
        //                 extendedInputImage[i+2+1][j+2-2] = extendedInputImage[i+2+1][j+2-2] + error * STUCKI[3][4];
        //                 extendedInputImage[i+2+1][j+2-1] = extendedInputImage[i+2+1][j+2-1] + error * STUCKI[3][3];
        //                 extendedInputImage[i+2+1][j+2+0] = extendedInputImage[i+2+1][j+2+0] + error * STUCKI[3][2];
        //                 extendedInputImage[i+2+1][j+2+1] = extendedInputImage[i+2+1][j+2+1] + error * STUCKI[3][1];
        //                 extendedInputImage[i+2+1][j+2+2] = extendedInputImage[i+2+1][j+2+2] + error * STUCKI[3][0];
        //                 extendedInputImage[i+2+2][j+2-2] = extendedInputImage[i+2+2][j+2-2] + error * STUCKI[4][4];
        //                 extendedInputImage[i+2+2][j+2-1] = extendedInputImage[i+2+2][j+2-1] + error * STUCKI[4][3];
        //                 extendedInputImage[i+2+2][j+2+0] = extendedInputImage[i+2+2][j+2+0] + error * STUCKI[4][2];
        //                 extendedInputImage[i+2+2][j+2+1] = extendedInputImage[i+2+2][j+2+1] + error * STUCKI[4][1];
        //                 extendedInputImage[i+2+2][j+2+2] = extendedInputImage[i+2+2][j+2+2] + error * STUCKI[4][0];
        //             }
        //         }
        //     }
        // }
        // else{
        //     cout << "Error inputing error diffusion algorithm." << endl;
        //     exit(EXIT_FAILURE);
        // }


        // // set outputBuffer
        // for(int i = 0; i < imageHeight; i++){
        //     for(int j = 0; j < imageWidth; j++){
        //         outputBuffer[i*imageWidth+j] = ouputImage[i][j];
        //     }
        // }
    }



private:
    double threshold = 0.5;    // Error diffusion threshold

    // Floyd-Steinberg’s error diffusion with the serpentine scanning
    const double STEINBERG[3][3] = {{0,0,0},
                                    {0,0,7/16},
                                    {3/16,5/16,1/16}};

    // Error diffusion proposed by Jarvis, Judice, and Ninke (JJN)
    const double JJN[5][5] = {{0,0,0,0,0},
                              {0,0,0,0,0},
                              {0,0,0,7/48,5/48},
                              {3/48,5/48,7/48,5/48,3/48},
                              {1/48,3/48,5/48,3/48,1/48}};

    // Error diffusion proposed by Stucki
    const double STUCKI[5][5] = {{0,0,0,0,0},
                                 {0,0,0,0,0},
                                 {0,0,0,8/42,4/42},
                                 {2/42,4/42,8/42,4/42,2/42},
                                 {1/42,2/42,4/42,2/42,1/42}};
};