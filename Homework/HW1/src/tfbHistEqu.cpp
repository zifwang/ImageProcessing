// Problem 1. C. Transfer Function Based Histogram Equalization
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

    // TODO: Read raw image into program and store it in array
    unsigned char imageData[ImageHeight][ImageWidth][BytesPerPixel];   // Image array to store .raw file
    if(file = fopen(argv[1],"rb")){
        fread(imageData,sizeof(unsigned char), ImageHeight*ImageWidth*BytesPerPixel, file);
    }else{
        cout << "Error opening file" << endl;
        exit(1);
    }

    // TODO: Transfer-function-based Hist. Equalization
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
    // // print function
    // for(int channel = 0; channel < BytesPerPixel; channel++){
    //     for(int i = 0; i < 256; i++){
    //         cout << i << ": " << histTable[channel][i] << endl;
    //     }
    // }
    
    // build a LUT for cumulative histogram 
    int sumHistTable[256][BytesPerPixel];
    for(int channel = 0; channel < BytesPerPixel; channel++){
        int sum = 0;
        for(int i = 0; i < 256; i++){
            sum += histTable[i][channel];
            sumHistTable[i][channel] = sum;
        }
    }

    // for(int channel = 0; channel < BytesPerPixel; channel++){
    //     for(int i = 0; i < 256; i++){
    //         cout << sumHistTable[channel][i] << endl;
    //     }
    // }

    // transform image using using sum hisrotgram
    int imageSize = ImageHeight*ImageWidth;
    float scale = 255.0 / imageSize;
    unsigned char imageOut[ImageHeight][ImageWidth][BytesPerPixel];

    for(int channel = 0; channel < BytesPerPixel; channel++){
        for(int h = 0; h < 256; h++){
            for(int i = 0; i < ImageHeight; i++){
                for(int j = 0; j < ImageWidth; j++){
                    if(int(imageData[i][j][channel]) == h) {
                        // if pixels intensity == h, then assign modify intensity to the output image.
                        imageOut[i][j][channel] = (unsigned char) (double(sumHistTable[h][channel])*scale);
                        // cout << i << "," << j << "    ";
                        // cout << int(imageOut[i][j][channel]) << endl;
                    }
                }        
            }
        }
    }

    // Test Function    
    // for(int channel = 0; channel < BytesPerPixel; channel++){
    //     for(int i = 0; i < ImageHeight; i++){
    //         for(int j = 0; j < ImageWidth; j++){
    //             cout << i << "," << j << " Intensity: " << int(imageOut[i][j][channel]) << endl;
    //         }
    //     }
    // }

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
    // // print function
    // for(int channel = 0; channel < BytesPerPixel; channel++){
    //     for(int i = 0; i < 256; i++){
    //         cout << i << ": " << histTableMod[channel][i] << endl;
    //     }
    // }

    // Transfer function
    double tf[256][BytesPerPixel];
    for(int channel = 0; channel < BytesPerPixel; channel++){
        for(int i = 0; i < 256; i++){
            tf[i][channel] = double(sumHistTable[i][channel])/imageSize;
        }
    }
    // // print function
    // for(int channel = 0; channel < BytesPerPixel; channel++){
    //     for(int i = 0; i < 256; i++){
    //         cout << i << ": " << tf[i][channel] << endl;
    //     }
    // }


    // Todo: Store image
    if(!(file = fopen(argv[2],"wb"))){
        cout << "Cannot open file: " << endl;
        exit(1);
    }
    fwrite(imageOut, sizeof(unsigned char), (ImageHeight)*(ImageWidth)*3, file);
    fclose(file);
    
    return 0;
}    