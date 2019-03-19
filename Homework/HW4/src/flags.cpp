#include "flags.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void print_usage(){
    cout << 
        "To run this program, must follow the following:\n "
        "      program_name (help or inputImage) outputImage algorithm imageHeight imageWidth BytesPerPixel\n"
        "Arguments Explain: \n"
        " 0.program_name: here is HW3"
        " 1.help:\n"
        "        Print this help message and exit. \n"
        " 1.inputImage: image name want to do the operation with correct path\n"
        " 2.outputImage: image name want to output with correct path, default to current directory\n"
        " 3.algorithm:\n"
        "        The algorithms can be used in this program. Valid values are:\n"
        "            TC: Texture Classification\n"
        "            TS: Texture Segmentation\n"
        "            SIFT: SIFT Image Matching\n"
        "            BOW: Bag of Words Digit Classification\n"
        " 4.imageHeight: height of image\n"
        " 5.imageWidth: width of image\n"
        " 6.BytesPerPixel: color image: BytesPerPixel = 3, \n"
        "                  gray scale image: BytesPerPixel = 1\n"
        " 7.numClass: number of training class(only use when algorithm = BOW)\n"
        " 8.numImagePerClass: number of training images per class(only use when algorithm = BOW)\n";

}

FlagOptions parse_flags(int argc, char** argv){
    FlagOptions flags;

    if(argc < 2){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        print_usage();
        exit(EXIT_FAILURE);
    }
    else{
        if(argv[1] == "help"){
            print_usage();
            exit(EXIT_SUCCESS);
        }
        else{
            if(argc < 7){
                cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
                print_usage();
                exit(EXIT_FAILURE);
            }
            else{
                flags.inputFilename = argv[1];
                flags.outputFilename = argv[2];
                flags.algorithm = argv[3];
                flags.imageHeight = atoi(argv[4]);
                flags.imageWidth = atoi(argv[5]);
                flags.BytesPerPixel = atoi(argv[6]);
                if(argv[3] == "BOW"){
                    if(argc < 9){
                        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
                        print_usage();
                        exit(EXIT_FAILURE);
                    }else{
                        flags.numClassofTraining = atoi(argv[7]);
                        flags.numImgPerClass = atoi(argv[8]);
                    }
                }
            }
        }
    }
    return flags;
}