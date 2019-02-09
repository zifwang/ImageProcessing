#include "flags.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void print_usage(){
    cout << 
        "To run this program, must follow the following:\n "
        "      program_name (help or inputImage) outputImage algorithm imageHeight imageWidth BytesPerPixel\n"
        "Arguments Explain: \n"
        " 0.program_name: here is problem2"
        " 1.help:\n"
        "        Print this help message and exit. \n"
        " 1.inputImage: image name want to do the operation with correct path\n"
        " 2.outputImage: image name want to output with correct path, default to current directory\n"
        " 3.algorithm:\n"
        "        The algorithm can be used in this program. Valid values are:\n"
        "            RT: Random thresholding\n"
        "            DM: Dithering Matrix\n"
        "            FSED: Floyd-Steinberg’s error diffusion\n"
        "            JJNED: Error diffusion proposed by Jarvis, Judice, and Ninke\n"
        "            STUCKIED: Error diffusion proposed by Stucki\n"
        "            SED: Error diffusion for colorful image\n"
        "            MBVQ: MBVQ error diffusion for colorful image\n"
        "            SOBEL: Sobel Edge Detection method\n"
        " 4.imageHeight: height of image\n"
        " 5.imageWidth: width of image\n"
        " 6.BytesPerPixel: color image: BytesPerPixel = 3, \n"
        "                  gray scale image: BytesPerPixel = 1\n";
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
            }
        }
    }
    return flags;
}