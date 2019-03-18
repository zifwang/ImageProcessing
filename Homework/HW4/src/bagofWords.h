// Author: Zifan Wang
/**
 * Class for bag of words method
 */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
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

class bow{
public:
    /**
     * Constructor
     */
    bow(FlagOptions flags){
        imageHeight = flags.imageHeight;
        imageWidth = flags.imageWidth;
        BytesPerPixel = flags.BytesPerPixel;
        imageSize_ori = flags.imageHeight*flags.imageWidth*flags.BytesPerPixel;
        imageSize_gray = flags.imageHeight*flags.imageWidth;
        inputFilename = flags.inputFilename;
        outputFilename = flags.outputFilename;
        algorithm = flags.algorithm;
        numClass = flags.numClassofTraining;
        numImgPerClass = flags.numImgPerClass;
        inputBuffer = (unsigned char*) malloc (sizeof(unsigned char)*imageHeight*imageWidth*BytesPerPixel);
        if (inputBuffer == NULL) {fputs ("Memory error",stderr); exit (EXIT_FAILURE);}
        outputBuffer = (unsigned char*) malloc (sizeof(unsigned char)*imageHeight*imageWidth*BytesPerPixel);
        if (outputBuffer == NULL) {fputs ("Memory error",stderr); exit (EXIT_FAILURE);}
    }

    /**
     * Default constructor
     */
    bow(){
        imageHeight = 0;
        imageWidth = 0;
        BytesPerPixel = 0;
        inputFilename = "";
        outputFilename = "";
        algorithm = "";
    }

    /**
     * Read signle image  
     */
    void read_image(string location){
        FILE *file;                                  // Read in file.
        file = fopen(location.c_str(),"rb");
        if(file != NULL){
            // obtain file size:
            fseek (file , 0 , SEEK_END);
            long lSize = ftell (file);
            rewind (file);

            // copy the file into the buffer:
            fread(inputBuffer,1, lSize, file);
        }else{
            cout << "Error opening file: " <<  location.c_str() << endl;
            exit(EXIT_FAILURE);
        }
        fclose(file);
    }

    /**
     * Get feature descriptor
     */
    void setFeatureDescriptor(){
        // hard code to file name with input directory  
        // one type in this problem is one, other is zero with the same number of training image
        // Mat img(imageHeight,imageWidth,CV_8UC1);
        // Mat descriptor;
        // Ptr<SIFT> detector = SIFT::create();
        // vector<KeyPoint> keypoints;
        // Ptr<SiftDescriptorExtractor> extractor = SiftDescriptorExtractor::create();
        for(int i = 0; i < 5; i++){
            string name_0 = "zero_"+to_string(i+1)+".raw";
            string name_1 = "one_"+to_string(i+1)+".raw";
            name_0 = inputFilename+"/"+name_0;
            name_1 = inputFilename+"/"+name_1;
            Mat img0(imageHeight,imageWidth,CV_8UC1);
            Mat descriptor0;
            Ptr<SIFT> detector0 = SIFT::create();
            vector<KeyPoint> keypoints0;
            Ptr<SiftDescriptorExtractor> extractor0 = SiftDescriptorExtractor::create();

            // read zero_?.raw
            read_image(name_0);
            memcpy(img0.data,inputBuffer,getImageSize_ori());
            // store mat
            trainingImg.push_back(img0);
            // detect feature points
            detector0->detect(img0,keypoints0);
            // extract feature
            extractor0->compute(img0,keypoints0,descriptor0);
            // put all feature descriptors in a single Mat object
            featuresUnclustered.push_back(descriptor0); 

            Mat img1(imageHeight,imageWidth,CV_8UC1);
            Mat descriptor1;
            Ptr<SIFT> detector1 = SIFT::create();
            vector<KeyPoint> keypoints1;
            Ptr<SiftDescriptorExtractor> extractor1 = SiftDescriptorExtractor::create();
            // read one_?.raw
            read_image(name_1);
            memcpy(img1.data,inputBuffer,getImageSize_ori());
            // store mat
            trainingImg.push_back(img1);
            // detect feature points
            detector1->detect(img1,keypoints1);
            // extract feature
            extractor1->compute(img1,keypoints1,descriptor1);
            // put all feature descriptors in a single Mat object
            featuresUnclustered.push_back(descriptor1); 
        }
    }

    /**
     * BOW method
     */
    void methodBOW(){
        // define Term Criteria
        TermCriteria tc(CV_TERMCRIT_ITER,1000,0.001);
        // retries number
        int attempts=1;
        // necessary flags
        int flags=KMEANS_PP_CENTERS;
        // Create the BoW (or BoF) trainer
        BOWKMeansTrainer trainer(2,tc,attempts,flags);
        // Calculate feature
        setFeatureDescriptor();
        // Cluster the feature vectors
        Mat dictionary = trainer.cluster(featuresUnclustered);
        // cout << dictionary.rows << " " << dictionary.cols << endl;

        // // create a nearest neighbor matcher
        // Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
        // // create Sift feature point extracter
        // Ptr<DescriptorExtractor> extractor = new SiftDescriptorExtractor();
        // // create BoW descriptor extractor
        // // BOWImgDescriptorExtractor bowDE(extractor,matcher);
        // Ptr<BOWImgDescriptorExtractor> bowDE = new BOWImgDescriptorExtractor(extractor,matcher);
        // // set the dictionary with the vocabulary
        // bowDE->setVocabulary(dictionary);
        // Mat vocabulary = bowDE->getVocabulary();
        // cout << vocabulary.rows << " " << vocabulary.cols << endl;
        

        // load testing image
        Mat testImg(imageHeight,imageWidth,CV_8UC1);
        string filename = inputFilename+"/"+"eight.raw";
        read_image(filename);
        memcpy(testImg.data,inputBuffer,getImageSize_ori());

        Ptr<SIFT> detector = SIFT::create();
        vector<KeyPoint> keypoints;
        Mat descriptor;
        // Detect keypoints
        detector->detect(testImg,keypoints);
        detector->compute(testImg,keypoints,descriptor);
        
        double tempD = 0, temp = 0; 
        int index = 0;
        int histCount[dictionary.rows];
        double euclideanD[dictionary.rows]; 
        for (int i = 0; i < dictionary.rows;i++) {
            histCount[i] = 0;
        }
        for (int i = 0; i < descriptor.rows; i++) {
            for (int k = 0; k < dictionary.rows;k++) {
                for (int j = 0; j < descriptor.cols; j++) {
                    tempD += ((descriptor.at<float>(i,j)-dictionary.at<float>(k,j))*(descriptor.at<float>(i, j) - dictionary.at<float>(k, j)));
                }
                euclideanD[k] = (sqrt(tempD));
                tempD = 0.0;
            }
            temp = euclideanD[0]; 
            index = 0;

            for (int k = 1; k<dictionary.rows; k++) {
                if (temp>euclideanD[k]) {
                    temp = euclideanD[k];
                    index = k;
                }
            }	
            histCount[index] += 1;
        }

        ofstream txtOut;
        txtOut.open("his.txt");
        int max = 0;
        int indexClass = 0;
        if(txtOut.is_open()){
            for(size_t i = 0; i < dictionary.rows; i++){
                cout << histCount[i] << endl;
                txtOut << histCount[i] << " ";
                if(max < histCount[i]){ 
                    max = histCount[i];
                    indexClass = i;
                }
            }
        }
        String digitClass[2] = {"zero","one"};
        cout << "The testing image is more closed to: " << digitClass[indexClass] << endl;
        
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

private:
    int imageHeight;                    // image height
    int imageWidth;                     // image width
    int BytesPerPixel;                  // color channels
    int numClass;                       // number of training classes
    int numImgPerClass;                 // number of images in one training class
    long imageSize_ori;                 // original input image Size
    long imageSize_gray;                // gray scale input image Size
    std::string inputFilename;          // input file name
    std::string outputFilename;         // output file name
    std::string algorithm;              // algorithm to use

    /**
     * image data 
     */ 
    unsigned char *inputBuffer;
    unsigned char *outputBuffer;        // Output image 

    // To store all the descriptors that are extracted from all training images.
    Mat featuresUnclustered;
    // To store all the training image;
    vector<Mat> trainingImg;
    // Training histgram
    vector<Mat> trainHist, testHist;


    // Mat getHistVals(Mat image, Ptr<BOWImgDescriptorExtractor> extractor){
    //     Mat hist;
    //     Ptr<SIFT> detector = SIFT::create();
    //     vector<KeyPoint> keypoints;

    //     // Detect keypoints
    //     detector->detect(image,keypoints);

    //     // Comput histgram values
    //     extractor->compute(image,keypoints,hist);

    //     return hist;
    // }


};