/**
 * Author: Zifan Wang
 * HW3: Geo and Morphological
 */ 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "image.h"
#include "morphology.h"
#include "defectDetection.h"
#include "objectAnalysis.h"
#include "geoTransformation.h"
#include "lensUndistortion.h"

using namespace std;

int main(int argc, char** argv){
    // Flag parse
    FlagOptions flags = parse_flags(argc,argv);
    image imageData = image(flags);
    
    if(flags.algorithm == "Shrink" || flags.algorithm == "Thin" || flags.algorithm == "Skeletonzie"){
        morphology morphoImg;
        morphoImg.init_image(imageData);
        int* image = new int[morphoImg.getImageSize_gray()];
        if(flags.algorithm == "Shrink"){
            image = morphoImg.morphologyOperation(morphoImg.getOriImage_0_1(),morphoImg.getImageHeight(),morphoImg.getImageWidth(),
                                          morphoImg.getShrinkMark(),morphoImg.getShrinkThinUncondMark(),58,173);
        }
        else if(flags.algorithm == "Thin"){
            image = morphoImg.morphologyOperation(morphoImg.getOriImage_0_1(),morphoImg.getImageHeight(),morphoImg.getImageWidth(),
                                          morphoImg.getThinMark(),morphoImg.getShrinkThinUncondMark(),46,173);
        }
        else if(flags.algorithm == "Skeletonzie"){
            image = morphoImg.morphologyOperation(morphoImg.getOriImage_0_1(),morphoImg.getImageHeight(),morphoImg.getImageWidth(),
                                          morphoImg.getSkeletonizeMark(),morphoImg.getSkeletonizeUncondMark(),40,444);
            // brigde here need to implement
        }
        morphoImg.set_0_1_OutputImage(image);
        delete[] image;
        imageData.setOutputImage(morphoImg.output(),morphoImg.getImageSize_gray()); 
        imageData.writeImage(morphoImg.getImageSize_gray());
    }
    else if(flags.algorithm == "DetectionDefect"){
        defectDetect defectImg;
        defectImg.init_image(imageData);
        defectImg.detection();
        defectImg.fixMainBodyDefect();
        imageData.setOutputImage(defectImg.output(),defectImg.getImageSize_gray()); 
        imageData.writeImage(defectImg.getImageSize_gray());   
    }
    else if(flags.algorithm == "ObjectAnalysis"){
        objectAnalysis objImg;
        objImg.init_image(imageData);
        objImg.analysis();
        imageData.setOutputImage(objImg.output(),objImg.getImageSize_gray()); 
        imageData.writeImage(objImg.getImageSize_gray());   
    }
    else if(flags.algorithm == "GeoTransformation"){
        geoTransformation geoModifiedImg;   
        geoModifiedImg.init_image(imageData);
        geoModifiedImg.methodGEO();
        imageData.setOutputImage(geoModifiedImg.output(),geoModifiedImg.getImageSize_gray()); 
        imageData.writeImage(geoModifiedImg.getImageSize_gray()); 
    }
    else if(flags.algorithm == "SpatialWarp"){
        cout << "Matlab" << endl;
    }
    else if(flags.algorithm == "LensUndistortion"){
        lensUndistortion lenModImg;
        lenModImg.init_image(imageData);
        lenModImg.lens_distortion_correction(600,600);
        imageData.setOutputImage(lenModImg.output(),lenModImg.getImageSize_gray()); 
        imageData.writeImage(lenModImg.getImageSize_gray()); 
    }
    else{
        cout << "Error: the input algorithm is not found." << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}

