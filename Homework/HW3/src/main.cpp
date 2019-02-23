// Homework 2: Half-Toning
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "image.h"
#include "shrinking.h"
#include "thinning.h"
#include "skeletonizing.h"

using namespace std;

int main(int argc, char** argv){
    // Flag parse
    FlagOptions flags = parse_flags(argc,argv);
    image imageData = image(flags);
    // shrinking shrinkImg;
    // shrinkImg.init_image(imageData);
    // shrinkImg.methodShrinking();
    // imageData.setOutputImage(shrinkImg.output(),shrinkImg.getImageSize_gray()); 
    // imageData.writeImage(shrinkImg.getImageSize_gray());
    // thinning thinImg;
    // thinImg.init_image(imageData);
    // thinImg.methodThinning();
    // imageData.setOutputImage(thinImg.output(),thinImg.getImageSize_gray()); 
    // imageData.writeImage(thinImg.getImageSize_gray());    
    skeletonzing sktImg;
    sktImg.init_image(imageData);
    sktImg.methodSkeletonzing();
    imageData.setOutputImage(sktImg.output(),sktImg.getImageSize_gray()); 
    imageData.writeImage(sktImg.getImageSize_gray());   

    
    // else{
    //     cout << "Error: the input algorithm is not found." << endl;
    //     exit(EXIT_FAILURE);
    // }

    return 0;
}

