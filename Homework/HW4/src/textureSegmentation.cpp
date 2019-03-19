#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "textureSegmentation.h"

using namespace std;
using namespace cv;

void matSave(cv::Mat array, int rows, int cols, string filename){
    ofstream dataFile(filename.c_str());
    if(!dataFile){
        cout << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            dataFile << array.at<int>(i*cols+j,0) << " ";
        }
        dataFile << "\n";
    }
    dataFile.close();
}

void matSaveOri(cv::Mat array, int rows, int cols, string filename){
    ofstream dataFile(filename.c_str());
    if(!dataFile){
        cout << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            dataFile << array.at<float>(i,j) << " ";
        }
        dataFile << "\n";
    }
    dataFile.close();
}

// Constructor: init. all private variable
textureSegmentation::textureSegmentation(FlagOptions flags){
    imageHeight = flags.imageHeight;
    imageWidth = flags.imageWidth;
    BytesPerPixel = flags.BytesPerPixel;
    inputFilename = flags.inputFilename;
    outputFilename = flags.outputFilename;
    algorithm = flags.algorithm;

    read_image(inputFilename);
    
    // Init. 25 laws filters
    create_laws_filters();
}

// Default Constructor:
textureSegmentation::textureSegmentation(){
    imageHeight = 0;
    imageWidth = 0;
    BytesPerPixel = 0;
    inputFilename = "";
    outputFilename = "";
    algorithm = "";
}

// read single image
void textureSegmentation::read_image(string location){
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

// method to get pixel energy
double textureSegmentation::getPixelEnergy(double* image, int height, int width, int x, int y, int windowsSize){
    double sum = 0;
    for(int m = -windowsSize/2; m <= windowsSize/2; m++){
        for(int n = -windowsSize/2; n <= windowsSize/2; n++){
            // cout << image[(x+i+windowsSize)*(width+windowsSize*2)+(y+j+windowsSize)] << " ";
            sum = sum + pow(image[(x+m+windowsSize)*(width+windowsSize*2)+(y+n+windowsSize)],2);
        }
    }
    // cout << endl;
    
    return sum/(windowsSize*windowsSize);
}


// method to do texture classification
void textureSegmentation::methodSegmentation(){
    // Get ori image to the double type
    double* inputImage = new double[imageHeight*imageWidth];
    for(size_t i = 0; i < imageHeight*imageWidth; i++){
        inputImage[i] = double(inputBuffer[i]);
    }
    double* extentedImage = new double[(imageHeight+5*2)+(imageWidth+5*2)];
    extentedImage = padding(inputImage,imageHeight,imageWidth,5);

    // store 25 image
    map<int,vector<double>> imageMap;
    for(int i = 0; i < laws_filters_bank.size(); i++){
        double* filteredImage = new double[imageHeight*imageWidth];
        filteredImage = getFilteredImage(extentedImage, laws_filters_bank[i], imageHeight, imageWidth, 5);
        // string filename = "texture"+to_string(i+1)+".raw";
        // saveImage(filteredImage,imageHeight,imageWidth,3,filename);
        string filename = "texture"+to_string(i+1)+".txt";
        // saveArray(filteredImage,imageHeight,imageWidth,filename);
        for(size_t j = 0; j < imageHeight*imageWidth; j++){
            imageMap[i].push_back(filteredImage[j]);
        }
    }

    cout << "Start getting pixel energy.........." << endl;
    // pixel energy
    Mat pixelEnergy(imageHeight*imageWidth,laws_filters_bank.size(),CV_32F);
    int windowsSize = 30;
    for(int i = 0; i < laws_filters_bank.size(); i++){
        // cout << i << endl;
        vector<double> image = imageMap[i];
        double* oriImage = new double[imageHeight*imageWidth];
        for(size_t z = 0; z < imageHeight*imageWidth; z++){
            oriImage[z] = image[z];
        }
        double* extentedImage = new double[(imageHeight+windowsSize*2)+(imageWidth+windowsSize*2)];
        extentedImage = padding(oriImage,imageHeight,imageWidth,windowsSize);
        for(int j = 0; j < imageHeight; j++){
            for(int k = 0; k < imageWidth; k++){
                double pixel_energy = getPixelEnergy(extentedImage,imageHeight,imageWidth,j,k,windowsSize);
                pixelEnergy.at<float>(j*imageWidth+k,i) = pixel_energy;
            }
        }
    }
    // normalize pixel energy 
    for(size_t i = 0; i < imageHeight*imageWidth; i++){
        for(int j = 0; j < laws_filters_bank.size(); j++){
            pixelEnergy.at<float>(i,j) = pixelEnergy.at<float>(i,j)/pixelEnergy.at<float>(i,0);
        }
    }

    cout << "Finish getting pixel energy!" << endl;
    
    // matSaveOri(pixelEnergy, imageHeight*imageWidth,laws_filters_bank.size(), "pixelenergy.txt");
    
    // perform pca
    cv::PCA pca_analysis(pixelEnergy,cv::Mat(),CV_PCA_DATA_AS_ROW,3);
    cv::Mat projectTo3D = pca_analysis.project(pixelEnergy);


    // Kmeans
    Mat labels, centers;
    // PCA
    kmeans(projectTo3D, 7, labels, TermCriteria(TermCriteria::EPS+TermCriteria::COUNT, 1000, 0.001 ), 2, KMEANS_RANDOM_CENTERS, centers);
    // no PCA
    // kmeans(pixelEnergy, 7, labels, TermCriteria(TermCriteria::EPS+TermCriteria::COUNT, 1000, 0.001 ), 2, KMEANS_RANDOM_CENTERS, centers);
    // matSave(labels,imageHeight,imageWidth,"labels.txt");
    // Set output image
    double* outputImage = new double[imageHeight*imageWidth];
    Mat imgOut(imageHeight,imageWidth,CV_8UC1);
    for(int i = 0; i < imageHeight; i++){
		for(int j = 0; j < imageWidth; j++){
            double pixelVal;
			if(labels.at<int>(i*imageWidth+j,0) == 0){
                pixelVal = 0;
            }
            if(labels.at<int>(i*imageWidth+j,0) == 1){
                pixelVal = 42;
            }
            if(labels.at<int>(i*imageWidth+j,0) == 2){
                pixelVal = 84;
            }
            if(labels.at<int>(i*imageWidth+j,0) == 3){
                pixelVal = 126;
            }
            if(labels.at<int>(i*imageWidth+j,0) == 4){
                pixelVal = 168;
            }
            if(labels.at<int>(i*imageWidth+j,0) == 5){
                pixelVal = 210;
            }
            if(labels.at<int>(i*imageWidth+j,0) == 6){
                pixelVal = 255;
            }
            outputImage[i*imageWidth+j] = pixelVal;
            imgOut.at<unsigned char>(i,j) = (unsigned char) pixelVal;
		}
	}
    imwrite(outputFilename,imgOut);
    // saveImage(outputImage, imageHeight, imageWidth, 1, outputFilename);
}

// Save array into txt file.
void textureSegmentation::saveArray(double* array, int rows, int cols, string filename){
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

// Save Image
void textureSegmentation::saveImage(double* array, int height, int width, int colorChannels, string filename){
    unsigned char outputImage[height*width];
    for(size_t i = 0; i < height*width; i++){
        if(array[i] < 0){
            outputImage[i] = (unsigned char) 0;
        }
        else if(array[i] > 255){
            outputImage[i] = (unsigned char) 255;
        }else{
            outputImage[i] = (unsigned char)array[i];
        }
    } 
    FILE *file;
    file = fopen(filename.c_str(),"wb");
    if(file == NULL){
        cout << "Error opening file: " << filename.c_str() << endl;
        exit(EXIT_FAILURE);
    }else{
        fwrite(outputImage, sizeof(unsigned char), height*width, file);
    }
    fclose(file);
}


// Padding
double* textureSegmentation::padding(double* inputImage, int imageHeight, int imageWidth, int paddingSize){
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
int textureSegmentation::getImageHeight(){
    return imageHeight;
}

// Get image width
int textureSegmentation::getImageWidth(){
    return imageWidth;
}

// Get number of color channels
int textureSegmentation::getBytesPerPixel(){
    return BytesPerPixel;
}

// Get the algorithm used
string textureSegmentation::getAlgorithm(){
    return algorithm;
}

// Get input file name
string textureSegmentation::getFileName_input(){
    return inputFilename;
}

// Get laws filters
vector<double*> textureSegmentation::getLawsFilter(){
    return laws_filters_bank;
}

// Get feature vectors
map<int,vector<double>> textureSegmentation::getfeatureVectors(){
    return featureVectors;
}



/**
 * Helper function 
 */ 
void textureSegmentation::create_laws_filters(){
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

double* textureSegmentation::multiply(double* array1, double* array2){
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

double textureSegmentation::getImageMean(double* image, int height, int width){
    double sum = 0;
    for(long i = 0; i < height*width; i++){
        sum = sum + image[i];
    }
    return sum/(height*width);
}

double* textureSegmentation::reduceIllumination(double* image, int height, int width, double imageMean){
    double* returnImage = new double[height*width];
    for(size_t i = 0; i < height*width; i++){
        returnImage[i] = image[i] - imageMean;
    }
    return returnImage;
}

double textureSegmentation::getImageEnergy(double* image, int height, int width){
    double energy = 0;
    for(size_t i = 0; i < height*width; i++){
        energy = energy + image[i]*image[i];
    }
    return energy/(height*width);
}

double* textureSegmentation::getFilteredImage(double* image, double* filter, int height, int width, int paddingSize){
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

double textureSegmentation::getMean(double* array,int size){
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum = sum + array[i];
    }
    return sum/size;
}

double textureSegmentation::getVariance(double* array, int size, double mean){
    double variance = 0;
    for(int i = 0; i < size; i++){
        variance = variance + (array[i]-mean)*(array[i]-mean);
    }
    return variance/(size-1);
}

void textureSegmentation::arrayDisplay(double* array, int numRows, int numCols){
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            cout << array[i*numCols+j] << " ";
        }
        cout << endl;
    }
}