// Author: Zifan Wang
/**
 * Class for sift detector
 */

#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include "flags.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

class sift{
public:
    /**
     * Constructor
     */
    sift(FlagOptions flags){
        imageHeight = flags.imageHeight;
        imageWidth = flags.imageWidth;
        BytesPerPixel = flags.BytesPerPixel;
        imageSize_ori = flags.imageHeight*flags.imageWidth*flags.BytesPerPixel;
        imageSize_gray = flags.imageHeight*flags.imageWidth;
        inputFilename = flags.inputFilename;
        outputFilename = flags.outputFilename;
        algorithm = flags.algorithm;
        inputBuffer_1 = (unsigned char*) malloc (sizeof(unsigned char)*imageHeight*imageWidth*BytesPerPixel);
        if (inputBuffer_1 == NULL) {fputs ("Memory error",stderr); exit (EXIT_FAILURE);}
        inputBuffer_2 = (unsigned char*) malloc (sizeof(unsigned char)*imageHeight*imageWidth*BytesPerPixel);
        if (inputBuffer_2 == NULL) {fputs ("Memory error",stderr); exit (EXIT_FAILURE);}
        string location = "";
        size_t found = inputFilename.find(".raw");
        location = inputFilename.substr(0,found) + to_string(1) + ".raw";
        // read image into unsigned char type
        inputBuffer_1 = read_image(location);
        location = inputFilename.substr(0,found) + to_string(2) + ".raw";
        inputBuffer_2 = read_image(location);
    }

    /**
     * Default constructor 
     */
    sift(){
        imageHeight = 0;
        imageWidth = 0;
        BytesPerPixel = 0;
        inputFilename = "";
        outputFilename = "";
        algorithm = "";
    }
    
    /**
     * Read image
     */ 
    unsigned char* read_image(string location){
        FILE *file;                                  // Read in file.
        file = fopen(location.c_str(),"rb");
        unsigned char* inputBuffer;
        if(file != NULL){
            // obtain file size:
            fseek (file , 0 , SEEK_END);
            long lSize = ftell (file);
            rewind (file);

            // allocate memory to contain the whole file:
            inputBuffer = new unsigned char[getImageSize_ori()];
            // copy the file into the buffer:
            fread(inputBuffer,sizeof(unsigned char), getImageSize_ori(), file);
        }else{
            cout << "Error opening file: " <<  location.c_str() << endl;
            exit(EXIT_FAILURE);
        }
        fclose(file);
        return inputBuffer;
    }

    double* rgb2gray(unsigned char *inputImage,int imageHeight, int imageWidth,int BytesPerPixel){
        double* grayImage = new double[imageHeight*imageWidth];    // Declare a return image
    
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                // rgb2gray converts RGB values to grayscale values by forming a weighted sum of the R, G, and B components:
                // 0.2989 * R + 0.5870 * G + 0.1140 * B 
                double tmp = 0.2989*double(inputImage[i*BytesPerPixel*imageWidth+j*BytesPerPixel+0])+
                                0.5870*double(inputImage[i*BytesPerPixel*imageWidth+j*BytesPerPixel+1])+
                                0.1140*double(inputImage[i*BytesPerPixel*imageWidth+j*BytesPerPixel+2]);
                // cout << tmp << endl;
                if(tmp > 255) grayImage[i*imageWidth+j] = 255;
                else if(tmp < 0) grayImage[i*imageWidth+j] = 0;
                else grayImage[i*imageWidth+j] = tmp;
            }
        }
        return grayImage;
    }

    /**
     * Change type of image
     */
    vector<int> typeMod(unsigned char* image, long imageSize){
        vector<int> returnImage;
        for(size_t i = 0; i < imageSize; i++){
            returnImage.push_back((int)image[i]);
        }
        return returnImage;
    }

    /**
     * SIFT method
     */
    void methodSIFT(double threshold){
        // put image into CV MAT
        double* grayImage_1 = new double[getImageSize_gray()];
        grayImage_1 = rgb2gray(inputBuffer_1,imageHeight,imageWidth,BytesPerPixel);
        double* grayImage_2 = new double[getImageSize_gray()];
        grayImage_2 = rgb2gray(inputBuffer_2,imageHeight,imageWidth,BytesPerPixel);
        Mat image_1(imageHeight,imageWidth,CV_8UC1);
        Mat image_2(imageHeight,imageWidth,CV_8UC1);
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                image_1.at<unsigned char>(i,j) = (unsigned char)(grayImage_1[i*imageWidth+j]);
                image_2.at<unsigned char>(i,j) = (unsigned char)(grayImage_2[i*imageWidth+j]);
            }
        }

        // find key points 
        // SIFT initi.
        Ptr<SIFT> detectorSIFT_1 = SIFT::create();
        Ptr<SIFT> detectorSIFT_2 = SIFT::create();        
        vector<KeyPoint> keypointsSIFT_1, keypointsSIFT_2;
        // SIFT detect
        detectorSIFT_1->detect(image_1,keypointsSIFT_1);
        detectorSIFT_2->detect(image_2,keypointsSIFT_2);
        // Draw keypoints & save
        Mat keypointsImageOut_1, keypointsImageOut_2;
        drawKeypoints(image_1,keypointsSIFT_1,keypointsImageOut_1);
        drawKeypoints(image_2,keypointsSIFT_2,keypointsImageOut_2);
        imwrite("river1_keypoints.jpg",keypointsImageOut_1);
        imwrite("river2_keypoints.jpg",keypointsImageOut_2);

        // sift descriptor extractor
        Ptr<SiftDescriptorExtractor> extractorSIFT = SiftDescriptorExtractor::create();
        Mat SiftDescriptor_image1, SiftDescriptor_image2;

        extractorSIFT->compute(image_1,keypointsSIFT_1,SiftDescriptor_image1);
        extractorSIFT->compute(image_2,keypointsSIFT_2,SiftDescriptor_image2);

        // Find match
        BFMatcher match;
        vector<DMatch> matchSIFT;
        match.match(SiftDescriptor_image1,SiftDescriptor_image2,matchSIFT);

        double maxSIFT = -100, minSIFT = 100;
        for(size_t i = 0; i < SiftDescriptor_image1.rows; i++){
            if(matchSIFT[i].distance > maxSIFT) maxSIFT = matchSIFT[i].distance;
            if(matchSIFT[i].distance < minSIFT) minSIFT = matchSIFT[i].distance;
        }
        // threshold match
        vector<DMatch> good_match;
        for(size_t i = 0; i < SiftDescriptor_image1.rows; i++){
            if(matchSIFT[i].distance <= threshold*minSIFT){
                good_match.push_back(matchSIFT[i]);
            }
        }

        // draw image
        Mat imageOut;
        drawMatches(image_1,keypointsSIFT_1,image_2,keypointsSIFT_2,good_match,imageOut, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
        imwrite(outputFilename,imageOut);
    }


    /**
     * Get input image
    */ 
    unsigned char* getiInputImage_1(){
        return inputBuffer_1;
    }
    unsigned char* getiInputImage_2(){
        return inputBuffer_2;
    }

    /**
     * Get output image 
     */
    unsigned char* output(){
        return outputBuffer;
    }

    /**
     * Return image color type size(long type) and gray type
     */
    long getImageSize_ori(){
        return imageSize_ori;
    }
    long getImageSize_gray(){
        return imageSize_gray;
    }

    /**
     * Return image Height(int type) and width
     */
    int getImageHeight(){
        return imageHeight;
    }
    int getImageWidth(){
        return imageWidth;
    }
    int getBytesPerPixel(){
        return BytesPerPixel;
    }

protected:
    int imageHeight;                    // Height of input image
    int imageWidth;                     // Width of input image
    int BytesPerPixel;                  // number of color channels
    long imageSize_ori;                 // original input image Size
    long imageSize_gray;                // gray scale input image Size
    std::string inputFilename;          // input file name
    std::string outputFilename;         // output file name
    std::string algorithm;              // algorithm to use
    unsigned char *inputBuffer_1;       // Input image 1
    unsigned char *inputBuffer_2;       // Input image 1
    unsigned char *outputBuffer;        // Output image 


};