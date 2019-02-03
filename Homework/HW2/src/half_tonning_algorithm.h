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
        input2DImage = oneDTOtwoD(inputBuffer,imageHeight,imageWidth);
        // allocate memory to contain the whole file:
        outputBuffer = (unsigned char*) malloc (sizeof(unsigned char)*imageSize);
        if (outputBuffer == NULL) {
            fputs ("Memory error",stderr); 
            exit (EXIT_FAILURE);
        }
    }

    /**
     * Get input image
    */ 
    unsigned char* getiInputImage(){
        return inputBuffer;
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
    unsigned char** input2DImage;       // Input 2D image
    unsigned char *outputBuffer;        // Output image

    /**
     * Function to calculate 2D image
    */ 
    unsigned char** oneDTOtwoD(unsigned char* inputImage, int height, int width){
        unsigned char** outputImage;
        outputImage = new unsigned char*[height];
        for(int i = 0; i < height; i++){
            outputImage[i] = new unsigned char[width];
        }

        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                outputImage[i][j] = inputImage[i*width+j];
            }
        }
        return outputImage;
    }

    /**
     * Function to extend input image depending on filter's size
     */
    unsigned char** imageExtension(unsigned char* inputImage, int height, int width, int filterSize){
        // Resize 1D image array to 2D image array
        unsigned char** tempImage = oneDTOtwoD(inputImage,height,width);

        // output image initization
        unsigned char** outputImage;
        unsigned char** outputTempImage;
        outputTempImage = new unsigned char*[height+filterSize-1];
        for(int i = 0; i < height+filterSize-1; i++){
            outputTempImage[i] = new unsigned char[width];
        }
        outputImage = new unsigned char*[height+filterSize-1];
        for(int i = 0; i < height+filterSize-1; i++){
            outputImage[i] = new unsigned char[width+filterSize-1];
        }

        // set extented image
        for(int h = 0; h < height+filterSize-1; h++){
            for(int w = 0; w < width; w++){
                if(h < filterSize/2){
                    int actHeight = filterSize/2 - h - 1;
                    outputTempImage[h][w] = tempImage[actHeight][w];
                }
                else if(h > height+filterSize/2-1){
                    int actHeight = height - (h-height) + (filterSize/2-1);
                    outputTempImage[h][w] = tempImage[actHeight][w];
                }
                else outputTempImage[h][w] = tempImage[h-filterSize/2][w];
            }
        }
        for(int w = 0; w < width+filterSize-1; w++){
            for(int h = 0; h < height+filterSize-1; h++){
                if(w < filterSize/2){
                    int actWidth = filterSize/2 - w -1;
                    outputImage[h][w] = outputTempImage[h][actWidth];
                }
                else if(w > width+filterSize/2-1){
                    int actWidth = width - (w-width) + (filterSize/2-1);
                    outputImage[h][w] = outputTempImage[h][actWidth];
                }
                else outputImage[h][w] = outputTempImage[h][w-filterSize/2];
            }
        }

        return outputImage;
    }

    /**
     * Function to normalize image
     */
    double* normalizeImage(unsigned char* inputImage, int height, int width){
        double* normalization;
        for(long i = 0; i < long(height)*long(width); i++){
            normalization[i] = double(inputImage[i])/255;
        }
        return normalization;
    }

    /**
     * Function to convert 1D normalized image to 2D
    */
    double** oneDTOtwoD_double(double* inputImage, int height, int width){
        double** outputImage;
        outputImage = new double*[height];
        for(int i = 0; i < height; i++){
            outputImage[i] = new double[width];
        }

        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                outputImage[i][j] = inputImage[i*width+j];
            }
        }
        return outputImage;
    }

    /**
     * Function to extend double type image
    */
    double** imageExtension_double(double* inputImage, int height, int width, int filterSize){
        // Resize 1D image array to 2D image array
        double** tempImage = oneDTOtwoD_double(inputImage,height,width);

        // output image initization
        double** outputImage;
        double** outputTempImage;
        outputTempImage = new double*[height+filterSize-1];
        for(int i = 0; i < height+filterSize-1; i++){
            outputTempImage[i] = new double[width];
        }
        outputImage = new double*[height+filterSize-1];
        for(int i = 0; i < height+filterSize-1; i++){
            outputImage[i] = new double[width+filterSize-1];
        }

        // set extented image
        for(int h = 0; h < height+filterSize-1; h++){
            for(int w = 0; w < width; w++){
                if(h < filterSize/2){
                    int actHeight = filterSize/2 - h - 1;
                    outputTempImage[h][w] = tempImage[actHeight][w];
                }
                else if(h > height+filterSize/2-1){
                    int actHeight = height - (h-height) + (filterSize/2-1);
                    outputTempImage[h][w] = tempImage[actHeight][w];
                }
                else outputTempImage[h][w] = tempImage[h-filterSize/2][w];
            }
        }
        for(int w = 0; w < width+filterSize-1; w++){
            for(int h = 0; h < height+filterSize-1; h++){
                if(w < filterSize/2){
                    int actWidth = filterSize/2 - w -1;
                    outputImage[h][w] = outputTempImage[h][actWidth];
                }
                else if(w > width+filterSize/2-1){
                    int actWidth = width - (w-width) + (filterSize/2-1);
                    outputImage[h][w] = outputTempImage[h][actWidth];
                }
                else outputImage[h][w] = outputTempImage[h][w-filterSize/2];
            }
        }

        return outputImage;
    }

};
