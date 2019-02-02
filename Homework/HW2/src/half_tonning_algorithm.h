#pragma once 

#include "image.h"
#include <iostream>
#include <string>

using namespace std;

class half_tonning_algorithm{
public:
    // image set up
    void init_image(image inputImage){
        imageHeight = inputImage.getImageHeight();
        imageWidth = inputImage.getImageWidth();
        BytesPerPixel = inputImage.getBytesPerPixel();
        imageSize = imageHeight*imageWidth*BytesPerPixel;
        inputBuffer = inputImage.getInputImage();
        // set_2D_buffer();
        // allocate memory to contain the whole file:
        outputBuffer = (unsigned char*) malloc (sizeof(unsigned char)*imageSize);
        if (outputBuffer == NULL) {
            fputs ("Memory error",stderr); 
            exit (EXIT_FAILURE);
        }
    }

    /**
     * Get output image 
     */
    unsigned char* output(){
        return outputBuffer;
    }

    /**
     * Return image size(long type)
     */
    long getImageSize(){
        return imageSize;
    }

protected:
    int imageHeight;                    // Height of input image
    int imageWidth;                     // Width of input image
    int BytesPerPixel;                  // number of color channels
    long imageSize;                     // input image Size
    unsigned char *inputBuffer;         // Input image
    unsigned char *outputBuffer;        // Output image
};
