// Problem 1. C. Cumulative Probability Based Histogram Equalization
// Author: Zifan Wang
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <fstream>

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

    // TODO: Read raw image into program and store it in array
    unsigned char imageData[ImageHeight][ImageWidth][BytesPerPixel];   // Image array to store .raw file
    if(file = fopen(argv[1],"rb")){
        fread(imageData,sizeof(unsigned char), ImageHeight*ImageWidth*BytesPerPixel, file);
    }else{
        cout << "Error opening file" << endl;
        exit(1);
    }

    // TODO: CPB Histogram Equalization
    // Calculate Image Histogram
    int histTable[256][BytesPerPixel]; // grey-scale intensity: 0-255
    // init. every intensity to 0
    for(int channel = 0; channel < BytesPerPixel; channel++){
        for(int i = 0; i < 256; i++){
            histTable[i][channel] = 0;
        }
    }
    // loop through all pixels and accumulate the count of the same intensity values
    for(int channel = 0; channel < BytesPerPixel; channel++){
        for(int i = 0; i < ImageHeight; i++){
            for(int j = 0; j < ImageWidth; j++){
                histTable[int(imageData[i][j][channel])][channel]++;
            }
        }
    }

    // Get cdf
    double cdf[256][BytesPerPixel];
    for(int channel = 0; channel < BytesPerPixel; channel++){
        for(int i = 0; i < 256; i++){
            double temp = double(histTable[i][channel])/ImageHeight/ImageWidth;
            if(i == 0) cdf[i][channel] = temp;
            else cdf[i][channel] = cdf[i-1][channel]+temp;
        }
    }


    // CPB Method requires each intensity value should have the same number of pixels in the image 
    // So, define this value as following
    int requiredNumPixels = ImageHeight*ImageWidth/256;     // If image is large, change int to long 
    unsigned char imageOut[ImageHeight][ImageWidth][BytesPerPixel];
    for(int channel = 0; channel < BytesPerPixel; channel++){
        int num = 0; // Keep tracking with how many pixels already in the specific intensity.
        int intensity = 0;
        for(int i = 0; i < 256; i++){
            for(int h = 0; h < ImageHeight; h++){
                for(int w = 0; w < ImageWidth; w++){
                    if(int(imageData[h][w][channel]) == i){
                        // if I(h,w) == intensity 
                        if(num < requiredNumPixels-1) num++;
                        else {
                            num = 0;
                            intensity++;
                            // if(intensity > 255) intensity = 255;
                        }
                        imageOut[h][w][channel] = (unsigned char)intensity;
                    }
                }
            }
        }
    }

    // Hew intensity histogram
    int histTableMod[256][BytesPerPixel]; // grey-scale intensity: 0-255
    // init. every intensity to 0
    for(int channel = 0; channel < BytesPerPixel; channel++){
        for(int i = 0; i < 256; i++){
            histTableMod[i][channel] = 0;
        }
    }
    // loop through all pixels and accumulate the count of the same intensity values
    for(int channel = 0; channel < BytesPerPixel; channel++){
        for(int i = 0; i < ImageHeight; i++){
            for(int j = 0; j < ImageWidth; j++){
                histTableMod[int(imageOut[i][j][channel])][channel]++;
            }
        }
    }

    ofstream outputHist("outputHistogram.txt");
    if(outputHist.is_open()){
        for(int channel = 0; channel < BytesPerPixel; channel++){
            outputHist << "Channel: " << channel << endl;
            for(int i = 0; i < 256; i++){
                outputHist << histTableMod[i][channel] << endl;
            }
        }
        outputHist.close();
    }

    int sumHistTable[256][BytesPerPixel];
    for(int channel = 0; channel < BytesPerPixel; channel++){
        int sum = 0;
        for(int i = 0; i < 256; i++){
            sum += histTableMod[channel][i];
            sumHistTable[channel][i] = sum;
        }
    }

    ofstream tfTxt("cpb_h.txt");
    if(tfTxt.is_open()){
        for(int channel = 0; channel < BytesPerPixel; channel++){
            tfTxt << "Channel: " << channel << endl;
            for(int i = 0; i < 256; i++){
                tfTxt << sumHistTable[channel][i] << endl;
            }
        }
        tfTxt.close();
    }


    // TODO: Store image
    if(!(file = fopen(argv[2],"wb"))){
        cout << "Cannot open file: " << endl;
        exit(1);
    }
    fwrite(imageOut, sizeof(unsigned char), (ImageHeight)*(ImageWidth)*3, file);
    fclose(file);
    
    return 0;
}    