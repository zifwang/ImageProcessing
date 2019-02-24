// Author: Zifan Wang
/**
 * Implement the shrinking algorithm.
 * 
 * 
*/

#pragma once 

#include <string>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include "morphology.h"
#include "image.h"

using namespace std;

class defectDetect : public morphology{
public:
    void detection(){
        // Declare a tmpImage pointer 
        int* tmpImage = getOriImage_0_1();

        // Find defects
        for(int i = 1; i < imageHeight-1; i++){
            for(int j = 1; j < imageWidth-1; j++){
                X = tmpImage[i*imageWidth+j];
                X0 = tmpImage[i*imageWidth+j+1];
                X1 = tmpImage[(i-1)*imageWidth+j+1];
                X2 = tmpImage[(i-1)*imageWidth+j];
                X3 = tmpImage[(i-1)*imageWidth+j-1];
                X4 = tmpImage[i*imageWidth+j-1];
                X5 = tmpImage[(i+1)*imageWidth+j-1];
                X6 = tmpImage[(i+1)*imageWidth+j];
                X7 = tmpImage[(i+1)*imageWidth+j+1];
                string tmpPattern = to_string(X2)+to_string(X4)+to_string(X)+to_string(X0)+to_string(X6);
                if(tmpPattern == interiorFill_5){
                    tmpPattern = to_string(X3)+to_string(X2)+to_string(X1)+
                                               to_string(X4)+to_string(X)+to_string(X0)+
                                               to_string(X5)+to_string(X6)+to_string(X7);
                    if(tmpPattern == interiorFill_9){
                        // Main body
                        numDefect++;
                        // cout << i << " " << j << endl;
                        coordinate = make_pair(i,j);
                        mainBodyDefects.push_back(coordinate);
                    }else{
                        // Boundary
                        numBoundaryHole++;
                        coordinate = make_pair(i,j);
                        boundaryHoles.push_back(coordinate);
                    }
                }
            }
        }
        delete[] tmpImage;
        // cout << "Number of Main body Defects: " << numDefect << endl;
        // cout << "Number of holes in the boundary: " << numBoundaryHole << endl;
        beautifulPrint(mainBodyDefects,numDefect,"body");
        beautifulPrint(boundaryHoles,numBoundaryHole,"boundary");
    }

    void fixMainBodyDefect(){
        int* tmpImage = getOriImage_0_1();
        for(int i = 0; i < mainBodyDefects.size(); i++){
            coordinate = mainBodyDefects[i];
            tmpImage[coordinate.first*imageWidth+coordinate.second] = 1;
        }

        // Set ouput image
        for(int i = 0; i < imageSize_gray; i++){
            outputBuffer[i] = (unsigned char) tmpImage[i]*255;
        }
    }


    int getNumDefect(){
        return numDefect;
    }

    int getNumBoundaryHole(){
        return numBoundaryHole;
    }

    vector<pair<int,int>> getMainBodyDefectsLoc(){
        return mainBodyDefects;
    }

    vector<pair<int,int>> getBoundaryHolesLoc(){
        return boundaryHoles;
    }

private:
    // number of defect
    int numDefect = 0;
    int numBoundaryHole = 0;

    // (x_cor, y_cor)
    pair<int,int> coordinate;
    
    // mainBodyDefects list
    vector<pair<int,int>> mainBodyDefects;
    // boundary hole list
    vector<pair<int,int>> boundaryHoles;

    // pixels nearby
    int X, X0, X1, X2, X3, X4, X5, X6, X7;

    // Additive Operators:
    // 1. Interior Fill  G(j,k) = X∪[X0 ∩ X2 ∩ X4 ∩ X6]
    string interiorFill_5 = "11011";        // First detection
    string interiorFill_9 = "111101111";    // Second detection

    void beautifulPrint(vector<pair<int,int>> coordinateVec, int numberOfDefect, string typeOfDefect){
        if(coordinateVec.size() != 0){
            if(typeOfDefect == "body"){
                cout << "There are defects in the main body" << endl;
                cout << "There are " << coordinateVec.size() << " defects in the main body" << endl;
                cout << "There are located in the following position: " << endl;
                for(int i = 0; i < coordinateVec.size(); i++){
                    cout << "X: " << coordinateVec[i].second << ", Y: " << coordinateVec[i].first << endl;
                }
            }
            else if(typeOfDefect == "boundary"){
                cout << "There are " << coordinateVec.size() << " holes in the boundary" << endl;
                cout << "There are located in the following position: " << endl;
                for(int i = 0; i < coordinateVec.size(); i++){
                    cout << "X: " << coordinateVec[i].second << ", Y: " << coordinateVec[i].first << endl;
                } 
            }
            else cout << typeOfDefect << " defect is not found." << endl;
        }
        else{
            cout << "There are no defects" << endl;
        }
    }
};