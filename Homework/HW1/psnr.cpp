// Problem 2. PSNR Calculation
// Author: Zifan Wang

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>

using namespace std;


int main(int argc, char *argv[]){
    // Define variables
    FILE *file;                                  // Read in file.
    int BytesPerPixel = 3;                       // image is in gray scale, set this para. to 1. If RGB set to 3.
    int ImageHeight = 300, ImageWidth = 390;     // Image size

    // Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [ImageHeight = 256] [ImageWidth = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			ImageHeight = atoi(argv[4]);
            ImageWidth = atoi(argv[5]);
		}
	}


    // Todo: Read raw ori image into program and store it in array
    unsigned char imageDataOri[ImageHeight][ImageWidth][BytesPerPixel];   // Image array to store .raw file
    if(file = fopen(argv[1],"rb")){
        fread(imageDataOri,sizeof(unsigned char), ImageHeight*ImageWidth*BytesPerPixel, file);
    }else{
        cout << "Error opening file" << endl;
        exit(1);
    }

    // TODO: Read raw mod image into program
    unsigned char imageDataMOD[ImageHeight][ImageWidth][BytesPerPixel];   // Image array to store .raw file
    if(file = fopen(argv[1],"rb")){
        fread(imageDataMOD,sizeof(unsigned char), ImageHeight*ImageWidth*BytesPerPixel, file);
    }else{
        cout << "Error opening file" << endl;
        exit(1);
    }

    ofstream psnr("outputPSNR.txt");
    if(psnr.is_open()){
        double MSE = 0, PSNR = 0;
        for(int channel = 0; channel < BytesPerPixel; channel++){
            for(int h = 0; h < ImageHeight; h++){
                for(int w = 0; w < ImageWidth; w++){
                    int temp = int(imageDataMOD[h][w][channel])-int(imageDataOri[h][w][channel]);
                    MSE = MSE + pow(temp,2);
                }
            }
        }
        MSE  = MSE/(ImageHeight*ImageWidth*BytesPerPixel);
        PSNR = 10*log10(pow(255,2)/MSE);
        psnr << PSNR << endl;
    }
    psnr.close();

    return 0;
}