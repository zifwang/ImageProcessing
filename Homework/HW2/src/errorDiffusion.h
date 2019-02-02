#pragma once

#include "image.h"
#include "half_tonning_algorithm.h"
#include <iostream>
#include <string>

class errorDiffusion : public half_tonning_algorithm{
public:
    void methodErrorDiffusion(){
        unsigned char** extendedInputImage = imageExtension(inputBuffer,imageHeight,imageWidth,3);

        // set outputBuffer
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                outputBuffer[i*imageWidth+j] = extendedInputImage[i][j];
            }
        }
    }



private:
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