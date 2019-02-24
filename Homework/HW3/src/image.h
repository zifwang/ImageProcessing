// Author: Zifan
/** 
 * header file for image.cpp 
*/
#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "flags.h"

using namespace std;

class image{
public:
    /** 
     * Constructor: Init. private variable
     */
    image(FlagOptions flags);

    /**
     * Default Constructor: Init. private variale to null
     */
    image();


    /**
     * Read in file
     */ 
    void readImage();

    /**
     * Write to file
     */ 
    void writeImage(long ouputImageSize);

    /**
     * Return 1-D array of input image
     */
    unsigned char* getInputImage();

    /**
     * Function used to set output image
     */ 
    void setOutputImage(unsigned char* imageMod,long imageSize);

    /** 
     * Return image height (int type)
    */
    int getImageHeight();

    /** 
     * Return image width (int type)
     */
    int getImageWidth();

    /**
     * Return how many color channel (int type)
    */
    int getBytesPerPixel();

    /**
     * Return the algorithm used in this program
     */
    string getAlgorithm();

private:
    int imageHeight;
    int imageWidth;
    int BytesPerPixel;
    std::string inputFilename;
    std::string outputFilename;
    std::string algorithm;
    /**
     * image data you want to access publicly
     */ 
    unsigned char *inputBuffer;

    /**
     * output image data used to write to file
     */
    unsigned char *outputBuffer;
};