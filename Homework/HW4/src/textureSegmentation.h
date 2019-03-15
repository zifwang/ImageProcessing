// Author: Zifan
/** 
 * header file for textureSegmentation.cpp 
*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include "flags.h"

using namespace std;

class textureSegmentation{
public:
    /** 
     * Constructor: Init. private variable
     */
    textureSegmentation(FlagOptions flags);

    /**
     * Default Constructor: Init. private variale to null
     */
    textureSegmentation();

    /**
     * Read image
     */ 
    void read_image(string location);

    /**
     * Get energy per pixel
     */
    double getPixelEnergy(double* image, int height, int width, int x, int y, int windowsSize);

    /**
     * Method to do texture classification
     */
    void methodSegmentation();

    /**
     * Method to save array into txt file.
     */
    void saveArray(double* array, int rows, int cols, string filename);

    /**
     * Method to save unsigned char image
     */
    void saveImage(double* array, int height, int width, int colorChannels, string filename);


    /**
     * This function is used to extended the image
     */
    double* padding(double* inputImage, int imageHeight, int imageWidth, int paddingSize);

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

    /**
     * Return the input file name
     */
    string getFileName_input();

    /**
     * Return the laws filer
     */
    vector<double*> getLawsFilter();

    /**
     * Return the featureVectors
     */
    map<int,vector<double>> getfeatureVectors();

    /**
     * Return the texture_doubles;
     */
    map<int,vector<double>> getTextureDouble(){
        return texture_double;
    }

private:
    int imageHeight;                    // image height
    int imageWidth;                     // image width
    int BytesPerPixel;                  // color channels
    std::string inputFilename;          // input file name
    std::string outputFilename;         // output file name
    std::string algorithm;              // algorithm to use

    /**
     * image data you want to access publicly
     */ 
    unsigned char *inputBuffer;

    // Training image vector
    map<int,vector<unsigned char>> texture_unsigned_char;  // in unsigned char type
    map<int,vector<double>> texture_double;     // in double type

    // laws filter
    double L5 [5] = {1,4,6,4,1};        // Level
    double E5 [5] = {-1,-2,0,2,1};      // Edge
    double S5 [5] = {-1,0,2,0,-1};      // Spot
    double W5 [5] = {-1,2,0,-2,1};      // Wave
    double R5 [5] = {1,-4,6,-4,1};      // Ripple

    vector<double*> laws_filters_bank;  // laws filter bank container

    // featureVectors
    map<int,vector<double>> featureVectors;


    // helper function 
    /**
     * Function to create laws filter bank
     */    
    void create_laws_filters(); 

    /**
     * Function to do tensorproduct
     */ 
    double* multiply(double* array1, double* array2);

    /**
     * Function to calculate the image mean pixel value
     * return double type 
     */ 
    double getImageMean(double* image, int height, int width);

    /**
     * Function to reduce illumination effect
     * Return a 2D double array same to the image size
     */
    double* reduceIllumination(double* image, int height, int width, double imageMean);

    /**
     * Function to calculate the average image energy
     * Return a double type: averaged image energy
     */
    double getImageEnergy(double* image, int height, int width);

    /**
     * Function to do filter in the image
     * Return a 2D double array with the same size to ori image
     */
    double* getFilteredImage(double* image, double* filter, int height, int width, int paddingSize);

    /**
     * Function to find mean
     * Return double
     */
    double getMean(double* array,int size);

    /**
     * Function to find variance
     * Return double
     */
    double getVariance(double* array, int size, double mean);

    /**
     * Print array in terminal
     */
    void arrayDisplay(double* array, int numRows, int numCols);
};