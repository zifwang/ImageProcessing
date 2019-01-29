// Author: Zifan Wang
/**
 * This is the header file for RT.cpp
 * This RT.h/cpp is used to implement Random thresholding Dithering for Half-Toning
 * In order to break the monotones in the result from fixed thresholding, we may use a ‘random’ threshold.
 * The algorithm can be described as:
 *       • For each pixel, generate a random number in the range 0 ∼ 255, so called 𝑟𝑎𝑛𝑑(𝑖, 𝑗)
 *       • Compare the pixel value with 𝑟𝑎𝑛𝑑(𝑖, 𝑗). If it is greater, then map it to 255; otherwise, map it to 0,
 */

#pragma once 


#include <string>
#include "image.h"

using namespace std;

class RT{   
public:
    /**
     * Constructor: take in the imageData
     */
    RT(image inputImage);

    /**
     * Default Constructor
     */ 
    RT();

    /**
     * Get output image 
     */
    unsigned char* output();

    /**
     * Return image size(long type)
     */
    long getImageSize();

private:
    int imageHeight;              // Height of input image
    int imageWidth;               // Width of input image
    int BytesPerPixel;            // number of color channels
    long imageSize;               // input image Size
    unsigned char *inputBuffer;   // Input image
    unsigned char *outputBuffer;  // Output image
    /**
     * RT method: Random thresholding 
     *  In order to break the monotones in the result from fixed thresholding, we may use a ‘random’ threshold.
     * The algorithm can be described as:
     *       • For each pixel, generate a random number in the range 0 ∼ 255, so called 𝑟𝑎𝑛𝑑(𝑖, 𝑗)
     *       • Compare the pixel value with 𝑟𝑎𝑛𝑑(𝑖, 𝑗). If it is greater, then map it to 255; otherwise, map it to 0,
    */
    void methodRT();
};