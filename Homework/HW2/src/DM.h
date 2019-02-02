// Author: Zifan Wang
/**
 * This is the method of Dithering Matrix for half-tonning and is the inheritance from half_tonning_algorithm class
 * Dithering parameters are specified by an index matrix. The values in an index matrix indicate how likely
 * a dot will be turned on. For example, I(i,j) = [1,2;3,0], where 0 indicates the pixel most likely to be turned on, and 3 is the least likely one. 
 * This index matrix is a special case of a family of dithering matrices first introduced by Bayer [4].
 * Bayer index matrices are defined recursively using the formula:
 *                  I_2n(i,j) = [(4*I_n(i,j)+1, 4*I_n(i,j)+2);
 *                               (4*I_n(i,j)+3, 4*I_n(i,j))]
 * The index matrix can then be transformed into a threshold matrix T for an input gray-level image with
 * normalized pixel values (i.e. with its dynamic range between 0 and 255) by the following formula:
 *              T(x,y) = (I(x,y)+0.5)/N^2/*255
 * where N 6 denotes the number of pixels in the matrix
 * if 0 ≤ F i, j ≤ T(i modN, j modN) -> G(i,j) = 0
 * if T(i modN, j modN) < F i, j < 256 -> G(i,j) = 255
 */
#pragma once 

#include <string>
#include <iostream>
#include <cstdlib>
#include "half_tonning_algorithm.h"
#include "image.h"

using namespace std;

class DM : public half_tonning_algorithm{
public:
    // Dithering matrix method
    void methodDM(int indexNumber){
        // setting indexMatrix_16 
        set_indexMatrix_16();
        set_indexMatrix_32();
        
        unsigned char imageOut[imageHeight][imageWidth];
        if(indexNumber == 2){
            // 2 indexMatrix
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    if((double)input2DImage[i][j] <= (indexMatrix_2[i%2][j%2]+0.5)/2/2*255){
                        imageOut[i][j] = (unsigned char)0;
                    }
                    else{
                        imageOut[i][j] = (unsigned char)255;
                    }
                }
            }
        }
        else if(indexNumber == 8){
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    if((double)input2DImage[i][j] <= (indexMatrix_8[i%8][j%8]+0.5)/8/8*255){
                        imageOut[i][j] = (unsigned char)0;
                    }
                    else{
                        imageOut[i][j] = (unsigned char)255;
                    }
                }
            }
        }
        else if(indexNumber == 16){
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    if((double)input2DImage[i][j] <= (indexMatrix_16[i%16][j%16]+0.5)/16/16*255){
                        imageOut[i][j] = (unsigned char)0;
                    }
                    else{
                        imageOut[i][j] = (unsigned char)255;
                    }
                }
            }
        }
        else if(indexNumber = 32){
            for(int i = 0; i < imageHeight; i++){
                for(int j = 0; j < imageWidth; j++){
                    if((double)input2DImage[i][j] <= (indexMatrix_32[i%32][j%32]+0.5)/32/32*255){
                        imageOut[i][j] = (unsigned char)0;
                    }
                    else{
                        imageOut[i][j] = (unsigned char)255;
                    }
                }
            }
        }
        else{
            cout << "Input index number is not supported in dithering matrix method now." << endl;
            exit(EXIT_SUCCESS);
        }

        // set outputBuffer
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                outputBuffer[i*imageWidth+j] = imageOut[i][j];
            }
        }
    }



private:
    // index_2: index matrix
    double indexMatrix_2[2][2] = {{1.0,2.0},
                               {3.0,0.0}};  

    // index_8: index matrix
    double indexMatrix_8[8][8] = {{21.,37.,25.,41.,22.,38.,26.,42.},
                               {53.,5.,57.,9.,54.,6.,58.,10.},
                               {29.,45.,17.,33.,30.,46.,18.,34.},
                               {61.,13.,49.,1.,62.,14.,50.,2.},
                               {23.,39.,27.,43.,20.,36.,24.,40.},
                               {55.,7.,59.,11.,52.,4.,56.,8.},
                               {31.,47.,19.,35.,28.,44.,16.,32.},
                               {63.,15.,51.,3.,60.,12.,48.,0.}};

    // index_16: index matrix
    double indexMatrix_16[16][16];
                                      
    // index_32: index matrix
    double indexMatrix_32[32][32];

    // Function to calculate 16 index_matrix
    void set_indexMatrix_16(){
        for(int i = 0; i < 16; i++){
            for(int j = 0; j < 16; j++){
                if(i < 8 && j < 8)      {indexMatrix_16[i][j] = indexMatrix_8[i][j]*4 + 1;}
                if(i < 8 && j >= 8)     {indexMatrix_16[i][j] = indexMatrix_8[i][j-8]*4 + 2;}
                if(i >= 8 && j < 8)     {indexMatrix_16[i][j] = indexMatrix_8[i-8][j]*4 + 3;}
                if(i >= 8 && j >= 8)    {indexMatrix_16[i][j] = indexMatrix_8[i-8][j-8]*4;}
            }
        }
    }

    // Function to calculate 32 index_matrix
    void set_indexMatrix_32(){
        for(int i = 0; i < 32; i++){
            for(int j = 0; j < 32; j++){
                if(i < 16 && j < 16)      {indexMatrix_32[i][j] = indexMatrix_16[i][j]*4 + 1;}
                if(i < 16 && j >= 16)     {indexMatrix_32[i][j] = indexMatrix_16[i][j-16]*4 + 2;}
                if(i >= 16 && j < 16)     {indexMatrix_32[i][j] = indexMatrix_16[i-16][j]*4 + 3;}
                if(i >= 16 && j >= 16)    {indexMatrix_32[i][j] = indexMatrix_16[i-16][j-16]*4;}
            }
        }
    }

};   