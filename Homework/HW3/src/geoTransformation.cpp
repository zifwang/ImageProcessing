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
#define INT_MAX 2147483647


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

/**
 * Image rotation
 */ 
double* geoTransformation::bilinearInterpolation_rotation(double* img, int imgHeight, int imgWidth){
    double pixel_above, pixel_below, pixel_left, pixel_right;
    double* modImage = new double[imgHeight*imgWidth];
    for(int i = 0; i < imgHeight; i++){
        for(int j = 0; j < imgWidth; j++){
            modImage[(i)*imgWidth+j] = 255;
        }
    }
    
    for(int i = 1; i < imgHeight-1; i++){
        for(int j = 1; j < imgWidth-1; j++){
            // get four sounring pixels
            if(img[(i)*imgWidth+j] == 255){
                pixel_above = img[(i-1)*imgWidth+j];
                pixel_below = img[(i+1)*imgWidth+j];
                pixel_left = img[(i)*imgWidth+j-1];
                pixel_right = img[(i)*imgWidth+j+1];
                if(pixel_above != 255 && pixel_below != 255 && pixel_left != 255 && pixel_right != 255){
                    modImage[(i)*imgWidth+j] = (double(pixel_above+pixel_below+pixel_left+pixel_right))/4;
                    // if(i == 98 && j == 101){
                    //     cout << pixel_above << " " << pixel_below << " " << pixel_left << " " << pixel_right << endl;
                    //     cout << modImage[(i)*imgWidth+j];
                    // }
                }
                else{
                    modImage[(i)*imgWidth+j] = img[(i)*imgWidth+j];
                }
            }
            else{
                modImage[(i)*imgWidth+j] = img[(i)*imgWidth+j];
            }
        }
    }

    return modImage;
}

double* geoTransformation::rotated_image_unsigned(vector<pair<int,int>> corners, unsigned char* subImage, int subImgHeight, int subImgWidth, double angle){
    // Get height & width & angle of each sub image
    // int true_height = sqrt(pow((corners[1].first-corners[2].first),2)+pow((corners[1].second-corners[2].second),2)); 
    // int true_width = sqrt(pow((corners[1].first-corners[3].first),2)+pow((corners[1].second-corners[3].second),2)); 
    // double angle = atan(double(corners[1].second-corners[3].second)/double(corners[1].first-corners[3].first));
    
    int height_min = corners[2].second;
    int height_max = corners[3].second;
    int width_min = corners[0].first;
    int width_max = corners[1].first;
    // cout << height_min << " " << height_max << " " << width_min << " " << width_max << endl;
    int center_x = (width_max+width_min)/2;
    int center_y = (height_max+height_min)/2;
    // cout << center_x << " "<< center_y << endl;
    // cout << width_min << " " << width_max << " " << height_min << " " << height_max << endl;
    
    double cos_theta = cos(-angle);
    double sin_theta = sin(-angle);
    // cout << -angle << " " << cos_theta << " " << sin_theta << endl;
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
            // cout << y << " " << x << endl;
            rotated_x = x - center_x;
            rotated_y = y - center_y;
            double rotated_x_tmp = rotated_x;
            double rotated_y_tmp = rotated_y;
            rotated_x = cos_theta*rotated_x_tmp - sin_theta*rotated_y_tmp;
            rotated_y = sin_theta*rotated_x_tmp + cos_theta*rotated_y_tmp;
            rotated_x = rotated_x + center_x;
            rotated_y = rotated_y + center_y;
            // cout << rotated_y << " " << rotated_x << endl;
            if(rotated_x > 0 && rotated_y > 0 && rotated_x < subImgWidth && rotated_y < subImgHeight){
                returnImage[int(rotated_y)*subImgWidth+int(rotated_x)] = tmpImage[y][x];
            }
        }
    }

    return returnImage;
}

double* geoTransformation::rotated_image_90(vector<pair<int,int>> corners, double* subImage, int subImgHeight, int subImgWidth){
    // Get height & width & angle of each sub image
    // int true_height = sqrt(pow((corners[1].first-corners[2].first),2)+pow((corners[1].second-corners[2].second),2)); 
    // int true_width = sqrt(pow((corners[1].first-corners[3].first),2)+pow((corners[1].second-corners[3].second),2)); 
    // double angle = atan(double(corners[1].second-corners[3].second)/double(corners[1].first-corners[3].first));
    // find height_min & height_max & width_min & width_max
    int height_min = subImgHeight;
    int height_max = 0;
    int width_min = subImgWidth;
    int width_max = 0;
    for(int i = 0; i < corners.size(); i++){
        if(corners[i].second > height_max){
            height_max = corners[i].second;
        }
        if(corners[i].second < height_min){
            height_min = corners[i].second;
        }
        if(corners[i].first > width_max){
            width_max = corners[i].first;
        }
        if(corners[i].first < width_min){
            width_min = corners[i].first;
        }
    }

    // cout << height_min << " " << height_max << " " << width_min << " " << width_max << endl;
    int center_x = (width_max+width_min)/2;
    int center_y = (height_max+height_min)/2;
    // cout << center_x << " "<< center_y << endl;
    // cout << width_min << " " << width_max << " " << height_min << " " << height_max << endl;
    
    double cos_theta = 0.0;
    double sin_theta = 1;
    // cout << -angle << " " << cos_theta << " " << sin_theta << endl;
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
            // cout << y << " " << x << endl;
            rotated_x = x - center_x;
            rotated_y = y - center_y;
            double rotated_x_tmp = rotated_x;
            double rotated_y_tmp = rotated_y;
            rotated_x = cos_theta*rotated_x_tmp - sin_theta*rotated_y_tmp;
            rotated_y = sin_theta*rotated_x_tmp + cos_theta*rotated_y_tmp;
            rotated_x = rotated_x + center_x;
            rotated_y = rotated_y + center_y;
            // cout << rotated_y << " " << rotated_x << endl;
            if(rotated_x > 0 && rotated_y > 0 && rotated_x < subImgWidth && rotated_y < subImgHeight){
                returnImage[int(rotated_y)*subImgWidth+int(rotated_x)] = tmpImage[y][x];
            }
        }
    }

    return returnImage;
}

/**
 * Image Scaling
 */ 
double* geoTransformation::scaling_image_doubled(vector<pair<int,int>> &corners, double* subImage, int subImgHeight, int subImgWidth, int targetSize){
    int height_min = subImgHeight;
    int height_max = 0;
    int width_min = subImgWidth;
    int width_max = 0;
    for(int i = 0; i < corners.size(); i++){
        // cout << "ORI: " << corners[i].first << " " << corners[i].second << endl;
        if(corners[i].second > height_max){
            height_max = corners[i].second;
        }
        if(corners[i].second < height_min){
            height_min = corners[i].second;
        }
        if(corners[i].first > width_max){
            width_max = corners[i].first;
        }
        if(corners[i].first < width_min){
            width_min = corners[i].first;
        }
    }
    int sub_imageWidth = width_max - width_min;
    int sub_imageHeight = height_max - height_min;
    // scaling parameter
    double scale = double(targetSize)/sub_imageHeight;
    double ratio = double(1)/scale;
    // cout << ratio << endl;

    double* scaled_image = new double[int(subImgHeight*scale)*int(subImgWidth*scale)];
    // cout << int(subImgHeight*scale) << endl;
    // cout << int(subImgWidth*scale)<< endl;
    // set back_ground to 255
    for(int i = 0; i < int(subImgHeight*scale); i++){
        for(int j = 0; j < int(subImgWidth*scale); j++){
            scaled_image[i*int(subImgWidth*scale)+j] = 255;
        }
    }
    
    double est_x, est_y;
    int lt_x, lt_y;
    double scale_x, scale_y;
    bool is_vec_updated[4] = {false,false,false,false};

    // update corner
    for(int k = 0; k < corners.size(); k++){
        // cout << "ORI: "<< corners[k].first << " " << corners[k].second << endl;
        corners[k].first = int(double(corners[k].first)*scale);
        corners[k].second = int(double(corners[k].second)*scale);
        // cout << "MOD: "<< corners[k].first << " " << corners[k].second << endl;
    }

    // Scaling
    for(int i = 0; i < int(subImgHeight*scale); i++){
        for(int j = 0; j < int(subImgWidth*scale); j++){
            est_y = ratio*double(i);
            est_x = ratio*double(j);

            lt_y = int(est_y);
            lt_x = int(est_x);
            
            scale_y = est_y - double(lt_y);
            scale_x = est_x - double(lt_x);

            double val = (1.0 - scale_x)*(1-scale_y)*subImage[lt_y*subImgWidth+lt_x]
                         + (1.0 - scale_x)*scale_y*subImage[(lt_y+1)*subImgWidth+lt_x]
                         + scale_x*scale_y*subImage[(lt_y+1)*subImgWidth+lt_x+1]
                         + scale_x*(1.0-scale_y)*subImage[(lt_y)*subImgWidth+lt_x+1];

            scaled_image[i*int(subImgWidth*scale)+j] = val;
        }
    }

    return scaled_image;
}


/**
 * Corner coordinates update
 */  
vector<pair<int,int>> geoTransformation::update_corners(vector<pair<int,int>> corners,double angle){
    vector<pair<int,int>> new_corners;
    int height_min = corners[2].second;
    int height_max = corners[3].second;
    int width_min = corners[0].first;
    int width_max = corners[1].first;
    int center_x = (width_max+width_min)/2;
    int center_y = (height_max+height_min)/2;
    double rotated_x;
    double rotated_y;
    double cos_theta = cos(-angle);
    double sin_theta = sin(-angle);
    for(int i = 0; i < corners.size(); i++){
        rotated_x = corners[i].first - center_x;
        rotated_y = corners[i].second - center_y;
        double rotated_x_tmp = rotated_x;
        double rotated_y_tmp = rotated_y;
        rotated_x = cos_theta*rotated_x_tmp - sin_theta*rotated_y_tmp;
        rotated_y = sin_theta*rotated_x_tmp + cos_theta*rotated_y_tmp;
        rotated_x = rotated_x + center_x;
        rotated_y = rotated_y + center_y;
        new_corners.push_back(make_pair(rotated_x,rotated_y));
    }
    return new_corners;

}
vector<pair<int,int>> geoTransformation::update_corners_90(vector<pair<int,int>> corners){
    vector<pair<int,int>> new_corners;
    int height_min = INT_MAX;
    int height_max = 0;
    int width_min = INT_MAX;
    int width_max = 0;
    for(int i = 0; i < corners.size(); i++){
        if(corners[i].second > height_max){
            height_max = corners[i].second;
        }
        if(corners[i].second < height_min){
            height_min = corners[i].second;
        }
        if(corners[i].first > width_max){
            width_max = corners[i].first;
        }
        if(corners[i].first < width_min){
            width_min = corners[i].first;
        }
    }
    int center_x = (width_max+width_min)/2;
    int center_y = (height_max+height_min)/2;
    double rotated_x;
    double rotated_y;
    double cos_theta = 0;
    double sin_theta = 1;
    for(int i = 0; i < corners.size(); i++){
        rotated_x = corners[i].first - center_x;
        rotated_y = corners[i].second - center_y;
        double rotated_x_tmp = rotated_x;
        double rotated_y_tmp = rotated_y;
        rotated_x = cos_theta*rotated_x_tmp - sin_theta*rotated_y_tmp;
        rotated_y = sin_theta*rotated_x_tmp + cos_theta*rotated_y_tmp;
        rotated_x = rotated_x + center_x;
        rotated_y = rotated_y + center_y;
        new_corners.push_back(make_pair(rotated_x,rotated_y));
    }
    return new_corners;

}


/**
 * Implement GEO transformation
 */
void geoTransformation::methodGEO(){
    
    // Get corners
    vector<pair<int,int>> corners = getPossibleCorners(inputBuffer,imageHeight,imageWidth,513300000000);
    corners = locate3holes(inputBuffer,corners, imageHeight, imageWidth);
    pair<int,int> corner_1 = corners[1];
    pair<int,int> corner_2 = corners[0];
    pair<int,int> corner_3 = corners[2];

    // Get a tmp ori_image;
    double* ori_image = new double[imageHeight*imageWidth];
    for(long i = 0; i < imageHeight*imageWidth; i++){
        ori_image[i] = (double)inputBuffer[i];
    }

    
    // Sub 1
    vector<pair<int,int>> corners_1 = getPossibleCorners(subImage1,256,256,400000000000);
    corners_1 = limitFourCorners(corners_1);
    // Sub 2
    vector<pair<int,int>> corners_2 = getPossibleCorners(subImage2,256,256,200000000000);
    corners_2 = limitFourCorners(corners_2);
    // Sub 3
    vector<pair<int,int>> corners_3 = getPossibleCorners(subImage3,256,256,1500000000000);
    corners_3 = limitFourCorners(corners_3);

    // Get height & width & angle of each sub image
    double angle_1 = atan(double(corners_1[1].second-corners_1[3].second)/double(corners_1[1].first-corners_1[3].first));
    double angle_2 = atan((double((corners_2[1].second-corners_2[3].second))/double(corners_2[1].first-corners_2[3].first)));
    double angle_3 = atan((corners_3[1].second-corners_3[3].second)/(corners_3[1].first-corners_3[3].first-1))-0.03;
    
    // Rotation
    // 1st
    double* rotated_sub_img_1 = new double[256*256];
    rotated_sub_img_1 = rotated_image_unsigned(corners_1,subImage1,256,256,angle_1);
    vector<pair<int,int>> corners_1_update = update_corners(corners_1,angle_1);
    rotated_sub_img_1 = bilinearInterpolation_rotation(rotated_sub_img_1,256,256);
    // scaling
    int sub_1_width = mod_sub_image_width(corners_1_update,256,256,160);
    int sub_1_height = mod_sub_image_height(corners_1_update,256,256,160);
    double* scaled_sub_img_1 = scaling_image_doubled(corners_1_update, rotated_sub_img_1, 256, 256, 160);
    delete[] rotated_sub_img_1;
    int sub_1_tl_x = find_x_min(corners_1_update);
    int sub_1_tl_y = find_y_min(corners_1_update);
    // Translation
    for(int i = 0; i < 160; i++){
        for(int j = 0; j < 160; j++){
            ori_image[(corner_1.second+i)*imageWidth+j+corner_1.first] = scaled_sub_img_1[(i+sub_1_tl_y+2)*sub_1_width+j+sub_1_tl_x+1];
        }
    }
    

    // 2st
    double* rotated_sub_img_2 = new double[256*256];
    rotated_sub_img_2 = rotated_image_unsigned(corners_2,subImage2,256,256,angle_2);
    rotated_sub_img_2 = bilinearInterpolation_rotation(rotated_sub_img_2,256,256);
    vector<pair<int,int>> corners_2_update = update_corners(corners_2,angle_2);
    rotated_sub_img_2 = rotated_image_90(corners_2_update,rotated_sub_img_2,256,256);
    corners_2_update = update_corners_90(corners_2_update);
    // scaling
    int sub_2_width = mod_sub_image_width(corners_2_update,256,256,160);
    int sub_2_height = mod_sub_image_height(corners_2_update,256,256,160);
    double* scaled_sub_img_2 = scaling_image_doubled(corners_2_update, rotated_sub_img_2, 256, 256, 160);
    delete[] rotated_sub_img_2;
    int sub_2_tl_x = find_x_min(corners_2_update);
    int sub_2_tl_y = find_y_min(corners_2_update);
    // Translation
    for(int i = 0; i < 160; i++){
        for(int j = 0; j < 160; j++){
            ori_image[(corner_2.second+i)*imageWidth+j+corner_2.first] = scaled_sub_img_2[(i+sub_2_tl_y+1)*sub_2_width+j+sub_2_tl_x+1];
        }
    }
    

    // 3rd
    double* rotated_sub_img_3 = new double[256*256];
    rotated_sub_img_3 = rotated_image_unsigned(corners_3,subImage3,256,256,angle_3);
    vector<pair<int,int>> corners_3_update = update_corners(corners_3,angle_3);
    rotated_sub_img_3 = bilinearInterpolation_rotation(rotated_sub_img_3,256,256);
    rotated_sub_img_3 = rotated_image_90(corners_3_update,rotated_sub_img_3,256,256);
    corners_3_update = update_corners_90(corners_3_update);
    rotated_sub_img_3 = rotated_image_90(corners_3_update,rotated_sub_img_3,256,256);
    corners_3_update = update_corners_90(corners_3_update);
    rotated_sub_img_3 = rotated_image_90(corners_3_update,rotated_sub_img_3,256,256);
    corners_3_update = update_corners_90(corners_3_update);
    // Scaling
    int sub_3_width = mod_sub_image_width(corners_3_update,256,256,160);
    int sub_3_height = mod_sub_image_height(corners_3_update,256,256,160);
    double* scaled_sub_img_3 = scaling_image_doubled(corners_3_update, rotated_sub_img_3, 256, 256, 160);
    delete[] rotated_sub_img_3;
    int sub_3_tl_x = find_x_min(corners_3_update);
    int sub_3_tl_y = find_y_min(corners_3_update);
    // Translation
    for(int i = 0; i < 160; i++){
        for(int j = 0; j < 160; j++){
            ori_image[(corner_3.second+i)*imageWidth+j+corner_3.first] = scaled_sub_img_3[(i+sub_3_tl_y+1)*sub_3_width+j+sub_3_tl_x];
        }
    }
    



    unsigned char* buffer = new unsigned char[512*512];
    for(long i = 0; i < 512*512; i++){
        buffer[i] = ori_image[i];
    }
    FILE *file;
    file = fopen("try.raw","wb");
    if(file == NULL){
        cout << "Error opening file: try.raw" << endl;
        exit(EXIT_FAILURE);
    }else{
        fwrite(buffer, sizeof(unsigned char), 512*512, file);
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

int geoTransformation::height_finder(vector<pair<int,int>> corners){
    int height_min = INT_MAX;
    int height_max = 0;
    int width_min = INT_MAX;
    int width_max = 0;
    for(int i = 0; i < corners.size(); i++){
        if(corners[i].second > height_max){
            height_max = corners[i].second;
        }
        if(corners[i].second < height_min){
            height_min = corners[i].second;
        }
        if(corners[i].first > width_max){
            width_max = corners[i].first;
        }
        if(corners[i].first < width_min){
            width_min = corners[i].first;
        }
    }
    return height_max-height_min;
}

int geoTransformation::width_finder(vector<pair<int,int>> corners){
    int height_min = INT_MAX;
    int height_max = 0;
    int width_min = INT_MAX;
    int width_max = 0;
    for(int i = 0; i < corners.size(); i++){
        if(corners[i].second > height_max){
            height_max = corners[i].second;
        }
        if(corners[i].second < height_min){
            height_min = corners[i].second;
        }
        if(corners[i].first > width_max){
            width_max = corners[i].first;
        }
        if(corners[i].first < width_min){
            width_min = corners[i].first;
        }
    }
    return width_max-width_min;
}

int geoTransformation::find_x_min(vector<pair<int,int>> corners){
    int height_min = INT_MAX;
    int height_max = 0;
    int width_min = INT_MAX;
    int width_max = 0;
    for(int i = 0; i < corners.size(); i++){
        if(corners[i].second > height_max){
            height_max = corners[i].second;
        }
        if(corners[i].second < height_min){
            height_min = corners[i].second;
        }
        if(corners[i].first > width_max){
            width_max = corners[i].first;
        }
        if(corners[i].first < width_min){
            width_min = corners[i].first;
        }
    }
    return width_min;
}

int geoTransformation::find_y_min(vector<pair<int,int>> corners){
    int height_min = INT_MAX;
    int height_max = 0;
    int width_min = INT_MAX;
    int width_max = 0;
    for(int i = 0; i < corners.size(); i++){
        if(corners[i].second > height_max){
            height_max = corners[i].second;
        }
        if(corners[i].second < height_min){
            height_min = corners[i].second;
        }
        if(corners[i].first > width_max){
            width_max = corners[i].first;
        }
        if(corners[i].first < width_min){
            width_min = corners[i].first;
        }
    }
    return height_min;
}