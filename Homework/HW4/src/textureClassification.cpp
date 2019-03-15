#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "textureClassification.h"

using namespace std;
using namespace cv;

// Constructor: init. all private variable
textureClassification::textureClassification(FlagOptions flags){
    imageHeight = flags.imageHeight;
    imageWidth = flags.imageWidth;
    BytesPerPixel = flags.BytesPerPixel;
    inputFilename = flags.inputFilename;
    outputFilename = flags.outputFilename;
    algorithm = flags.algorithm;

    // hard code 12 texture images
    for(int i = 1; i < 13; i++){
        // Get correct location based on the input file name
        string location = "";
        size_t found = inputFilename.find(".raw");
        location = inputFilename.substr(0,found) + to_string(i) + ".raw";

        // read image into unsigned char type
        read_image(location);
        
        // put into map container
        for(long j = 0; j < imageWidth*imageHeight; j++){
            texture_unsigned_char[i-1].push_back(inputBuffer[j]);
            texture_double[i-1].push_back((double)inputBuffer[j]);
        }
    }
    // Init. 25 laws filters
    create_laws_filters();
}

// Default Constructor:
textureClassification::textureClassification(){
    imageHeight = 0;
    imageWidth = 0;
    BytesPerPixel = 0;
    inputFilename = "";
    outputFilename = "";
    algorithm = "";
}

// read single image
void textureClassification::read_image(string location){
    FILE *file;                                  // Read in file.
    file = fopen(location.c_str(),"rb");
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
        cout << "Error opening file: " <<  location.c_str() << endl;
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

// method to do texture classification
void textureClassification::methodClassification(){
    map<int,vector<double>> texture_double_tmp;
    // Step 1: subtract image mean (reduce illumination effects)
    for(int i = 0; i < texture_double.size(); i++){;
        // get ori image from texture_double vector
        double* tmp_image = new double[imageHeight*imageWidth];
        for(size_t j = 0; j < texture_double[i].size(); j++){
            tmp_image[j] = texture_double[i][j];
        }
        // get image mean
        double imageMean = getImageMean(tmp_image,imageHeight,imageWidth);
        tmp_image = reduceIllumination(tmp_image,imageHeight,imageWidth,imageMean);
        for(size_t j = 0; j < texture_double[i].size(); j++){
            texture_double_tmp[i].push_back(tmp_image[j]);
        }
        delete[] tmp_image;
    }

    
    // Step 2: padding + apply 25 laws filter bank
    // Step 3: Average energy to form a 25-D feature vector
    for(int i = 0; i < texture_double_tmp.size(); i++){
        double* tmp_image = new double[(imageHeight+5*2)*(imageWidth+5*2)];
        // padding:
        double* image = new double[imageHeight*imageWidth];
        for(size_t j = 0; j < texture_double_tmp[i].size(); j++){
            image[j] = texture_double_tmp[i][j];
        }
        tmp_image = padding(image,imageHeight,imageWidth,5);
        // cout << i << ": ";
        for(int j = 0; j < laws_filters_bank.size(); j++){
            double* filteredImage = new double[imageHeight*imageWidth];
            // Apply filter
            filteredImage = getFilteredImage(tmp_image, laws_filters_bank[j], imageHeight, imageWidth, 5);
            // Get average energy
            double imageEnergy = getImageEnergy(filteredImage,imageHeight,imageWidth);
            // cout << int(imageEnergy) << " ";
            featureVectors[i].push_back(imageEnergy);
            delete[] filteredImage;
        }
        // cout << endl;
        delete[] tmp_image;
        delete[] image;
    }

    // Step 4: feature normalization
    double featureVectors_array[texture_double_tmp.size()][laws_filters_bank.size()];
    double featureVectors_normalized[texture_double_tmp.size()*laws_filters_bank.size()];
    for(int i = 0; i < texture_double_tmp.size(); i++){
        // cout << i << ": ";
        for(int j = 0; j < laws_filters_bank.size(); j++){
            featureVectors_array[i][j] = featureVectors[i][j];
            // cout << int(featureVectors_array[i][j]) << " ";
        }
        // cout << endl;
    }
    // Normalized by dim
    // for(int i = 0; i < laws_filters_bank.size(); i++){
    //     double tmp_feature_array[texture_double_tmp.size()];
    //     for(int j = 0; j < texture_double_tmp.size(); j++){
    //         tmp_feature_array[j] = featureVectors_array[j][i];
    //     }
    //     double feature_mean = getMean(tmp_feature_array,texture_double_tmp.size());
    //     double variance = getVariance(tmp_feature_array,texture_double_tmp.size(), feature_mean);
    //     for(int j = 0; j < texture_double_tmp.size(); j++){
    //         featureVectors_normalized[j*laws_filters_bank.size()+i] = (featureVectors_array[j][i]-feature_mean)/sqrt(variance);
    //     }
    // }

    // Normalized by image
    for(int i = 0; i < texture_double_tmp.size(); i++){
        double tmp_feature_array[laws_filters_bank.size()];
        for(int j = 0; j < laws_filters_bank.size(); j++){
            tmp_feature_array[j] = featureVectors_array[i][j];
        }
        double feature_mean = getMean(tmp_feature_array,laws_filters_bank.size());
        double variance = getVariance(tmp_feature_array,laws_filters_bank.size(),feature_mean);
        for(int j = 0; j < laws_filters_bank.size(); j++){
            featureVectors_normalized[i*laws_filters_bank.size()+j] = (featureVectors_array[i][j]-feature_mean)/sqrt(variance);
            // featureVectors_normalized[i*laws_filters_bank.size()+j] = featureVectors_array[i][j];
        }
    }

    
    // Step 5: PCA -> reduce to 3 dimensional
    cv::Mat featureVectors_mat(12,25,CV_32F);
    for(int i = 0; i < texture_double_tmp.size(); i++){
        for(int j = 0; j < laws_filters_bank.size(); j++){
            featureVectors_mat.at<float>(i,j) = featureVectors_normalized[i*laws_filters_bank.size()+j];
        }
    }

    // PCA
    cv::PCA pca_analysis(featureVectors_mat,cv::Mat(),CV_PCA_DATA_AS_ROW,3);
    
    // Project to 3D
    cv::Mat projectTo3D = pca_analysis.project(featureVectors_mat);
    int rows = projectTo3D.rows;
    int cols = projectTo3D.cols;
    
    double feature_vector_3D_array[rows*cols];
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            feature_vector_3D_array[i*cols+j] = projectTo3D.at<float>(i,j);
        }
    }
    
    // arrayDisplay(featureVectors_normalized,texture_double_tmp.size(),laws_filters_bank.size());
    saveArray(featureVectors_normalized,texture_double_tmp.size(),laws_filters_bank.size(),"texture_25D.txt");

    // arrayDisplay(feature_vector_3D_array,rows,cols);
    saveArray(feature_vector_3D_array,rows,cols,"texture_3D.txt");

    // Step 6: K-means
    // 25D
    Mat labels_25, centers_25;
    kmeans(featureVectors_mat,4,labels_25,TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 0.01),1,KMEANS_RANDOM_CENTERS,centers_25);
    cout << "25D: " << endl;
    cout << labels_25 << endl;
    
    Mat labels_3, centers_3;
    kmeans(projectTo3D,4,labels_3,TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 0.01),1,KMEANS_RANDOM_CENTERS,centers_3);
    cout << "3D: " << endl;
    cout << labels_3 << endl;

}

// Save array into txt file.
void textureClassification::saveArray(double* array, int rows, int cols, string filename){
    ofstream dataFile(filename.c_str());
    if(!dataFile){
        cout << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            dataFile << array[i*cols+j] << " ";
        }
        dataFile << "\n";
    }
    dataFile.close();
}

// Padding
double* textureClassification::padding(double* inputImage, int imageHeight, int imageWidth, int paddingSize){
    double* imageTemp = new double[(imageHeight+paddingSize*2)*imageWidth];
    double* imageExtend = new double[(imageHeight+paddingSize*2)*(imageWidth+paddingSize*2)];
    // Extend height(row)
    for(int h = 0; h < imageHeight+paddingSize*2; h++){
        for(int w = 0; w < imageWidth; w++){
            if(h < paddingSize){
                int actHeight = paddingSize - h - 1;
                imageTemp[h*imageWidth+w] = inputImage[actHeight*imageWidth+w];
            }
            else if(h > imageHeight+paddingSize-1){
                int actHeight = imageHeight - (h-imageHeight) + (paddingSize-1);
                imageTemp[h*imageWidth+w] = inputImage[actHeight*imageWidth+w];
            }
            else imageTemp[h*imageWidth+w] = inputImage[(h-paddingSize)*imageWidth+w];
        }
    }
    // Extend width(col)
    for(int w = 0; w < imageWidth+paddingSize*2; w++){
        for(int h = 0; h < imageHeight+paddingSize*2; h++){
            if(w < paddingSize){
                int actWidth = paddingSize - w - 1;
                imageExtend[h*(imageWidth+paddingSize*2)+w] = imageTemp[h*(imageWidth)+actWidth];
            }
            else if(w > imageWidth+paddingSize-1){
                int actWidth = imageWidth - (w-imageWidth) + (paddingSize-1);
                imageExtend[h*(imageWidth+paddingSize*2)+w]= imageTemp[h*(imageWidth)+actWidth];
            }
            else{
                imageExtend[h*(imageWidth+paddingSize*2)+w]= imageTemp[h*(imageWidth)+(w-paddingSize)];
            }
        }
    }
    delete[] imageTemp;
    return imageExtend;
}


// Get image height
int textureClassification::getImageHeight(){
    return imageHeight;
}

// Get image width
int textureClassification::getImageWidth(){
    return imageWidth;
}

// Get number of color channels
int textureClassification::getBytesPerPixel(){
    return BytesPerPixel;
}

// Get the algorithm used
string textureClassification::getAlgorithm(){
    return algorithm;
}

// Get input file name
string textureClassification::getFileName_input(){
    return inputFilename;
}

// Get laws filters
vector<double*> textureClassification::getLawsFilter(){
    return laws_filters_bank;
}

// Get feature vectors
map<int,vector<double>> textureClassification::getfeatureVectors(){
    return featureVectors;
}



/**
 * Helper function 
 */ 
void textureClassification::create_laws_filters(){
    vector<double*> tmp_filter_bank;
    tmp_filter_bank.push_back(L5);
    tmp_filter_bank.push_back(E5);
    tmp_filter_bank.push_back(S5);
    tmp_filter_bank.push_back(W5);
    tmp_filter_bank.push_back(R5);

    // generate filter laws filter bank;
    for(int i = 0; i < tmp_filter_bank.size(); i++){
        for(int j = 0; j < tmp_filter_bank.size(); j++){
            laws_filters_bank.push_back(multiply(tmp_filter_bank[i],tmp_filter_bank[j]));
        }
    }
}

double* textureClassification::multiply(double* array1, double* array2){
    int array1_len = 5;
    int array2_len = 5;
    int len = array1_len*array2_len;
    double* returnArray = new double[len];
    for(int i = 0; i < array1_len; i++){
        for(int j = 0; j < array2_len; j++){
            returnArray[i*array2_len+j] = array1[i]*array2[j];
        }
    }
    return returnArray;
}

double textureClassification::getImageMean(double* image, int height, int width){
    double sum = 0;
    for(long i = 0; i < height*width; i++){
        sum = sum + image[i];
    }
    return sum/(height*width);
}

double* textureClassification::reduceIllumination(double* image, int height, int width, double imageMean){
    double* returnImage = new double[height*width];
    for(size_t i = 0; i < height*width; i++){
        returnImage[i] = image[i] - imageMean;
    }
    return returnImage;
}

double textureClassification::getImageEnergy(double* image, int height, int width){
    double energy = 0;
    for(size_t i = 0; i < height*width; i++){
        energy = energy + image[i]*image[i];
    }
    return energy/(height*width);
}

double* textureClassification::getFilteredImage(double* image, double* filter, int height, int width, int paddingSize){
    double* filteredImage = new double[height*width];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            double newPixelVal = 0;
            for(int fi = -paddingSize/2; fi <= paddingSize/2; fi++){
                for(int fj = -paddingSize/2; fj <= paddingSize/2; fj++){
                    newPixelVal = newPixelVal + image[(i+paddingSize+fi)*(width+paddingSize*2)+(j+paddingSize+fj)]*filter[(fi+paddingSize/2)*paddingSize+(fj+paddingSize/2)];
                }
            }
            filteredImage[i*width+j] = newPixelVal;
        }
    }
    return filteredImage;
}

double textureClassification::getMean(double* array,int size){
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum = sum + array[i];
    }
    return sum/size;
}

double textureClassification::getVariance(double* array, int size, double mean){
    double variance = 0;
    for(int i = 0; i < size; i++){
        variance = variance + (array[i]-mean)*(array[i]-mean);
    }
    return variance/(size-1);
}

void textureClassification::arrayDisplay(double* array, int numRows, int numCols){
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            cout << array[i*numCols+j] << " ";
        }
        cout << endl;
    }
}