// Homework 2: Half-Toning
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "image.h"
#include "RT.h"
#include "DM.h"
#include "errorDiffusion.h"

using namespace std;

int main(int argc, char** argv){
    // Flag parse
    FlagOptions flags = parse_flags(argc,argv);
    // Init. input image data
    image imageData = image(flags);
    // put into RT method
    // RT rtImage;
    // rtImage.init_image(imageData); 
    // rtImage.methodRT();
    // imageData.setOutputImage(rtImage.output(),rtImage.getImageSize()); 
    // imageData.writeImage();

    // put into DM method
    // DM dmImage;
    // dmImage.init_image(imageData);
    // dmImage.methodDM(2);
    // imageData.setOutputImage(dmImage.output(),dmImage.getImageSize()); 
    // imageData.writeImage();
    
    // put into Error Diffusion method
    errorDiffusion erImage;
    erImage.init_image(imageData);
    erImage.methodErrorDiffusion("JJN");
    imageData.setOutputImage(erImage.output(),erImage.getImageSize());
    imageData.writeImage();
    return 0;
}

