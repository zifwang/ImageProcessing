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
image::image(){
    imageHeight = 0;
    imageWidth = 0;
    BytesPerPixel = 0;
    inputFilename = "";
    outputFilename = "";
    algorithm = "";
}

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
        if (inputBuffer == NULL) {fputs ("Memory error",stderr); exit (EXIT_FAILURE);}

        // copy the file into the buffer:
        fread(inputBuffer,1, lSize, file);
    }else{
        cout << "Error opening file: " <<  inputFilename.c_str() << endl;
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

void image::writeImage(long ouputImageSize){
    FILE *file; 
    file = fopen(outputFilename.c_str(),"wb");
    if(file == NULL){
        cout << "Error opening file: " << outputFilename.c_str() << endl;
        exit(EXIT_FAILURE);
    }else{
        fwrite(outputBuffer, sizeof(unsigned char), ouputImageSize, file);
    }
    fclose(file);
}

unsigned char* image::getInputImage(){
    return inputBuffer;
}

void image::setOutputImage(unsigned char* imageMod,long imageSize){
    // allocate memory to contain the whole file:
    outputBuffer = (unsigned char*) malloc (sizeof(unsigned char)*imageSize);
    if (outputBuffer == NULL) {fputs ("Memory error",stderr); exit (EXIT_FAILURE);}
    // Store in OutputBuffer
    outputBuffer = imageMod;
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