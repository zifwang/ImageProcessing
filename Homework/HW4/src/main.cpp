/**
 * Author: Zifan Wang
 * HW3: Geo and Morphological
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

using namespace std;
using namespace cv;

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
    // else if(flags.algorithm == "SIFT"){

    // }


    return 0;
}

