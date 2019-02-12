#pragma once

#include "image.h"
#include "half_tonning_algorithm.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>

using namespace std;

class MBVQ : public half_tonning_algorithm{
public:
    void methodMVQB(){
        double tmpImage[imageHeight][imageWidth][BytesPerPixel];
        double* error = new double[imageSize];
        unsigned char imageOut[imageHeight][imageWidth][BytesPerPixel];   

        // set all tmp images.
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                for(int color = 0; color < BytesPerPixel; color++){
                    // cout << i << " " << j << " "<< color << " : ";
                    /**
                    * Read image by oneD array: first by row, then col, last color
                    * in the format: Row: 0, Col: 0, Color: 0 <- first element in 1D array
                    *                Row: 0, Col: 0, Color: 1 <- Second element
                    *                Row: 0, Col: 0, Color: 2 <- third element
                    *                Row: 0, Col: 1, Color: 0 <- fourth element
                    *                Row: 0, Col: 1, Color: 1 <- fifth element
                    *                Row: 0, Col: 1, Color: 2 <- sixth element
                    */
                    tmpImage[i][j][color] = double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color])/255;
                    error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color] = 0.0;
                }
            }
        }

        for(int i = 0; i < imageHeight; i++){
            if(i%2 == 0){
                for(int j = 0; j < imageWidth; j++){
                    // Get rgb value by tmpImage + error
                    double r = double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+0]);
                    double g = double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+1]);
                    double b = double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+2]);
    
                    // determine the mbvq in image (i,j) based on image(i,j)
                    string mbvq = whichMBVQ(r,g,b);
                    
                    r = tmpImage[i][j][0] + error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+0];
                    g = tmpImage[i][j][1] + error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+1];
                    b = tmpImage[i][j][2] + error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+2];

                    // determine the closet vertex based on image(i,j) + error, and mbvq
                    string vertex = whichVertex(mbvq,r,g,b);
                    // determine the closet color by vertex
                    int* determinedColor = whichColor(vertex);

                    for(int color = 0; color < BytesPerPixel; color++){
                        // set color to the ouput image
                        imageOut[i][j][color] = (unsigned char) determinedColor[color];
                        // compute the error
                        error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color] = tmpImage[i][j][color] + error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color] - determinedColor[color];
                        // error[i][j][color] = tmpImage[i][j][color] + error[i][j][color] - determinedColor[color];
                        // // Distribute error to nearby location based on steinberg algorithm
                        if(j+1 < imageWidth){
                            error[i*imageWidth*BytesPerPixel+(j+1)*BytesPerPixel+color] += error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color]*7/16;
                        }
                        if(j-1 >= 0 && i+1 < imageHeight){
                            error[(i+1)*imageWidth*BytesPerPixel+(j-1)*BytesPerPixel+color] += error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color]*3/16;
                        }
                        if(i+1 < imageHeight){
                            error[(i+1)*imageWidth*BytesPerPixel+(j)*BytesPerPixel+color] += error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color]*5/16;
                        } 
                        if(i+1 < imageHeight && j+1 < imageWidth){
                            error[(i+1)*imageWidth*BytesPerPixel+(j+1)*BytesPerPixel+color] += error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color]*1/16;
                        }
                    }
                }
            }
            else{
                for(int j = imageWidth-1; j >= 0; j--){
                    // Get rgb value by tmpImage + error
                    double r = double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+0]);
                    double g = double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+1]);
                    double b = double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+2]);

                    // determine the mbvq in image (i,j) based on image(i,j) + error
                    string mbvq = whichMBVQ(r,g,b);
                    
                    // determine the closet vertex based on image(i,j) + error, and mbvq
                    r = tmpImage[i][j][0] + error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+0];
                    g = tmpImage[i][j][1] + error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+1];
                    b = tmpImage[i][j][2] + error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+2];

                    // determine the closet vertex based on image(i,j) + error, and mbvq
                    string vertex = whichVertex(mbvq,r,g,b);

                    // determine the closet color by vertex
                    int* determinedColor = whichColor(vertex);

                    for(int color = 0; color < BytesPerPixel; color++){
                        // set color to the ouput image
                        imageOut[i][j][color] = (unsigned char) determinedColor[color];
                        // compute the error
                        error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color] = tmpImage[i][j][color] + error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color] - determinedColor[color];
                        // Distribute error to nearby location based on steinberg algorithm
                        if(j > 0){
                            error[i*imageWidth*BytesPerPixel+(j-1)*BytesPerPixel+color] += error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color]*7/16;
                        }
                        if(j > 0 && i+1 < imageHeight){
                            error[(i+1)*imageWidth*BytesPerPixel+(j-1)*BytesPerPixel+color] += error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color]*1/16;
                        } 
                        if(i+1 < imageHeight){
                            error[(i+1)*imageWidth*BytesPerPixel+(j)*BytesPerPixel+color] += error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color]*5/16;
                        }
                        if(j < imageWidth-1 && i+1 < imageHeight){
                            error[(i+1)*imageWidth*BytesPerPixel+(j+1)*BytesPerPixel+color] += error[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color]*3/16;
                        } 
                    }
                }
            }
        }

        
        // set outputBuffer
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                for(int color = 0; color < BytesPerPixel; color++){
                    outputBuffer[i*BytesPerPixel*imageWidth+j*BytesPerPixel+color] = (unsigned char) double(imageOut[i][j][color])*255;
                }
            }
        }
    }

private:

    // Determine which MBVQ to use
    string whichMBVQ(double red, double green, double blue){
        if((red+green) > 255){
            if((green+blue) > 255){
                if((red+green+blue) > 510) return "CMYW";
                else return "MYGC";
            }
            else return "RGMY";
        }
        else{
            if(!((green+blue) > 255)){
                if(!((red+green+blue) > 255)) return "KRGB";
                else return "RGBM";
            }
            else return "CMGB";
        }
    }

    // Determine to closet vertex
    string whichVertex(string mbvq, double red, double green, double blue){
        string vertex;
        // 1.CMYW
        if(mbvq == "CMYW"){
            vertex = "white";
            if(blue < 0.5){
                if(blue < red){
                    if(blue < green){
                        vertex = "yellow";
                    }
                }
            }
            if(green < 0.5){
                if(green < blue){
                    if(green < red){
                        vertex = "magenta";
                    }
                }
            }
            if(red < 0.5){
                if(red < blue){
                    if(red < green){
                        vertex = "cyan";
                    }
                }
            }
        }
        // 2.MYGC
        else if(mbvq == "MYGC"){
            vertex = "magenta";
            if(green > blue){
                if(red > blue){
                    if(red > 0.5){
                        vertex = "yellow";
                    }else{
                        vertex = "green";
                    }
                }
            }
            if(green > red){
                if(blue > red){
                    if(blue > 0.5){
                        vertex = "cyan";
                    }else{
                        vertex = "green";
                    }
                }
            }
        }
        // 3.RGMY
        else if(mbvq == "RGMY"){
            if(blue > 0.5){
                if(red > 0.5){
                    if(blue > green){
                        vertex = "magenta";
                    }else{
                        vertex = "yellow";
                    }
                }else{
                    if(green > blue + red){
                        vertex = "green";
                    }else{
                        vertex = "magenta";
                    }
                }
            }else{
                if(red > 0.5){
                    if(green > 0.5){
                        vertex = "yellow";
                    }else{
                        vertex = "red";
                    }
                }else{
                    if(red > green){
                        vertex = "red";
                    }else{
                        vertex = "green";
                    }
                }
            }
        }
        // 4.KRGB
        else if(mbvq == "KRGB"){
            vertex = "black";
            if(blue > 0.5){
                if(blue > red){
                    if(blue > green){
                        vertex = "blue";
                    }
                }
            }
            if(green > 0.5){
                if(green > blue){
                    if(green >= red){
                        vertex = "green";
                    }
                }
            }
            if(red > 0.5){
                if(red > blue){
                    if(red > green){
                        vertex = "red";
                    }
                }
            }
        }
        // 5.RGBM
        else if(mbvq == "RGBM"){
            vertex = "green";
            if(red > green){
                if(red > blue){
                    if(blue < 0.5){
                        vertex = "red";
                    }else{
                        vertex = "magenta";
                    }
                }
            }
            if(blue > green){
                if(blue > red){
                    if(red < 0.5){
                        vertex = "blue";
                    }else{
                        vertex = "magenta";
                    }
                }
            }
        }
        // 6.CMGB
        else if(mbvq == "CMGB"){
            if(blue > 0.5){
                if(red > 0.5){
                    if(green > red){
                        vertex = "cyan";
                    }else{
                        vertex = "magenta";
                    }
                }else{
                    if(green > 0.5){
                        vertex = "cyan";
                    }else{
                        vertex = "blue";
                    }
                }
            }else{
                if(red > 0.5){
                    if(red - green + blue > 0.5){
                        vertex = "magenta";
                    }else{
                        vertex = "green";
                    }
                }else{
                    if(green > blue){
                        vertex = "green";
                    }else{
                        vertex = "blue";
                    }
                }
            }
        }
        return vertex;
    }

    // // Define color;
    // int* whichColor(string vertex){
    //     static int color[3];
    //     if(vertex == "white"){
    //         color[0] = 0;
    //         color[1] = 0;
    //         color[2] = 0;
    //     }
    //     if(vertex == "black"){
    //         color[0] = 1;
    //         color[1] = 1;
    //         color[2] = 1;
    //     }
    //     if(vertex == "yellow"){
    //         color[0] = 0;
    //         color[1] = 0;
    //         color[2] = 1;
    //     }
    //     if(vertex == "cyan"){
    //         color[0] = 1;
    //         color[1] = 0;
    //         color[2] = 0;
    //     }
    //     if(vertex == "magenta"){
    //         color[0] = 0;
    //         color[1] = 1;
    //         color[2] = 0;
    //     }
    //     if(vertex == "green"){
    //         color[0] = 1;
    //         color[1] = 0;
    //         color[2] = 1;
    //     }
    //     if(vertex == "red"){
    //         color[0] = 0;
    //         color[1] = 1;
    //         color[2] = 1;
    //     }
    //     if(vertex == "blue"){
    //         color[0] = 1;
    //         color[1] = 1;
    //         color[2] = 0;
    //     }
    //     return color;
    // }
    int* whichColor(string vertex){
        static int color[3];
        if(vertex == "white"){
            color[0] = 1;
            color[1] = 1;
            color[2] = 1;
        }
        if(vertex == "black"){
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        }
        if(vertex == "yellow"){
            color[0] = 1;
            color[1] = 1;
            color[2] = 0;
        }
        if(vertex == "cyan"){
            color[0] = 0;
            color[1] = 1;
            color[2] = 1;
        }
        if(vertex == "magenta"){
            color[0] = 1;
            color[1] = 0;
            color[2] = 1;
        }
        if(vertex == "green"){
            color[0] = 0;
            color[1] = 1;
            color[2] = 0;
        }
        if(vertex == "red"){
            color[0] = 1;
            color[1] = 0;
            color[2] = 0;
        }
        if(vertex == "blue"){
            color[0] = 0;
            color[1] = 0;
            color[2] = 1;
        }
        return color;
    }
};
