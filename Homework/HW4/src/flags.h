// Homework4:
// Author: Zifan Wang
/** 
 * This code is used to prase input flags
*/
#pragma once

#include <string>


struct FlagOptions{
    std::string inputFilename;
    std::string outputFilename;
    std::string algorithm;
    int imageHeight;
    int imageWidth;
    int BytesPerPixel;
    int numClassofTraining;
    int numImgPerClass;
};

/** 
 * Prints information about how to use this program.
*/
void print_usage();


/** 
 * Parses any provided flags, populating the given config and file path as
 * appropriate.
*/
FlagOptions parse_flags(int argc, char** argv);

