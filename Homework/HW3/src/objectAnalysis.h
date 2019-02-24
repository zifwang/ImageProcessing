// Author: Zifan Wang
/**
 * Implement the object analysis.
 * 
 * 
*/

#pragma once 

#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <math.h>
#include "morphology.h"
#include "image.h"

using namespace std;

class objectAnalysis : public morphology{
public:
    void analysis(){
        int* preproImage = new int[getImageSize_gray()];
        int* shrinkImage = new int[getImageSize_gray()];
        unsigned char* shrinkImageOut = new unsigned char[getImageSize_gray()];
        int* thinImage = new int[getImageSize_gray()];
        preproImage = preprocessImage();
        // Shrink operation
        shrinkImage = morphologyOperation(preproImage,getImageHeight(),getImageWidth(),getShrinkMark(),getShrinkThinUncondMark(),58,173);
        int countRice = 0;
        for(int i = 0; i < imageSize_gray; i++){
            if(shrinkImage[i] == 1){
                countRice++;
            }
            shrinkImageOut[i] = (unsigned char)shrinkImage[i]*255;
        }
        cout << "There are " << countRice << " grains in the image." << endl;
        // Output shrink image
        FILE *file_shrink; 
        if(!(file_shrink = fopen("out_rice_after_Shrink.raw","wb"))){
            cout << "Cannot open file: out_rice_after_Shrink.raw." << endl;
            exit(1);
        }
        fwrite(shrinkImageOut, sizeof(unsigned char), (imageHeight*imageWidth), file_shrink);
        fclose(file_shrink);
        delete[] shrinkImage;
        delete[] shrinkImageOut;

        // Thin operation
        thinImage = morphologyOperation(preproImage,getImageHeight(),getImageWidth(),getThinMark(),getShrinkThinUncondMark(),46,173);
        
        for(int i = 0; i < imageSize_gray; i++){
            outputBuffer[i] = (unsigned char)thinImage[i]*255;
        }
        cout << "Position Print" << endl;
        // Vector to contains grains
        vector<vector<pair<int,int>>> grains;
        // bool
        bool hit = false;

        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                if(thinImage[i*imageWidth+j] == 1){
                    // cout << "x: "<< j << ", y: " << i << endl;
                    hit = false;
                    coordinate = make_pair(j,i);
                    // cout << "Grains.size: "<< grains.size() << endl;
                    for(int k = 0; k < grains.size(); k++){
                        vector<pair<int,int>> tmpPositionVec = grains[k];
                        // cout << "tmpPosition.size: " << tmpPositionVec.size() << endl;
                        for(int z = 0; z < tmpPositionVec.size(); z++){
                            if(abs(tmpPositionVec[z].first-j) < 12 && abs(tmpPositionVec[z].second-i) < 12){
                                // cout << tmpPositionVec[z].first-j << " " << tmpPositionVec[z].second-i << endl;
                                tmpPositionVec.push_back(coordinate);
                                grains[k] = tmpPositionVec;
                                hit = true;
                                break;
                            }
                        }
                        if(hit == true) break;
                    }
                    // cout << "hit: "<< hit << endl;
                    if(hit != true){
                        vector<pair<int,int>> tmpPositionVec_2;
                        tmpPositionVec_2.push_back(coordinate);
                        grains.push_back(tmpPositionVec_2);
                        // cout << "Init Vector" << endl;
                    }

                }
            }
        }
        cout << "Finish catgorize" << endl;
        cout <<  grains.size() << endl;
        for(int i = 0; i < grains.size(); i++){
            vector<pair<int,int>> tmpPositionVec = grains[i];
            cout << "Grain " << i << ": "<< endl;
            for(int j = 0; j < tmpPositionVec.size(); j++){
                cout << "x: " << tmpPositionVec[j].first << ", y: " << tmpPositionVec[j].second << endl;
            }
        }
        // need to find a way to calculate the long axis.

        delete[] thinImage;
        delete[] preproImage;
    }


private:
    pair<int,int> coordinate;
    
    /**
     * This function is used to preprocess the image
     * 1.median Filter -> 2. binarized Image -> 3. Add three color images together -> 4. holeFilling_gray -> 5.removing_iso_pixel_gray
     * return a image array: value from 0(0)-1(255)
     */
    int* preprocessImage(){
        // int* reconstructedImage = new int[getImageSize_gray()];
        // reconstructedImage = holeFilling_gray(threeChannelsReconstruction(),imageHeight,imageWidth);
        // return reconstructedImage;
        unsigned char* redImage = new unsigned char[getImageSize_gray()];   // image not in red color
        unsigned char* greenImage = new unsigned char[getImageSize_gray()]; // image not in green color
        unsigned char* blueImage = new unsigned char[getImageSize_gray()];  // image not in blue color
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                redImage[i*imageWidth+j] = inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+0];
                greenImage[i*imageWidth+j] = inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+1];
                blueImage[i*imageWidth+j] = inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+2];
            }
        }
        // 
        int* redImage_median = new int[getImageSize_gray()];   // image 
        int* greenImage_median = new int[getImageSize_gray()]; // image 
        int* blueImage_median = new int[getImageSize_gray()];  // image 
        redImage_median = medianFilter(redImage,getImageHeight(),getImageWidth(),5);
        greenImage_median = medianFilter(greenImage,getImageHeight(),getImageWidth(),5);
        blueImage_median = medianFilter(blueImage,getImageHeight(),getImageWidth(),5);
        redImage_median = binarizeImage(redImage_median,getImageHeight(),getImageWidth(),1,15,false);
        greenImage_median = binarizeImage(greenImage_median,getImageHeight(),getImageWidth(),1,15,false);
        blueImage_median = binarizeImage(blueImage_median,getImageHeight(),getImageWidth(),1,15,false);

        // Add three images together
        int* tmpImage = new int[getImageSize_gray()];
        for(int i = 0; i < getImageSize_gray(); i++){
            if(((int)redImage_median[i] == 255)||((int)greenImage_median[i] == 255)||((int)blueImage_median[i] == 255)){
                tmpImage[i] = 1;
            }else{
                tmpImage[i] = 0;
            }
        }
        
        // Delete pointers here
        delete[] redImage;
        delete[] greenImage;
        delete[] blueImage;
        delete[] redImage_median;
        delete[] greenImage_median;
        delete[] blueImage_median;
        tmpImage = holeFilling_gray(tmpImage,imageHeight,imageWidth);
        tmpImage = removing_iso_pixel_gray(tmpImage,imageHeight,imageWidth);

        return tmpImage;
    }

    /**
     * This function is unsed
     */
    int* threeChannelsReconstruction(){
        // original image
        int* oriImage = new int[getImageSize_gray()];
        oriImage = getOriImage_0_1();

        int* opp_redImage = new int[getImageSize_gray()];   // image not in red color
        int* opp_greenImage = new int[getImageSize_gray()]; // image not in green color
        int* opp_blueImage = new int[getImageSize_gray()];  // image not in blue color
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                if((int)inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+0] < 50){
                    opp_redImage[i*imageWidth+j] = 1;
                }
                else{
                    opp_redImage[i*imageWidth+j] = 0;
                }
                if((int)inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+1] < 50){
                    opp_greenImage[i*imageWidth+j] = 1;
                }
                else{
                    opp_greenImage[i*imageWidth+j] = 0;
                }
                if((int)inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+2] < 50){
                    opp_blueImage[i*imageWidth+j] = 1;
                }
                else{
                    opp_blueImage[i*imageWidth+j] = 0;
                }
            }
        }
        // Add four images together
        int* returnImage = new int[getImageSize_gray()];
        for(int i = 0; i < getImageSize_gray(); i++){
            if((oriImage[i] == 1)||(opp_redImage[i] == 1)||(opp_greenImage[i] == 1)||(opp_blueImage[i] == 1)){
                returnImage[i] = 1;
            }else{
                returnImage[i] = 0;
            }
        }
        // Delete the tmp image pointers
        delete[] oriImage;
        delete[] opp_redImage;
        delete[] opp_greenImage;
        delete[] opp_blueImage;

        return returnImage;
    }

    /**
     * Based on the most apperance pixel value to decide the output image
     * return binarized image
     */
    int* binarizeImage(int* inputImage, int imageHeight, int imageWidth, int channels, double radius, bool invert){
        if(channels != 1){
            cout << "Error: image have to int gray scale to perform conversion" << endl;
            exit(EXIT_FAILURE);
        }
        int* histogram = new int[256];
        for(int i = 0; i < 256; i++){
            histogram[i] = 0;
        }
        // get histogram
        for(int i = 0; i < imageWidth*imageHeight; i++){
            histogram[inputImage[i]]++;
        }
        int maxApperance = 0;
        int modPixel = 0;
        for(int i = 0; i < 256; i++){
            if(histogram[i] > maxApperance){
                maxApperance = histogram[i];
                modPixel = i;
            }
        }

        //
        int* returnImage = new int[imageHeight*imageWidth];
        int pixelVal = 0;
        for(int i = 0; i < imageWidth*imageHeight; i++){
            if((inputImage[i] >= modPixel-radius) && (inputImage[i] < modPixel+radius)){
                pixelVal = 0;
            }else{
                pixelVal = 255;
            }    
            if(invert) pixelVal = 255 - pixelVal;
            // cout << pixelVal << endl;
            returnImage[i] = pixelVal;
        }
        delete[] histogram;
        return returnImage;
    }


    // Median filter algorithm
    int* medianFilter(unsigned char* inputImage, int imageHeight, int imageWidth, int filterSize){
        unsigned char* imageExtend = new unsigned char[(imageHeight+filterSize*2)*(imageWidth+filterSize*2)];
        imageExtend = padding(inputImage,imageHeight,imageWidth,filterSize);
        
    
        // apply median filter
        int* filteredImage = new int[imageHeight*imageWidth];
        for(int h = 0; h < imageHeight; h++){
            for(int w = 0; w < imageWidth; w++){
                int tempArray[filterSize*filterSize];
                int i = 0;
                for(int fh = -filterSize/2; fh <= filterSize/2; fh++){
                    for(int fw = -filterSize/2; fw <= filterSize/2; fw++){
                        tempArray[i] = int(imageExtend[(h+filterSize+fh)*(imageWidth+filterSize*2)+(w+filterSize+fw)]);
                        i++;
                    }
                }
                filteredImage[h*imageWidth+w] = getMedian(tempArray,i);
            }
        }

        return filteredImage;
    }

    // Function to get median in an array
    int getMedian(int inputArray[], int arraySize){
        // Allocate an array of the same size and sort it.
        int* dpSorted = new int[arraySize];
        for (int i = 0; i < arraySize; ++i) {
            dpSorted[i] = inputArray[i];
        }
        for (int i = arraySize - 1; i > 0; --i) {
            for (int j = 0; j < i; ++j) {
                if (dpSorted[j] > dpSorted[j+1]) {
                    int dTemp = dpSorted[j];
                    dpSorted[j] = dpSorted[j+1];
                    dpSorted[j+1] = dTemp;
                }
            }
        }
        // Middle or average of middle values in the sorted array.
        int dMedian = 0.0;
        if ((arraySize % 2) == 0) {
            dMedian = (dpSorted[arraySize/2] + dpSorted[(arraySize/2) - 1])/2.0;
        } else {
            dMedian = dpSorted[arraySize/2];
        }
        delete[] dpSorted;
        return dMedian;
    }
};