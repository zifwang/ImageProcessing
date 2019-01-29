// Homework 2: Half-Toning
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "image.h"
#include "RT.h"

using namespace std;

int main(int argc, char** argv){
    // Flag parse
    FlagOptions flags = parse_flags(argc,argv);
    // Init. input image data
    image imageData = image(flags);
    // put into RT method
    RT rtImage = RT(imageData); 
    imageData.setOutputImage(rtImage.output(),rtImage.getImageSize()); 
    imageData.writeImage();


    return 0;
}

