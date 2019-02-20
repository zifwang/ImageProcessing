// Homework 2: Half-Toning
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "image.h"

using namespace std;

int main(int argc, char** argv){
    // Flag parse
    FlagOptions flags = parse_flags(argc,argv);
    image imageData = image(flags);

    

    
    // else{
    //     cout << "Error: the input algorithm is not found." << endl;
    //     exit(EXIT_FAILURE);
    // }

    return 0;
}

