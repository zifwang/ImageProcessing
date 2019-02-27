// Author: Zifan Wang
/**
 * Geometric Transformation cpp
*/

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "image.h"
#include "geoTransformation.h"

#define PI 3.14159265


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

void helper_print_points(vector<pair<int,int>> corners){
    for(int i = 0; i < corners.size(); i++){
        cout << "x: " << corners[i].first << ", j:" << corners[i].second << endl;
    }
}

double* rotated_image(vector<pair<int,int>> corners, unsigned char* subImage, int subImgHeight, int subImgWidth){
    // Get height & width & angle of each sub image
    int true_height = sqrt(pow((corners[1].first-corners[2].first),2)+pow((corners[1].second-corners[2].second),2)); 
    int true_width = sqrt(pow((corners[1].first-corners[3].first),2)+pow((corners[1].second-corners[3].second),2)); 
    double angle = atan(double(corners[1].second-corners[3].second)/double(corners[1].first-corners[3].first));
    
    int height_min = corners[2].second;
    int height_max = corners[3].second;
    int width_min = corners[0].first;
    int width_max = corners[1].first;
    cout << height_min << " " << height_max << " " << width_min << " " << width_max << endl;
    int center_x = (width_max+width_min)/2;
    int center_y = (height_max+height_min)/2;
    cout << center_x << " "<< center_y << endl;
    // cout << width_min << " " << width_max << " " << height_min << " " << height_max << endl;
    
    double cos_theta = cos(-angle);
    double sin_theta = sin(-angle);
    cout << -angle << " " << cos_theta << " " << sin_theta << endl;
    // double rotation_matrix[3][3] = {{cos_theta,-sin_theta,0},{sin_theta,cos_theta,0},{0,0,1}};
    double rotated_x, rotated_y;
    
    double tmpImage[subImgHeight][subImgWidth];
    double* returnImage = new double[subImgHeight*subImgWidth]; 
    for(int i = 0; i < subImgHeight; i++){
        for(int j = 0; j < subImgWidth; j++){
            tmpImage[i][j] = (double)subImage[i*subImgWidth+j];
            returnImage[i*subImgWidth+j] = 255;
        }
    }

    for(int y = height_min; y <= height_max; y++){
        for(int x = width_min; x <= width_max; x++){
            rotated_x = x - center_x;
            rotated_y = y - center_y;
            double rotated_x_tmp = rotated_x;
            double rotated_y_tmp = rotated_y;
            rotated_x = cos_theta*rotated_x_tmp - sin_theta*rotated_y_tmp;
            rotated_y = sin_theta*rotated_x_tmp + cos_theta*rotated_y_tmp;
            rotated_x = rotated_x + center_x;
            rotated_y = rotated_y + center_y;
            returnImage[int(rotated_y)*subImgWidth+int(rotated_x)] = tmpImage[y][x];
        }
    }

    return returnImage;
}



/**
 * Implement GEO transformation
 */
void geoTransformation::methodGEO(){
    
    // Get corners
    vector<pair<int,int>> corners = getPossibleCorners(inputBuffer,imageHeight,imageWidth,513300000000);
    corners = locate3holes(inputBuffer,corners, imageHeight, imageWidth);
    
    // Sub 1
    vector<pair<int,int>> corners_1 = getPossibleCorners(subImage1,256,256,400000000000);
    corners_1 = limitFourCorners(corners_1);
    helper_print_points(corners_1);

    // Sub 2
    vector<pair<int,int>> corners_2 = getPossibleCorners(subImage2,256,256,200000000000);
    corners_2 = limitFourCorners(corners_2);
    // helper_print_points(corners_2);

    // Sub 3
    vector<pair<int,int>> corners_3 = getPossibleCorners(subImage3,256,256,1500000000000);
    corners_3 = limitFourCorners(corners_3);
    // helper_print_points(corners_3);
    
    // Get height & width & angle of each sub image
    // int subImage_1_height = sqrt(pow((corners_1[1].first-corners_1[2].first),2)+pow((corners_1[1].second-corners_1[2].second),2)); 
    // int subImage_1_width = sqrt(pow((corners_1[1].first-corners_1[3].first),2)+pow((corners_1[1].second-corners_1[3].second),2)); 
    // double angle_1 = atan((corners_1[1].second-corners_1[3].second)/(corners_1[1].first-corners_1[3].first));
 
    // int subImage_2_height = sqrt(pow((corners_2[1].first-corners_2[2].first),2)+pow((corners_2[1].second-corners_2[2].second+1),2)); 
    // int subImage_2_width = sqrt(pow((corners_2[1].first-corners_2[3].first),2)+pow((corners_2[1].second-corners_2[3].second),2)); 
    // double angle_2 = atan((double((corners_2[1].second-corners_2[3].second))/double(corners_2[1].first-corners_2[3].first)));

    // int subImage_3_height = sqrt(pow((corners_3[1].first-corners_3[2].first),2)+pow((corners_3[1].second-corners_3[2].second),2)); 
    // int subImage_3_width = sqrt(pow((corners_3[1].first-corners_3[3].first),2)+pow((corners_3[1].second-corners_3[3].second-1),2)); 
    // double angle_3 = atan((corners_3[1].second-corners_3[3].second)/(corners_3[1].first-corners_3[3].first-1));
    // cout << angle_1 << endl;
    // cout << subImage_1_height << " " << subImage_1_width << endl;
    // cout << angle_2 << endl;
    // cout << subImage_2_height << " " << subImage_2_width << endl;
    // cout << angle_3 << endl;
    // cout << subImage_3_height << " " << subImage_3_width << endl;

    unsigned char* tryOut = new unsigned char[256*256];
    double* outputImage = new double[256*256];
    outputImage = rotated_image(corners_1,subImage1,256,256);
    for(long i = 0; i < 256*256; i++){
        tryOut[i] = (unsigned char)outputImage[i];
    }
    FILE *file;
    file = fopen("try.raw","wb");
    if(file == NULL){
        cout << "Error opening file: try.raw" << endl;
        exit(EXIT_FAILURE);
    }else{
        fwrite(tryOut, sizeof(unsigned char), 256*256, file);
    }
    fclose(file);
    
    


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
    // possibleCorners map. key = R, value = coordinates
    int counter = 0;
    map<int,vector<double>> possibleCorners; 
    for(int i = 1; i < imageHeight-1; i++){
        for(int j = 1; j < imageWidth-1; j++){
            I_x_2 = 0;
            I_y_2 = 0;
            I_x_y = 0;
            for(int fi = -1; fi <= 1; fi++){
                for(int fj = -1; fj <= 1; fj++){
                    I_x_2 += gradient_x[(i+fi)*imageWidth+j+fj]*gradient_x[(i+fi)*imageWidth+j+fj];
                    I_y_2 += gradient_y[(i+fi)*imageWidth+j+fj]*gradient_y[(i+fi)*imageWidth+j+fj];
                    I_x_y += gradient_y[(i+fi)*imageWidth+j+fj]*gradient_x[(i+fi)*imageWidth+j+fj];
                }
            }
            M[0][0] = I_x_2;
            M[0][1] = I_x_y;
            M[1][0] = I_x_y;
            M[1][1] = I_y_2;
            determinant = M[0][0]*M[1][1] - (M[0][1]*M[1][0]);
            trace = M[0][0] + M[1][1];
            R = (determinant - 0.04*trace*trace);
            if(R > RThreshold){
                possibleCorners[counter].push_back(i);
                possibleCorners[counter].push_back(j);
                possibleCorners[counter].push_back(R);
                possibleCorners[counter].push_back(0.0);
                counter++;
            }
        }
    }
    // cout << "Finish locate the possible corner" << endl;

    // Non max suppression
    double currentX = 0;
    double currentY = 0;
    double currentR = 0;
    int nonMaxRadius = 10;
    int index = 0;
    
    // return vector
    vector<pair<int,int>> corners;
    // cout << "Start non-max suppresion" << endl;
    int positionInMap = 0;
    for(map<int,vector<double>>::iterator it = possibleCorners.begin(); it != possibleCorners.end(); ++it){
        if(it->second[3] == 1.0){
            positionInMap++;
            continue;
        }
        currentX = it->second[0];
        currentY = it->second[1];
        currentR = it->second[2];
        it->second[3] = 1.0;
        index = positionInMap;
        int internalPos = 0;
        for(map<int,vector<double>>::iterator internalIt = possibleCorners.begin(); internalIt != possibleCorners.end(); ++internalIt){
            if(internalIt->second[3] == 1.0){
                internalPos++;
                continue;
            }
            bool in_radius_x = abs(internalIt->second[0] - currentX) < nonMaxRadius;
            bool in_radius_y = abs(internalIt->second[1] - currentY) < nonMaxRadius;
            bool isGreaterThanCurrent = internalIt->second[2] > currentR;
            if(in_radius_x && in_radius_y){
                internalIt->second[3] = 1.0;
                if(isGreaterThanCurrent){
                    index = internalPos;
                    currentR = internalIt->second[2];
                }
            }
            internalPos++;
        }
        corners.push_back(make_pair(possibleCorners[index][1],possibleCorners[index][0]));
        positionInMap++;
    }
    return corners;
}

/**
 * This function is used to find the four corner
 */
vector<pair<int,int>> geoTransformation::limitFourCorners(vector<pair<int,int>> possibleCorners){
    vector<pair<int,int>> fourCorners;
    
    int min_x = 1024, min_y = 1024, max_x = 0, max_y = 0;
    int position_min_x, position_min_y, position_max_x, position_max_y = 0;
    for(int i = 0; i < possibleCorners.size(); i++){
        if(possibleCorners[i].first < min_x){
            min_x = possibleCorners[i].first;
            position_min_x = i;
        }
        if(possibleCorners[i].first > max_x){
            max_x = possibleCorners[i].first;
            position_max_x = i;
        }
        if(possibleCorners[i].second < min_y){
            min_y = possibleCorners[i].second;
            position_min_y = i;
        }
        if(possibleCorners[i].second > max_y){
            max_y = possibleCorners[i].second;
            position_max_y = i;
        }
    }
    fourCorners.push_back(make_pair(possibleCorners[position_min_x].first,possibleCorners[position_min_x].second));
    fourCorners.push_back(make_pair(possibleCorners[position_max_x].first,possibleCorners[position_max_x].second));
    fourCorners.push_back(make_pair(possibleCorners[position_min_y].first,possibleCorners[position_min_y].second));
    fourCorners.push_back(make_pair(possibleCorners[position_max_y].first,possibleCorners[position_max_y].second));
    
    return fourCorners;
}

/**
 * Find 3 holes in the main image (3 top left )
 */
vector<pair<int,int>> geoTransformation::locate3holes(unsigned char* image,vector<pair<int,int>> Corners, int imageHeight, int imageWidth){
    vector<pair<int,int>> leftCorners;
    string leftCornerCase = "111111111111111111111111111111";
    int* tmpImage = new int[imageHeight*imageWidth];
    for(long i = 0; i < imageHeight*imageWidth; i++){
        tmpImage[i] = ((double)image[i])/255;
    }

    for(int i = 0; i < Corners.size(); i++){
        string tmpString = "";
        int tmpY = Corners[i].first; 
        int tmpX = Corners[i].second; 
        if(tmpImage[tmpX*imageWidth+tmpY] == 1){
            for(int j = 0; j < 3; j++){
                for(int k = 0; k < 10; k++){
                    tmpString = tmpString + to_string(tmpImage[(tmpX+j)*imageWidth+tmpY+k]);
                    // cout << tmpImage[(tmpX+j)*imageWidth+tmpY+k] << endl;
                }
            }
            if(tmpString == leftCornerCase){
                leftCorners.push_back(make_pair(Corners[i].first,Corners[i].second));
            }
        }

    }
    return leftCorners;
}