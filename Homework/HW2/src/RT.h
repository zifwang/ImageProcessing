// Author: Zifan Wang
/**
 * This is the method of Random Thresholding for half-tonning and is the inheritance from half_tonning_algorithm class
 * This RT.h/cpp is used to implement Random thresholding Dithering for Half-Toning
 * In order to break the monotones in the result from fixed thresholding, we may use a ‘random’ threshold.
 * The algorithm can be described as:
 *       • For each pixel, generate a random number in the range 0 ∼ 255, so called 𝑟𝑎𝑛𝑑(𝑖, 𝑗)
 *       • Compare the pixel value with 𝑟𝑎𝑛𝑑(𝑖, 𝑗). If it is greater, then map it to 255; otherwise, map it to 0,
 */

#pragma once 

#include <string>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <random>
#include "half_tonning_algorithm.h"
#include "image.h"

using namespace std;

class RT : public half_tonning_algorithm{   
public:
    /**
     * RT method: Random thresholding 
     *  In order to break the monotones in the result from fixed thresholding, we may use a ‘random’ threshold.
     * The algorithm can be described as:
     *       • For each pixel, generate a random number in the range 0 ∼ 255, so called 𝑟𝑎𝑛𝑑(𝑖, 𝑗)
     *       • Compare the pixel value with 𝑟𝑎𝑛𝑑(𝑖, 𝑗). If it is greater, then map it to 255; otherwise, map it to 0,
    */
    void methodRT(){
        // Check BytesPerPixel  
        if(BytesPerPixel > 1){
            cout << "Error: image has to be in grayscale to do Random Thresholding Dithering" << endl;
            exit(EXIT_FAILURE);
        }

        // construct a trivial random generator engine from a time-based seed:
        // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        default_random_engine generator (100);
        uniform_int_distribution<int> distribution(0,255);

        // Create a 3D unsigned char array: better for imagination
        long imageSize = imageHeight*imageWidth*BytesPerPixel;

        for(int i = 0; i < imageSize; i++){
            // Loop through the inputimage array which is 1-D
            if(inputBuffer[i] >= 0 && inputBuffer[i] < distribution(generator)){
                outputBuffer[i] = (unsigned char)0;
            }
            else if(inputBuffer[i] <= 255 && inputBuffer[i] >= distribution(generator)){
                outputBuffer[i] = (unsigned char)255;
            }
        }
    }
};