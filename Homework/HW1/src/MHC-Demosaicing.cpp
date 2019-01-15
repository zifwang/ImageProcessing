// Problem 1. B. MHC Demosaicing
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
    if(file = fopen(argv[1],"rb")){
        fread(imageData,sizeof(unsigned char), ImageHeight*ImageWidth*BytesPerPixel, file);
    }else{
        cout << "Error opening file" << endl;
        exit(1);
    }

    // Todo: Bilinear Demosaicing
    // Step1: Boundary reflection: Reflect two pixels near boundary
    unsigned char imageTemp[ImageHeight+4][ImageWidth][BytesPerPixel];
    unsigned char imageExtend[ImageHeight+4][ImageWidth+4][BytesPerPixel];
    for(int channel = 0; channel < BytesPerPixel; channel++){
        // Extend height(row) 
        for(int h = 0; h < ImageHeight+4; h++){
            for(int w = 0; w < ImageWidth; w++){
                if(h == 0) imageTemp[h][w][channel] = imageData[h+1][w][channel];
                else if(h == 1) imageTemp[h][w][channel] = imageData[h-1][w][channel];
                else if(h == ImageHeight+2) imageTemp[h][w][channel] = imageData[h-3][w][channel];
                else if(h == ImageHeight+3) imageTemp[h][w][channel] = imageData[h-5][w][channel];
                else imageTemp[h][w][channel] = imageData[h-2][w][channel];
            }
        }
        // Extend width(col)
        for(int w = 0; w < ImageWidth+4; w++){
            for(int h = 0; h < ImageHeight+4; h++){
                if(w == 0) imageExtend[h][w][channel] = imageTemp[h][w+1][channel];
                else if(w == 1) imageExtend[h][w][channel] = imageTemp[h][w-1][channel];
                else if(w == ImageWidth+2) imageExtend[h][w][channel] = imageTemp[h][w-3][channel];
                else if(w == ImageWidth+3) imageExtend[h][w][channel] = imageTemp[h][w-5][channel];
                else imageExtend[h][w][channel] = imageTemp[h][w-2][channel];
            }
        }
    }
    // Step2: Apply MHC Demosaicing method to the extended image
    // Followed by GRBG Bayers' Pattern and channels: RGB
    // Create an output image
    unsigned char imageOut[ImageHeight][ImageWidth][3];
    /** Red Layer
    ** Green, Blue, Red pixels' red values estimation by using Red pixels
    **/
    // Red pixels line
    for(int i = 0; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            // Red at Green
            // Red value at green pixes poistion which in the same horz. line as red pixels
            int tempVal = (int(imageExtend[i+2][j+2-1][0]) + int(imageExtend[i+2][j+2+1][0]))/2; // Bilinear two red pixels where coeff = 4.
            tempVal = tempVal + 5*(int(imageExtend[i+2][j+2][0]))/8; // Add middle green pixels where coeff = 5.
            // Green value where green pixels' coeff = -1. 
            tempVal = tempVal - (int(imageExtend[i+2-1][j+2-1][0]) + int(imageExtend[i+2][j+2-2][0]) + int(imageExtend[i+2+1][j+2-1][0]) + int(imageExtend[i+2+1][j+2+1][0]) + int(imageExtend[i+2][j+2+2][0]) + int(imageExtend[i+2-1][j+2+1][0]))/8; 
            // Green value where green pixels' coeff = 1/2.
            tempVal = tempVal + (int(imageExtend[i+2-2][j+2][0]) + int(imageExtend[i+2+2][j+2][0]))/16;

            // Prevent value more than or less than grey scale range
            if(tempVal < 0) tempVal = 0;
            if(tempVal > 255) tempVal = 255;

            // Generate red value at location i,j which is green pixels
            imageOut[i][j][0] =  (unsigned char)tempVal;
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            // Red value at red pixels
            imageOut[i][k][0] =  imageExtend[i+2][k+2][0];
        }
    }
    // Blue pixels line
    for(int i = 1; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            // Red at Blue pixels
            // Bilinear 4 nearby red value 
            int tempVal = (int(imageExtend[i+2-1][j+2-1][0]) + int(imageExtend[i+2+1][j+2-1][0]) + int(imageExtend[i+2+1][j+2+1][0]) + int(imageExtend[i+2-1][j+2+1][0]))/4;
            // Add correction term to red value
            tempVal = tempVal + 3*(int(imageExtend[i+2][j+2][0]) - (int(imageExtend[i+2][j+2-2][0]) + int(imageExtend[i+2+2][j+2][0]) + int(imageExtend[i+2][j+2+2][0]) + int(imageExtend[i+2-2][j+2][0]))/4)/4;

            // Prevent value more than or less than grey scale range
            if(tempVal < 0) tempVal = 0;
            if(tempVal > 255) tempVal = 255;

            // Generate red value at location i,j which is blue pixels
            imageOut[i][j][0] =  (unsigned char)tempVal;
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            // Red at green which in the same horz. line of blue pixels
            // Bilinear 2 nearby red value
            int tempVal = (int(imageExtend[i+2-1][k+2][0]) + int(imageExtend[i+2+1][k+2][0]))/2;
            // Add middle green weighted by 5/8
            tempVal = tempVal + 5*int(imageExtend[i+2][k+2][0])/8;
            // Add two green each weighted by 1/16
            tempVal = tempVal + int(imageExtend[i+2][k+2-2][0])/16 + int(imageExtend[i+2][k+2+2][0])/16;
            // Add six green each weighted by -1/8
            tempVal = tempVal - (int(imageExtend[i+2-1][k+2-1][0]) + int(imageExtend[i+2+1][k+2-1][0]) + int(imageExtend[i+2+2][k+2][0]) + int(imageExtend[i+2+1][k+2+1][0]) + int(imageExtend[i+2-1][k+2+1][0]) + int(imageExtend[i+2-2][k+2][0]))/8;

            // Prevent value more than or less than grey scale range
            if(tempVal < 0) tempVal = 0;
            if(tempVal > 255) tempVal = 255;

            // Generate red value at location i,j which is green pixels
            imageOut[i][k][0] =  (unsigned char)tempVal;
        }   
    }

    /** Green Layer
    ** Green, Blue, Red pixels' red values estimation by using Red pixels
    **/
    // Red pixels' horz. line
    for(int i = 0; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            // Green at Green pixels which in the same horz. line as red pixels
            imageOut[i][j][1] = imageExtend[i+2][j+2][0]; 
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            // Green at red pixels
            int tempVal = (int(imageExtend[i+2][k+2-1][0]) + int(imageExtend[i+2+1][k+2][0]) + int(imageExtend[i+2][k+2+1][0]) + int(imageExtend[i+2-1][k+21][0]))/4;
            // Add Correction term.
            tempVal = tempVal + (int(imageExtend[i+2][k+2][0]) - (int(imageExtend[i+2][k+2-2][0]) + int(imageExtend[i+2+2][k+2][0]) + int(imageExtend[i+2][k+2+2][0]) + int(imageExtend[i+2-2][k+2][0]))/4)/2;

            // Prevent value more than or less than grey scale range
            if(tempVal < 0) tempVal = 0;
            if(tempVal > 255) tempVal = 255;

            // Generate red value at location i,j which is green pixels
            imageOut[i][k][1] =  (unsigned char)tempVal;
        }
    }
    // Blue pixels' horz. line
    for(int i = 1; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            // Green at blue pixels
            int tempVal = (int(imageExtend[i+2][j+2-1][0]) + int(imageExtend[i+2+1][j+2][0]) + int(imageExtend[i+2][j+2+1][0]) + int(imageExtend[i+2-1][j+2][0]))/4;
            // Add Correction term.
            tempVal = tempVal + (int(imageExtend[i+2][j+2][0]) - (int(imageExtend[i+2][j+2-2][0]) + int(imageExtend[i+2+2][j+2][0]) + int(imageExtend[i+2][j+2+2][0]) + int(imageExtend[i+2-2][j+2][0]))/4)/2;

            // Prevent value more than or less than grey scale range
            if(tempVal < 0) tempVal = 0;
            if(tempVal > 255) tempVal = 255;

            // Generate red value at location i,j which is green pixels
            imageOut[i][j][1] =  (unsigned char)tempVal;
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            // Green at green pixels which in the same horz. line as blue pixels.
            imageOut[i][k][1] = imageExtend[i+2][k+2][0];
        }
    }

    /** Blue Layer
    ** Green, Blue, Red pixels' red values estimation by using Red pixels
    **/
    // Red pixels' horz. line
    for(int i = 0; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            // Add two nearby blue
            int tempVal = (int(imageExtend[i+2-1][j+2][0]) + int(imageExtend[i+2+1][j+2][0]))/2;
            // Add Correction term
            tempVal = tempVal + int(imageExtend[i+2][j+2][0])*5/8 + int(imageExtend[i+2][j+2-2][0])/16 + int(imageExtend[i+2][j+2+2][0])/16;
            tempVal = tempVal - (int(imageExtend[i+2-1][j+2-1][0]) + int(imageExtend[i+2+1][j+2-1][0]) + int(imageExtend[i+2+2][j+2][0]) + int(imageExtend[i+2+1][j+2+1][0]) + int(imageExtend[i+2-1][j+2+1][0]) + int(imageExtend[i+2-2][j+2][0]))/8;

            // Prevent value more than or less than grey scale range
            if(tempVal < 0) tempVal = 0;
            if(tempVal > 255) tempVal = 255;

            imageOut[i][j][2] = (unsigned char)tempVal;
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            // Add four nearby blue
            int tempVal = (int(imageExtend[i+2-1][k+2-1][0]) + int(imageExtend[i+2+1][k+2-1][0]) + int(imageExtend[i+2+1][k+2+1][0]) + int(imageExtend[i+2-1][k+2+1][0]))/4;
            // Add correction term to red value
            tempVal = tempVal + 3/4*int(imageExtend[i+2][k+2][0]) - 3/4*((int(imageExtend[i+2][k+2-2][0]) + int(imageExtend[i+2+2][k+2][0]) + int(imageExtend[i+2][k+2+2][0]) + int(imageExtend[i+2-2][k+2][0]))/4);

            // Prevent value more than or less than grey scale range
            if(tempVal < 0) tempVal = 0;
            if(tempVal > 255) tempVal = 255;

            // Generate red value at location i,j which is blue pixels
            imageOut[i][k][2] =  (unsigned char)tempVal;
        }
    }
    // Blue pixels' horz. line
    for(int i = 1; i < ImageHeight; i = i+2){
        for(int j = 0; j < ImageWidth; j = j+2){
            // Blue at blue pixels
            imageOut[i][j][2] = imageExtend[i+2][j+2][0];
        }
        for(int k = 1; k < ImageWidth; k = k+2){
            // Add two nearby blue
            int tempVal = (int(imageExtend[i+2][k+2-1][0]) + int(imageExtend[i+2][k+2+1][0]))/2;
            // Add Correction term
            tempVal = tempVal + int(imageExtend[i+2][k+2][0])*5/8 + int(imageExtend[i+2-2][k+2][0])/16 + int(imageExtend[i+2+2][k+2][0])/16;
            tempVal = tempVal - (int(imageExtend[i+2][k+2-2][0]) + int(imageExtend[i+2+1][k+2-1][0]) + int(imageExtend[i+2+1][k+2+1][0]) + int(imageExtend[i+2][k+2+2][0]) + int(imageExtend[i+2-1][k+2+1][0]) + int(imageExtend[i+2-1][k+2-1][0]))/8;

            // Prevent value more than or less than grey scale ranges
            if(tempVal < 0) tempVal = 0;
            if(tempVal > 255) tempVal = 255;

            imageOut[i][k][2] = (unsigned char)tempVal;
        }
    }


    // Todo: Store image
    if(!(file = fopen(argv[2],"wb"))){
        cout << "Cannot open file: " << endl;
        exit(1);
    }
    fwrite(imageOut, sizeof(unsigned char), (ImageHeight)*(ImageWidth)*3, file);
    fclose(file);
    
    return 0;
}    