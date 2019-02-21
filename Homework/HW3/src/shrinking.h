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
#include "morphology.h"
#include "image.h"

using namespace std;

class shrinking : public morphology{
public:
    void methodShrinking(){

    }

private:
    int shrinkMarks[58][9] = {
        // S: bound 1
        0,0,1,0,1,0,0,0,0,
        1,0,0,0,1,0,0,0,0,
        0,0,0,0,1,0,1,0,0,
        0,0,0,0,1,0,0,0,1,
        // S: bound 2
        0,0,0,0,1,1,0,0,0,
        0,1,0,0,1,0,0,0,0,
        0,0,0,1,1,0,0,0,0,
        0,0,0,0,1,0,0,1,0,
        // S: bound 3
        0,0,1,0,1,1,0,0,0,
        0,1,1,0,1,0,0,0,0,
        1,1,0,0,1,0,0,0,0,
        1,0,0,1,1,0,0,0,0,
        0,0,0,1,1,0,1,0,0,
        0,0,0,0,1,0,1,1,0,
        0,0,0,0,1,0,0,1,1,
        0,0,0,0,1,1,0,0,1,
        // STK: bound 4
        0,0,1,0,1,1,0,0,1,
        1,1,1,0,1,0,0,0,0,
        1,0,0,1,1,0,1,0,0,
        0,0,0,0,1,0,1,1,1,
        // STK: bound 6
        1,1,1,0,1,1,0,0,0,
        0,1,1,0,1,1,0,0,1,
        1,1,1,1,1,0,0,0,0,
        1,1,0,1,1,0,1,0,0,
        1,0,0,1,1,0,1,1,0,
        0,0,0,1,1,0,1,1,1,
        0,0,0,0,1,1,1,1,1,
        0,0,1,0,1,1,0,1,1,
        // STK: bound 7
        1,1,1,0,1,1,0,0,1,
        1,1,1,1,1,0,1,0,0,
        1,0,0,1,1,0,1,1,1,
        0,0,1,0,1,1,1,1,1,
        // STK: bound 8
        0,1,1,0,1,1,0,1,1,
        1,1,1,1,1,1,0,0,0,
        1,1,0,1,1,0,1,1,0,
        0,0,0,1,1,1,1,1,1,
        // STK: bound 9
        1,1,1,0,1,1,0,1,1,
        0,1,1,0,1,1,1,1,1,
        1,1,1,1,1,1,1,0,0,
        1,1,1,1,1,1,0,0,1,
        1,1,1,1,1,0,1,1,0,
        1,1,0,1,1,0,1,1,1,
        1,0,0,1,1,1,1,1,1,
        0,0,1,1,1,1,1,1,1,
        // STK: bound 10
        1,1,1,0,1,1,1,1,1,
        1,1,1,1,1,1,1,0,1,
        1,1,1,1,1,0,1,1,1,
        1,0,1,1,1,1,1,1,1,
        // ST: bound 5
        1,1,0,0,1,1,0,0,0,
        0,1,0,0,1,1,0,0,1,
        0,1,1,1,1,0,0,0,0,
        0,0,1,0,1,1,0,1,0,
        0,1,1,0,1,1,0,0,0,
        1,1,0,1,1,0,0,0,0,
        0,0,0,1,1,0,1,1,0,
        0,0,0,0,1,1,0,1,1,
        // ST: bound 6
        1,1,0,0,1,1,0,0,1,
        0,1,1,1,1,0,1,0,0
    };

    
    int shrinkUncondMark [331][9] = {
        // Spur 2
        0,0,1,0,1,0,0,0,0,
        1,0,0,0,1,0,0,0,0,
        // Single 4-connection 2
        0,0,0,0,1,0,0,1,0,
        0,0,0,0,1,1,0,0,0,
        // L Cluster 8
        0,0,1,0,1,1,0,0,0,
        0,1,1,0,1,0,0,0,0,
        1,1,0,0,1,0,0,0,0,
        1,0,0,1,1,0,0,0,0,
        0,0,0,1,1,0,1,0,0,
        0,0,0,0,1,0,1,1,0,
        0,0,0,0,1,0,0,1,1,
        0,0,0,0,1,1,0,0,1,
        // 4-connected Offset 4
        0,1,1,1,1,0,0,0,0,
        1,1,0,0,1,1,0,0,0,
        0,1,0,0,1,1,0,0,1,
        0,0,1,0,1,1,0,1,0,
        // Spur corner Cluster 12
        0,1,1,0,1,0,1,0,0,
        0,1,1,0,1,1,1,0,0,
        0,0,1,0,1,1,1,0,0,
        1,0,0,1,1,0,0,0,1,
        1,1,0,1,1,0,0,0,1,
        1,1,0,0,1,0,0,0,1,
        0,0,1,1,1,0,1,0,0,
        0,0,1,1,1,0,1,1,0,
        0,0,1,0,1,0,1,1,0,
        1,0,0,0,1,0,0,1,1,
        1,0,0,0,1,1,0,1,1,
        1,0,0,0,1,1,0,0,1,
        // Corner Cluster 31
        // 1
        1,1,0,1,1,0,0,0,0,
        // 5
        1,1,1,1,1,0,0,0,0,
        1,1,0,1,1,1,0,0,0,
        1,1,0,1,1,0,1,0,0,
        1,1,0,1,1,0,0,1,0,
        1,1,0,1,1,0,0,0,1,
        // 10
        1,1,1,1,1,1,0,0,0,
        1,1,1,1,1,0,1,0,0,
        1,1,1,1,1,0,0,1,0,
        1,1,1,1,1,0,0,0,1,
        1,1,0,1,1,1,1,0,0,
        1,1,0,1,1,1,0,1,0,
        1,1,0,1,1,1,0,0,1,
        1,1,0,1,1,0,1,1,0,
        1,1,0,1,1,0,1,0,1,
        1,1,0,1,1,0,0,1,1,
        // 10
        1,1,1,1,1,1,1,0,0,
        1,1,1,1,1,1,0,1,0,
        1,1,1,1,1,1,0,0,1,
        1,1,1,1,1,0,1,1,0,
        1,1,1,1,1,0,1,0,1,
        1,1,1,1,1,0,0,1,1,
        1,1,0,1,1,1,1,1,0,
        1,1,0,1,1,1,1,0,1,
        1,1,0,1,1,1,0,1,1,
        1,1,0,1,1,0,1,1,1,
        // 4
        1,1,1,1,1,1,1,1,0,
        1,1,1,1,1,1,1,0,1,
        1,1,1,1,1,0,1,1,1,
        1,1,0,1,1,1,1,1,1,
        // 1
        1,1,1,1,1,1,1,1,1,
        // Tee Branch 32
        0,1,0,1,1,1,0,0,0,
        1,1,0,1,1,1,0,0,0,
        0,1,0,1,1,1,1,0,0,
        1,1,0,1,1,1,1,0,0,
        0,1,0,1,1,1,0,0,0,
        0,1,1,1,1,1,0,0,0,
        0,1,0,1,1,1,0,0,1,
        0,1,1,1,1,1,0,0,1,
        0,0,0,1,1,1,0,1,0,
        0,0,1,1,1,1,0,1,0,
        0,0,0,1,1,1,0,1,1,
        0,0,1,1,1,1,0,1,1,
        0,0,0,1,1,1,0,1,0,
        1,0,0,1,1,1,0,1,0,
        0,0,0,1,1,1,1,1,0,
        1,0,0,1,1,1,1,1,0,
        0,1,0,1,1,0,0,1,0,
        1,1,0,1,1,0,0,1,0,
        0,1,1,1,1,0,0,1,0,
        1,1,1,1,1,0,0,1,0,
        0,1,0,1,1,0,0,1,0,
        0,1,0,1,1,0,1,1,0,
        0,1,0,1,1,0,0,1,1,
        0,1,0,1,1,0,1,1,1,
        0,1,0,0,1,1,0,1,0,
        0,1,0,0,1,1,1,1,0,
        0,1,0,0,1,1,0,1,1,
        0,1,0,0,1,1,1,1,1,
        0,1,0,0,1,1,0,1,0,
        1,1,0,0,1,1,0,1,0,
        0,1,1,0,1,1,0,1,0,
        1,1,1,0,1,1,0,1,0,
        // Vee Branch 4*8*7 = 224
        1,0,1,0,1,0,1,0,0,
        1,0,1,0,1,0,0,1,0,
        1,0,1,0,1,0,0,0,1,
        1,0,1,0,1,0,1,1,0,
        1,0,1,0,1,0,1,0,1,
        1,0,1,0,1,0,0,1,1,
        1,0,1,0,1,0,1,1,1,

        1,1,1,0,1,0,1,0,0,
        1,1,1,0,1,0,0,1,0,
        1,1,1,0,1,0,0,0,1,
        1,1,1,0,1,0,1,1,0,
        1,1,1,0,1,0,1,0,1,
        1,1,1,0,1,0,0,1,1,
        1,1,1,0,1,0,1,1,1,

        1,0,1,1,1,0,1,0,0,
        1,0,1,1,1,0,0,1,0,
        1,0,1,1,1,0,0,0,1,
        1,0,1,1,1,0,1,1,0,
        1,0,1,1,1,0,1,0,1,
        1,0,1,1,1,0,0,1,1,
        1,0,1,1,1,0,1,1,1,

        1,0,1,0,1,1,1,0,0,
        1,0,1,0,1,1,0,1,0,
        1,0,1,0,1,1,0,0,1,
        1,0,1,0,1,1,1,1,0,
        1,0,1,0,1,1,1,0,1,
        1,0,1,0,1,1,0,1,1,
        1,0,1,0,1,1,1,1,1,

        1,1,1,1,1,0,1,0,0,
        1,1,1,1,1,0,0,1,0,
        1,1,1,1,1,0,0,0,1,
        1,1,1,1,1,0,1,1,0,
        1,1,1,1,1,0,1,0,1,
        1,1,1,1,1,0,0,1,1,
        1,1,1,1,1,0,1,1,1,

        1,1,1,0,1,1,1,0,0,
        1,1,1,0,1,1,0,1,0,
        1,1,1,0,1,1,0,0,1,
        1,1,1,0,1,1,1,1,0,
        1,1,1,0,1,1,1,0,1,
        1,1,1,0,1,1,0,1,1,
        1,1,1,0,1,1,1,1,1,

        1,0,1,1,1,1,1,0,0,
        1,0,1,1,1,1,0,1,0,
        1,0,1,1,1,1,0,0,1,
        1,0,1,1,1,1,1,1,0,
        1,0,1,1,1,1,1,0,1,
        1,0,1,1,1,1,0,1,1,
        1,0,1,1,1,1,1,1,1,

        1,1,1,1,1,1,1,0,0,
        1,1,1,1,1,1,0,1,0,
        1,1,1,1,1,1,0,0,1,
        1,1,1,1,1,1,1,1,0,
        1,1,1,1,1,1,1,0,1,
        1,1,1,1,1,1,0,1,1,
        1,1,1,1,1,1,1,1,1,

        //
        1,0,0,0,1,0,1,0,1,
        1,0,0,0,1,1,1,0,0,
        1,0,1,0,1,0,1,0,0,
        1,0,0,0,1,1,1,0,1,
        1,0,1,0,1,0,1,0,1,
        1,0,1,0,1,1,1,0,0,
        1,0,1,0,1,1,1,0,1,
        
        1,1,0,0,1,0,1,0,1,
        1,1,0,0,1,1,1,0,0,
        1,1,1,0,1,0,1,0,0,
        1,1,0,0,1,1,1,0,1,
        1,1,1,0,1,0,1,0,1,
        1,1,1,0,1,1,1,0,0,
        1,1,1,0,1,1,1,0,1,

        1,0,0,1,1,0,1,0,1,
        1,0,0,1,1,1,1,0,0,
        1,0,1,1,1,0,1,0,0,
        1,0,0,1,1,1,1,0,1,
        1,0,1,1,1,0,1,0,1,
        1,0,1,1,1,1,1,0,0,
        1,0,1,1,1,1,1,0,1,

        1,0,0,0,1,0,1,1,1,
        1,0,0,0,1,1,1,1,0,
        1,0,1,0,1,0,1,1,0,
        1,0,0,0,1,1,1,1,1,
        1,0,1,0,1,0,1,1,1,
        1,0,1,0,1,1,1,1,0,
        1,0,1,0,1,1,1,1,1,

        1,1,0,1,1,0,1,0,1,
        1,1,0,1,1,1,1,0,0,
        1,1,1,1,1,0,1,0,0,
        1,1,0,1,1,1,1,0,1,
        1,1,1,1,1,0,1,0,1,
        1,1,1,1,1,1,1,0,0,
        1,1,1,1,1,1,1,0,1,

        1,1,0,0,1,0,1,1,1,
        1,1,0,0,1,1,1,1,0,
        1,1,1,0,1,0,1,1,0,
        1,1,0,0,1,1,1,1,1,
        1,1,1,0,1,0,1,1,1,
        1,1,1,0,1,1,1,1,0,
        1,1,1,0,1,1,1,1,1,

        1,0,0,1,1,0,1,1,1,
        1,0,0,1,1,1,1,1,0,
        1,0,1,1,1,0,1,1,0,
        1,0,0,1,1,1,1,1,1,
        1,0,1,1,1,0,1,1,1,
        1,0,1,1,1,1,1,1,0,
        1,0,1,1,1,1,1,1,1,

        1,1,0,1,1,0,1,1,1,
        1,1,0,1,1,1,1,1,0,
        1,1,1,1,1,0,1,1,0,
        1,1,0,1,1,1,1,1,1,
        1,1,1,1,1,0,1,1,1,
        1,1,1,1,1,1,1,1,0,
        1,1,1,1,1,1,1,1,1,

        //
        0,0,1,0,1,0,1,0,1,
        0,1,0,0,1,0,1,0,1,
        1,0,0,0,1,0,1,0,1,
        0,1,1,0,1,0,1,0,1,
        1,0,1,0,1,0,1,0,1,
        1,1,0,0,1,0,1,0,1,
        1,1,1,0,1,0,1,0,1,

        0,0,1,1,1,0,1,0,1,
        0,1,0,1,1,0,1,0,1,
        1,0,0,1,1,0,1,0,1,
        0,1,1,1,1,0,1,0,1,
        1,0,1,1,1,0,1,0,1,
        1,1,0,1,1,0,1,0,1,
        1,1,1,1,1,0,1,0,1,

        0,0,1,0,1,1,1,0,1,
        0,1,0,0,1,1,1,0,1,
        1,0,0,0,1,1,1,0,1,
        0,1,1,0,1,1,1,0,1,
        1,0,1,0,1,1,1,0,1,
        1,1,0,0,1,1,1,0,1,
        1,1,1,0,1,1,1,0,1,

        0,0,1,0,1,0,1,1,1,
        0,1,0,0,1,0,1,1,1,
        1,0,0,0,1,0,1,1,1,
        0,1,1,0,1,0,1,1,1,
        1,0,1,0,1,0,1,1,1,
        1,1,0,0,1,0,1,1,1,
        1,1,1,0,1,0,1,1,1,

        0,0,1,1,1,1,1,0,1,
        0,1,0,1,1,1,1,0,1,
        1,0,0,1,1,1,1,0,1,
        0,1,1,1,1,1,1,0,1,
        1,0,1,1,1,1,1,0,1,
        1,1,0,1,1,1,1,0,1,
        1,1,1,1,1,1,1,0,1,

        0,0,1,1,1,0,1,1,1,
        0,1,0,1,1,0,1,1,1,
        1,0,0,1,1,0,1,1,1,
        0,1,1,1,1,0,1,1,1,
        1,0,1,1,1,0,1,1,1,
        1,1,0,1,1,0,1,1,1,
        1,1,1,1,1,0,1,1,1,

        0,0,1,0,1,1,1,1,1,
        0,1,0,0,1,1,1,1,1,
        1,0,0,0,1,1,1,1,1,
        0,1,1,0,1,1,1,1,1,
        1,0,1,0,1,1,1,1,1,
        1,1,0,0,1,1,1,1,1,
        1,1,1,0,1,1,1,1,1,

        0,0,1,1,1,1,1,1,1,
        0,1,0,1,1,1,1,1,1,
        1,0,0,1,1,1,1,1,1,
        0,1,1,1,1,1,1,1,1,
        1,0,1,1,1,1,1,1,1,
        1,1,0,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,

        //
        1,0,1,0,1,0,0,0,1,
        0,0,1,1,1,0,0,0,1,
        0,0,1,0,1,0,1,0,1,
        1,0,1,1,1,0,0,0,1,
        1,0,1,0,1,0,1,0,1,
        0,0,1,1,1,0,1,0,1,
        1,0,1,1,1,0,1,0,1,

        1,1,1,0,1,0,0,0,1,
        0,1,1,1,1,0,0,0,1,
        0,1,1,0,1,0,1,0,1,
        1,1,1,1,1,0,0,0,1,
        1,1,1,0,1,0,1,0,1,
        0,1,1,1,1,0,1,0,1,
        1,1,1,1,1,0,1,0,1,

        1,0,1,0,1,1,0,0,1,
        0,0,1,1,1,1,0,0,1,
        0,0,1,0,1,1,1,0,1,
        1,0,1,1,1,1,0,0,1,
        1,0,1,0,1,1,1,0,1,
        0,0,1,1,1,1,1,0,1,
        1,0,1,1,1,1,1,0,1,

        1,0,1,0,1,0,0,1,1,
        0,0,1,1,1,0,0,1,1,
        0,0,1,0,1,0,1,1,1,
        1,0,1,1,1,0,0,1,1,
        1,0,1,0,1,0,1,1,1,
        0,0,1,1,1,0,1,1,1,
        1,0,1,1,1,0,1,1,1,

        1,1,1,0,1,1,0,0,1,
        0,1,1,1,1,1,0,0,1,
        0,1,1,0,1,1,1,0,1,
        1,1,1,1,1,1,0,0,1,
        1,1,1,0,1,1,1,0,1,
        0,1,1,1,1,1,1,0,1,
        1,1,1,1,1,1,1,0,1,

        1,1,1,0,1,0,0,1,1,
        0,1,1,1,1,0,0,1,1,
        0,1,1,0,1,0,1,1,1,
        1,1,1,1,1,0,0,1,1,
        1,1,1,0,1,0,1,1,1,
        0,1,1,1,1,0,1,1,1,
        1,1,1,1,1,0,1,1,1,

        1,0,1,0,1,1,0,1,1,
        0,0,1,1,1,1,0,1,1,
        0,0,1,0,1,1,1,1,1,
        1,0,1,1,1,1,0,1,1,
        1,0,1,0,1,1,1,1,1,
        0,0,1,1,1,1,1,1,1,
        1,0,1,1,1,1,1,1,1,        

        1,1,1,0,1,1,0,1,1,
        0,1,1,1,1,1,0,1,1,
        0,1,1,0,1,1,1,1,1,
        1,1,1,1,1,1,0,1,1,
        1,1,1,0,1,1,1,1,1,
        0,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,   

        // Diagonal Branch 16
        0,1,0,0,1,1,1,0,0,
        1,1,0,0,1,1,1,0,0,
        0,1,0,0,1,1,1,0,1,
        1,1,0,0,1,1,1,0,1,
        0,1,0,1,1,0,0,0,1,
        0,1,1,1,1,0,0,0,1,
        0,1,0,1,1,0,1,0,1,
        0,1,1,1,1,0,1,0,1,
        0,0,1,1,1,0,0,1,0,
        1,0,1,1,1,0,0,1,0,
        0,0,1,1,1,0,0,1,1,
        1,0,1,1,1,0,0,1,1,
        1,0,0,0,1,1,0,1,0,
        1,0,1,0,1,1,0,1,0,
        1,0,0,0,1,1,1,1,0,
        1,0,1,0,1,1,1,1,0
    };


}