#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"

using namespace std;

// Constructor: init. all private variable
image::image(FlagOptions flags){
    imageHeight = flags.imageHeight;
    imageWidth = flags.imageWidth;
    BytesPerPixel = flags.BytesPerPixel;
    inputFilename = flags.inputFilename;
    outputFilename = flags.outputFilename;
    algorithm = flags.algorithm;
    readImage();
}

// // Default Constructor:
// image_data::image_data(){
//     imageHeight = 0;
//     imageWidth = 0;
//     BytesPerPixel = 0;
//     inputFilename = NULL;
//     outputFilename = NULL;
//     algorithm = NULL;
// }

void image::readImage(){
    FILE *file;                                  // Read in file.
    file = fopen(inputFilename.c_str(),"rb");
    if(file != NULL){
        // obtain file size:
        fseek (file , 0 , SEEK_END);
        long lSize = ftell (file);
        rewind (file);

        // allocate memory to contain the whole file:
        inputBuffer = (unsigned char*) malloc (sizeof(unsigned char)*lSize);
        if (inputBuffer == NULL) {fputs ("Memory error",stderr); exit (2);}

        // copy the file into the buffer:
        fread(inputBuffer,1, lSize, file);
    }else{
        cout << "Error opening file: " <<  inputFilename.c_str() << endl;
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

void image::writeImage(){
    FILE *file; 
    file = fopen(outputFilename.c_str(),"wb");
    if(file == NULL){
        cout << "Error opening file: " << outputFilename.c_str() << endl;
        exit(EXIT_FAILURE);
    }else{
        fwrite(outputData, sizeof(unsigned char), imageHeight*imageWidth*BytesPerPixel, file);
    }
    fclose(file);
}

void image::copy(){
    outputData = inputBuffer;
}

int image::getImageHeight(){
    return imageHeight;
}

int image::getImageWidth(){
    return imageWidth;
}

int image::getBytesPerPixel(){
    return BytesPerPixel;
}

string image::getAlgorithm(){
    return algorithm;
}