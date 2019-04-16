// Homework 2: Half-Toning
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "flags.h"
#include "image.h"
#include "RT.h"
#include "DM.h"
#include "errorDiffusion.h"
#include "separableErrorDiffusion.h"
#include "MBVQ.h"
#include "sobelEdgeDetector.h"

using namespace std;

void cannyEdgeDetection(FlagOptions flags);

int main(int argc, char** argv){
    // Flag parse
    FlagOptions flags = parse_flags(argc,argv);

    if(flags.algorithm == "RT"){
        // Init. input image data
        image imageData = image(flags);
        // put into RT method
        RT rtImage;
        rtImage.init_image(imageData); 
        rtImage.methodRT();
        imageData.setOutputImage(rtImage.output(),rtImage.getImageSize()); 
        imageData.writeImage(rtImage.getImageSize());
    }
    else if(flags.algorithm == "DM"){
        // Init. input image data
        image imageData = image(flags);
        // put into DM method
        DM dmImage;
        dmImage.init_image(imageData);
        dmImage.methodDM(2);
        imageData.setOutputImage(dmImage.output(),dmImage.getImageSize()); 
        imageData.writeImage(dmImage.getImageSize());
    }
    else if(flags.algorithm == "FSED"){
        // Init. input image data
        image imageData = image(flags);
        // put into Error Diffusion method
        errorDiffusion erImage;
        erImage.init_image(imageData);
        erImage.methodErrorDiffusion("steinberg");
        imageData.setOutputImage(erImage.output(),erImage.getImageSize());
        imageData.writeImage(erImage.getImageSize());
    }
    else if(flags.algorithm == "JJNED"){
        // Init. input image data
        image imageData = image(flags);
        // put into Error Diffusion method
        errorDiffusion erImage;
        erImage.init_image(imageData);
        erImage.methodErrorDiffusion("JJN");
        imageData.setOutputImage(erImage.output(),erImage.getImageSize());
        imageData.writeImage(erImage.getImageSize());
    }
    else if(flags.algorithm == "STUCKIED"){
        // Init. input image data
        image imageData = image(flags);
        // put into Error Diffusion method
        errorDiffusion erImage;
        erImage.init_image(imageData);
        erImage.methodErrorDiffusion("Stucki");
        imageData.setOutputImage(erImage.output(),erImage.getImageSize());
        imageData.writeImage(erImage.getImageSize());
    }
    else if(flags.algorithm == "SED"){
        // Init. input image data
        image imageData = image(flags);
        // put into separable error diffusion method
        separableErrorDiffusion sedImage;
        sedImage.init_image(imageData);
        sedImage.methodSeparableErrorDiffusion();
        imageData.setOutputImage(sedImage.output(),sedImage.getImageSize());
        imageData.writeImage(sedImage.getImageSize());
    }
    else if(flags.algorithm == "MBVQ"){
        // Init. input image data
        image imageData = image(flags);
        // put into MBVQ method
        MBVQ mbvqImage;
        mbvqImage.init_image(imageData);
        mbvqImage.methodMVQB();
        imageData.setOutputImage(mbvqImage.output(),mbvqImage.getImageSize());
        imageData.writeImage(mbvqImage.getImageSize());
    }
    else if(flags.algorithm == "SOBEL"){
        // Init. input image data
        image imageData = image(flags);
        // put into sobel edge detector method
        sobelEdgeDetector sobeledImage = sobelEdgeDetector(imageData);
        sobeledImage.methodSobelEdgeDetector();
        imageData.setOutputImage(sobeledImage.output(),sobeledImage.getGrayImageSize());
        imageData.writeImage(sobeledImage.getGrayImageSize());
    }
    else if(flags.algorithm == "CANNY"){
        cannyEdgeDetection(flags);
    }
    else{
        cout << "Error: the input algorithm is not found." << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}

void cannyEdgeDetection(FlagOptions flags){
    
    // read jpg image
    cv::Mat oriImage = cv::imread(flags.inputFilename);
    // Convert to gray scale
    cv::Mat grayOriImage;
    cv::cvtColor(oriImage,grayOriImage,CV_BGR2GRAY);
    // Define output image
    cv::Mat outputImage;
    // Apply Canny edge detection
    cv::Canny(grayOriImage,outputImage,200,300,3);
    outputImage = 255 - outputImage;
    // write to image
    cv::imwrite(flags.outputFilename,outputImage);
}

