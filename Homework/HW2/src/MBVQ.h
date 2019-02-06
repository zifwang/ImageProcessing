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
        unsigned char imageOut[imageHeight][imageWidth][BytesPerPixel];

        double tmpMBVQImage[imageHeight][imageWidth][BytesPerPixel];
        double tmpVertexImage[imageHeight][imageWidth][BytesPerPixel];
        double tmpImage[imageHeight][imageWidth][BytesPerPixel];
        // set all tmp images.
        for(int i = 0; i < imageHeight; i++){
            for(int j = 0; j < imageWidth; j++){
                for(int color = 0; color < BytesPerPixel; color++){
                    /**
                    * Read image by oneD array: first by row, then col, last color
                    * in the format: Row: 0, Col: 0, Color: 0 <- first element in 1D array
                    *                Row: 0, Col: 0, Color: 1 <- Second element
                    *                Row: 0, Col: 0, Color: 2 <- third element
                    *                Row: 0, Col: 1, Color: 0 <- fourth element
                    *                Row: 0, Col: 1, Color: 1 <- fifth element
                    *                Row: 0, Col: 1, Color: 2 <- sixth element
                    */
                    tmpMBVQImage[i][j][color] = double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color]);
                    tmpVertexImage[i][j][color] = double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color])/255;
                    tmpImage[i][j][color] = 1 - double(inputBuffer[i*imageWidth*BytesPerPixel+j*BytesPerPixel+color])/255;
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
                if(blue <= red){
                    if(blue <= green){
                        vertex = "yellow";
                    }
                }
            }
            if(green < 0.5){
                if(green <= blue){
                    if(green <= red){
                        vertex = "magenta";
                    }
                }
            }
            if(red < 0.5){
                if(red <= blue){
                    if(red <= green){
                        vertex = "cyan";
                    }
                }
            }
        }
        // 2.MYGC
        else if(mbvq == "MYGC"){
            vertex = "magenta";
            if(green >= blue){
                if(red >= blue){
                    if(red >= 0.5){
                        vertex = "yellow";
                    }else{
                        vertex = "green";
                    }
                }
            }
            if(green >= red){
                if(blue >= red){
                    if(blue >= 0.5){
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
                    if(blue >= green){
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
                if(red >= 0.5){
                    if(green >= 0.5){
                        vertex = "yellow";
                    }else{
                        vertex = "red";
                    }
                }else{
                    if(red >= green){
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
                if(blue >= red){
                    if(blue >= green){
                        vertex = "blue";
                    }
                }
            }
            if(green > 0.5){
                if(green >= blue){
                    if(green >= red){
                        vertex = "green";
                    }
                }
            }
            if(red > 0.5){
                if(red >= blue){
                    if(red >= green){
                        vertex = "red";
                    }
                }
            }
        }
        // 5.RGBM
        else if(mbvq == "RGBM"){
            vertex = "green";
            if(red > green){
                if(red >= blue){
                    if(blue < 0.5){
                        vertex = "red";
                    }else{
                        vertex = "magenta";
                    }
                }
            }
            if(blue > green){
                if(blue >= red){
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
                    if(green >= red){
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
                    if(red - green + blue >= 0.5){
                        vertex = "magenta";
                    }else{
                        vertex = "green";
                    }
                }else{
                    if(green >= blue){
                        vertex = "green";
                    }else{
                        vertex = "blue";
                    }
                }
            }
        }
        return vertex;
    }





















}
