// Author: Zifan Wang
/**
 * Geometric Transformation cpp
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "geoTransformation.h"

using namespace std;


/**
 * Image set up
 */
void geoTransformation::init_image(image inputImage){
    string filename = inputImage.getFileName_input();
    imageHeight = inputImage.getImageHeight();
    imageWidth = inputImage.getImageWidth();
    BytesPerPixel = inputImage.getBytesPerPixel();
    imageSize_ori = imageHeight*imageWidth*BytesPerPixel;
    imageSize_gray = imageHeight*imageWidth;
    inputBuffer = inputImage.getInputImage();
    // allocate memory to contain the whole output file:
    outputBuffer = (unsigned char*) malloc (sizeof(unsigned char)*imageSize_gray);
    if (outputBuffer == NULL) {
        fputs ("Memory error",stderr); 
        exit (EXIT_FAILURE);
    }
    // allocate memory to contain the sub image
    subImage1 = new unsigned char[getSubImageSize(filename,1)];
    subImage1 = readSubImage(filename,1);
    // allocate memory to contain the sub image
    subImage2 = new unsigned char[getSubImageSize(filename,2)];
    subImage2 = readSubImage(filename,2);
    // allocate memory to contain the sub image
    subImage3 = new unsigned char[getSubImageSize(filename,2)];
    subImage3 = readSubImage(filename,3);
}

/**
 * Set output Image
 */
void geoTransformation::set_0_1_OutputImage(int* tmpImage){
    // Set ouput image
    for(int i = 0; i < imageSize_gray; i++){
        outputBuffer[i] = (unsigned char) tmpImage[i]*255;
    }
}

/**
 * Get input image
 */ 
unsigned char* geoTransformation::getiInputImage(){
    return inputBuffer;
}

/**
 * Get output image 
 */
unsigned char* geoTransformation::output(){
    return outputBuffer;
}

/**
 * Return image color type size(long type) and gray type
 */
long geoTransformation::getImageSize_ori(){
    return imageSize_ori;
}
long geoTransformation::getImageSize_gray(){
    return imageSize_gray;
}

/**
 * Return image Height(int type) and width
 */
int geoTransformation::getImageHeight(){
    return imageHeight;
}
int geoTransformation::getImageWidth(){
    return imageWidth;
}

/**
 * Implement GEO transformation
 */
void geoTransformation::methodGEO(){
    
    vector<pair<int,int>> corners = getPossibleCorners(inputBuffer,imageHeight,imageWidth,800000000000);
    for(int i = 0; i < corners.size(); i++){
        cout << "x: " << corners[i].first << ", j:" << corners[i].second << endl;
    }
    cout << corners.size() << endl;
}



















// Private function define here 
/**
 * This function is used to get the sub image size
 */
unsigned char* geoTransformation::readSubImage(string filename, int number){
    FILE *file;
    size_t found = filename.find(".raw");
    string newName = filename.substr(0,found) + to_string(number) + filename.substr(found);
    file = fopen(newName.c_str(),"rb");
    unsigned char* subImage; 
    if(file != NULL){
        // obtain file size:
        fseek (file , 0 , SEEK_END);
        long lSize = ftell (file);
        rewind (file);
        // allocate memory to contain the whole file:
        subImage = new unsigned char[lSize];
        // copy the file into the buffer:
        fread(subImage,1, lSize, file);
    }else{
        cout << "Error opening file: " << newName.c_str() << endl;
        exit(EXIT_FAILURE);
    }
    fclose(file);

    return subImage;
}

/**
 * This function is used to get the sub image's size
 */
long geoTransformation::getSubImageSize(string filename, int number){
    FILE *file;
    size_t found = filename.find(".raw");
    string newName = filename.substr(0,found) + to_string(number) + filename.substr(found);
    file = fopen(newName.c_str(),"rb");
    long imgSize;
    if(file != NULL){
        // obtain file size:
        fseek (file , 0 , SEEK_END);
        imgSize = ftell (file);
        rewind (file);
    }else{
        cout << "Error opening file: " << newName.c_str() << endl;
        exit(EXIT_FAILURE);
    }
    fclose(file);
    return imgSize;
}

/**
 * This function is used to extended the image
 */
unsigned char* geoTransformation::padding(unsigned char* inputImage, int imageHeight, int imageWidth, int paddingSize){
    unsigned char* imageTemp = new unsigned char[(imageHeight+paddingSize*2)*imageWidth];
    unsigned char* imageExtend = new unsigned char[(imageHeight+paddingSize*2)*(imageWidth+paddingSize*2)];
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

/**
 * This function is used to find possible corners in the image
 */
vector<pair<int,int>> geoTransformation::getPossibleCorners(unsigned char* inputImage, int imageHeight, int imageWidth, double RThreshold){
    // extend image
    unsigned char* imageExtended = new unsigned char[(imageHeight+3*2)*(imageWidth+3*2)];
    imageExtended = padding(inputImage,imageHeight,imageWidth,3);         // use 3 because apply the sobel derviates
    double* imageExtended_double = new double[(imageHeight+3*2)*(imageWidth+3*2)];
    // transfer to double type
    for(long i = 0; i < (imageHeight+3*2)*(imageWidth+3*2); i++){
        imageExtended_double[i] = double(imageExtended[i]);
    }
    // remove extend image_unsigned char type
    delete[] imageExtended;

    // Define the 3*3 sobel mask
    double Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};     // Hor changes
    double Gy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};     // Vertical changes
    // Compute x and y derivatives of image: Ix = Gx*I, Iy = Gy*I
    double* gradient_x = new double[imageHeight*imageWidth];
    double* gradient_y = new double[imageHeight*imageWidth];
    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            double sumX = 0;
            double sumY = 0;
            for(int fh = -1; fh <= 1; fh++){
                for(int fw = -1; fw <= 1; fw++){
                    sumX = sumX + imageExtended_double[(i+3+fh)*(imageWidth+3*2)+(j+3+fw)]*Gx[fh+1][fw+1];
                    sumY = sumY + imageExtended_double[(i+3+fh)*(imageWidth+3*2)+(j+3+fw)]*Gy[fh+1][fw+1];
                }
            }
            gradient_x[i*imageWidth+j] = sumX;
            gradient_y[i*imageWidth+j] = sumY;
        }
    }
    // Compute products of derivatives at every pixel: Ix2 = Ix*Ix, Iy2 = Iy*Iy, Ixy = Ix*Iy
    // Compute the sums of the products of derivatives at each pixel: Sx2 = G*Ix2, Sy2 =G*Iy2, ASxy = G*Ixy
    double M[2][2];
    double determinant = 0;
    double trace = 0;
    double R = 0;
    double I_x_2 = 0;
    double I_y_2 = 0;
    double I_x_y = 0;    
    vector<pair<int,int>> possibleCorners; 
    for(int i = 1; i < imageHeight-1; i++){
        for(int j = 1; j < imageWidth-1; j++){
            I_x_2 = 0;
            I_y_2 = 0;
            I_x_y = 0;
            for(int fi = -1; fi <= 1; fi++){
                for(int fj = -1; fj <= 1; fj++){
                    I_x_2 += gradient_x[(i+fi)*imageWidth+j+fj]*gradient_x[(i+fi)*imageWidth+j+fj];
                    I_y_2 += gradient_y[(i+fi)*imageWidth+j+fj]*gradient_x[(i+fi)*imageWidth+j+fj];
                    I_x_y += gradient_y[(i+fi)*imageWidth+j+fj]*gradient_x[(i+fi)*imageWidth+j+fj];
                }
            }
            M[0][0] = I_x_2;
            M[0][1] = I_x_y;
            M[1][0] = I_x_y;
            M[1][1] = I_y_2;
            determinant = M[0][0]*M[1][1] - (M[0][1]*M[1][0]);
            trace = M[0][0] + M[1][1];
            R = determinant - 0.04*trace*trace;

            if(R > RThreshold){
                possibleCorners.push_back(make_pair(j,i));
            }
        }
    }
    // 
    return possibleCorners;

}

