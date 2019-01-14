// Problem 1. A. Bilinear Demosaicing
// Author: Zifan Wang

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

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


    // Todo: Read raw image into program and store it in array
    unsigned char imageData[ImageHeight][ImageWidth][BytesPerPixel];   // Image array to store .raw file
    // Todo: Modify Address
    if(file = fopen(argv[1],"rb")){
        fread(imageData,sizeof(unsigned char), ImageHeight*ImageWidth*BytesPerPixel, file);
    }else{
        cout << "Error opening file" << endl;
        exit(1);
    }

    
    // Todo: Bilinear Demosaicing
    // Step1: Boundary reflection: Reflect two pixels near boundary
    unsigned char imageTemp[ImageHeight+2][ImageWidth][BytesPerPixel];
    unsigned char imageExtend[ImageHeight+2][ImageWidth+2][BytesPerPixel];
    for(int channel = 0; channel < BytesPerPixel; channel++){
        // Extend height(row) 
        for(int h = 0; h < ImageHeight+2; h++){
            for(int w = 0; w < ImageWidth; w++){
                if(h == 0) imageTemp[h][w][channel] = imageData[h][w][channel];
                else if(h == ImageHeight+1) imageTemp[h][w][channel] = imageData[h-2][w][channel];
                else imageTemp[h][w][channel] = imageData[h-1][w][channel];
            }
        }
        // Extend width(col)
        for(int w = 0; w < ImageWidth+2; w++){
            for(int h = 0; h < ImageHeight+2; h++){
                if(w == 0) imageExtend[h][w][channel] = imageTemp[h][w][channel];
                else if(w == ImageWidth+1) imageExtend[h][w][channel] = imageTemp[h][w-2][channel];
                else imageExtend[h][w][channel] = imageTemp[h][w-1][channel];
            }
        }
    }
    // Step2: Apply Demosaicing method to the extended image
    // GRBG Bayers' Pattern and channels: RGB
    unsigned char imageOut[ImageHeight][ImageWidth][3];

    /** 
    ** Green, Blue, Red pixels' red values estimation by using Red pixels
    **/
    // Red pixels line
    for(int i = 0; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            // Red value at green pixels position which in the same line as red pixels
            if(j == 0) imageOut[i][j][0] = imageExtend[i+1][j+2][0];
            else if(j == ImageWidth-1) imageOut[i][j][0] = imageExtend[i][j-1][0];
            else imageOut[i][j][0] = (char)((int(imageExtend[i+1][j+2][0]) + int(imageExtend[i+1][j][0]))/2);
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            // Red value at red pixels position
            imageOut[i][k][0] = imageExtend[i+1][k+1][0]; 
        }
    }
    // Blue pixels line
    for(int i = 1; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            // Red value at blue pixels position
            if(j == 0) imageOut[i][j][0] = (char)((int(imageExtend[i][j+2][0]) + int(imageExtend[i+2][j+2][0]))/2);
            else if(j == ImageWidth-1) imageOut[i][j][0] = (char)((int(imageExtend[i][j][0]) + int(imageExtend[i+2][j][0]))/2);
            else imageOut[i][j][0] = (char) ( ( int(imageExtend[i][j][0]) + int(imageExtend[i][j+2][0]) + int(imageExtend[i+2][j][0]) + int(imageExtend[i+2][j+2][0]) )/4);
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            if(i == ImageHeight-1) imageOut[i][k][0] = imageExtend[i][k+1][0];
            imageOut[i][k][0] = (char)((int(imageExtend[i][k+1][0]) + int(imageExtend[i+2][k+1][0]))/2);
        }
    }

    /** 
    ** Green, Blue, Red pixels' green values estimation by using Green pixels
    **/
    for(int i = 0; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            imageOut[i][j][1] = imageExtend[i+1][j+1][0];
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            imageOut[i][k][1] = (char)(int(imageExtend[i][k+1][0]) + int(imageExtend[i+1][k][0]) + int(imageExtend[i+2][k+1][0]) + int(imageExtend[i+1][k+2][0]))/4;
        }
    }
    for(int i = 1; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            imageOut[i][j][1] = (char)(int(imageExtend[i][j+1][0]) + int(imageExtend[i+1][j][0]) + int(imageExtend[i+2][j+1][0]) + int(imageExtend[i+1][j+2][0]))/4;
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            imageOut[i][k][1] = imageExtend[i+1][k+1][0];
        }
    }

    /** 
    ** Green, Blue, Red pixels' blue values estimation by using Blue pixels
    **/
    for(int i = 0; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            imageOut[i][j][2] = (char)(int(imageExtend[i][j+1][0]) + int(imageExtend[i+2][j+1][0]))/2;
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            imageOut[i][k][2] = (char)(int(imageExtend[i][k][0]) + int(imageExtend[i+2][k][0]) + int(imageExtend[i+2][k+2][0]) + int(imageExtend[i][k+2][0]))/4;
        }
    }
    for(int i = 1; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            imageOut[i][j][2] = imageExtend[i+1][j+1][0];
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            imageOut[i][k][2] = (char)(int(imageExtend[i+1][k][0]) + int(imageExtend[i+1][k+2][0]))/2;
        }
    }



    // Todo: Store image
    if(!(file=fopen(argv[2],"wb"))){
        cout << "Cannot open file: " << endl;
        exit(1);
    }
    fwrite(imageOut, sizeof(unsigned char), (ImageHeight)*(ImageWidth)*3, file);
    fclose(file);
    
    return 0;
}    
    








