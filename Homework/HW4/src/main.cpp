/**
 * Author: Zifan Wang
 * HW4
 */ 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include "flags.h"
#include "image.h"
#include "textureClassification.h"
#include "textureSegmentation.h"
#include "SIFT_Experiment.h"
#include "bagofWords.h"

using namespace std;

int main(int argc, char** argv){
    // Flag parse
    FlagOptions flags = parse_flags(argc,argv);
    if(flags.algorithm == "TC"){
        textureClassification tc = textureClassification(flags);
        tc.methodClassification();
    }
    else if(flags.algorithm == "TS"){
        textureSegmentation ts = textureSegmentation(flags);
        ts.methodSegmentation();
    }
    else if(flags.algorithm == "SIFT"){
        sift siftExp = sift(flags);
        siftExp.methodSIFT(2);      // input argument: threshold of keypoints 
    }
    else if(flags.algorithm == "BOW"){
        bow bowExp = bow(flags);
        bowExp.methodBOW();
    }


    return 0;
}

