// Problem 3. C. Bilateral Filter
// Author: Zifan Wang

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;


int main(int argc, char *argv[]){
    // Define variables
    FILE *file;                                  // Read in file.
    int BytesPerPixel = 3;                       // image is in gray scale, set this para. to 1. If RGB set to 3.
    int ImageHeight = 300, ImageWidth = 390;     // Image size
    int FilterHeight = 3, FilterWidth = 3;       // A square N*N size filter
    double sigma = 3.0;                          // Gaussian Filter's sigma and NLM sigma value
    const double PI  = 3.141592653589793238463;  // Declare pi
    double sigma_c = 2.0;                        // bilateral filter's sigma_c 
    double sigma_s = 3.0;                        // bilateral filter's sigma_s       
    int searchHeight = 3, searchWidth = 3;       // NonlocalMean filter seach size
    double filterParam = 3.0;                    // NonlocalMean filter filtering parameter

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
		if (argc == 6){
			ImageHeight = atoi(argv[4]);
            ImageWidth = atoi(argv[5]);
		}
        if (argc == 7){
			ImageHeight = atoi(argv[4]);
            ImageWidth = atoi(argv[5]);
            FilterHeight = FilterWidth = atoi(argv[6]);
		}
        if (argc == 8){
			ImageHeight = atoi(argv[4]);
            ImageWidth = atoi(argv[5]);
            FilterHeight = FilterWidth = atoi(argv[6]);
            sigma = stod(argv[7]);
        }
        if (argc == 10){
			ImageHeight = atoi(argv[4]);
            ImageWidth = atoi(argv[5]);
            FilterHeight = FilterWidth = atoi(argv[6]);
            sigma = stod(argv[7]);
            sigma_c = stod(argv[8]);
            sigma_s = stod(argv[9]);
        }
        if (argc == 12){
			ImageHeight = atoi(argv[4]);
            ImageWidth = atoi(argv[5]);
            FilterHeight = FilterWidth = atoi(argv[6]);
            sigma = stod(argv[7]);
            sigma_c = stod(argv[8]);
            sigma_s = stod(argv[9]);
            searchHeight = searchWidth = atoi(argv[10]);
            filterParam = stod(argv[11]);
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

    // TDOO: Image Extension based on Filter Size
    unsigned char imageTemp[ImageHeight+FilterHeight*2][ImageWidth][BytesPerPixel];
    unsigned char imageExtend[ImageHeight+FilterHeight*2][ImageWidth+FilterWidth*2][BytesPerPixel];
    for(int channel = 0; channel < BytesPerPixel; channel++){
        // Extend height(row)
        for(int h = 0; h < ImageHeight+FilterHeight*2; h++){
            for(int w = 0; w < ImageWidth; w++){
                if(h < FilterHeight){
                    int actHeight = FilterHeight - h - 1;
                    imageTemp[h][w][channel] = imageData[actHeight][w][channel];
                }
                else if(h > ImageHeight+FilterHeight-1){
                    int actHeight = ImageHeight - (h-ImageHeight) + (FilterHeight-1);
                    imageTemp[h][w][channel] = imageData[actHeight][w][channel];
                }
                else imageTemp[h][w][channel] = imageData[h-FilterHeight][w][channel];
            }
        }
        // Extend width(col)
        for(int w = 0; w < ImageWidth+FilterWidth*2; w++){
            for(int h = 0; h < ImageHeight+FilterHeight*2; h++){
                if(w < FilterWidth){
                    int actWidth = FilterWidth - w - 1;
                    imageExtend[h][w][channel] = imageTemp[h][actWidth][channel];
                }
                else if(w > ImageWidth+FilterWidth-1){
                    int actWidth = ImageWidth - (w-ImageWidth) + (FilterWidth-1);
                    imageExtend[h][w][channel] = imageTemp[h][actWidth][channel];
                }
                else{
                    imageExtend[h][w][channel] = imageTemp[h][w-FilterWidth][channel];
                }
            }
        }
    }

    // Output Image
    unsigned char imageOut[ImageHeight][ImageWidth][BytesPerPixel];
    int FilterSize = FilterHeight*FilterWidth;

    for(int channel = 0; channel < BytesPerPixel; channel++){
        for(int h = 0; h < ImageHeight; h++){
            for(int w = 0; w < ImageWidth; w++){
                double weight = 0;
                double num = 0;
                // Go through filter
                for(int fh = -FilterHeight/2; fh <= FilterHeight/2; fh++){
                    for(int fw = -FilterWidth/2; fw <= FilterWidth/2; fw++){
                        double euclideanDis = 0;
                        // Go through searching window
                        for(int sh = -searchHeight/2; sh <= searchHeight/2; sh++){
                            for(int sw = -searchWidth/2; sw <= searchWidth/2; sw++){
                                double Ga = exp(-(sh*sh+sw*sw)/(2*sigma*sigma))/(sqrt(2*PI)*sigma);
                                euclideanDis = euclideanDis + Ga*pow((double(imageExtend[h+FilterHeight+sh][w+FilterWidth+sh][channel])-double(imageExtend[h+FilterHeight+fh+sh][w+FilterWidth+fw+sw][channel])),2);
                            }
                        }
                        // euclideanDis = euclideanDis/FilterHeight/FilterSize;
                        weight = weight + exp(-euclideanDis/(filterParam*filterParam));
                        num = num + double(imageExtend[h+FilterHeight+fh][w+FilterHeight+fw][channel])*exp(-euclideanDis/(filterParam*filterParam));
                    }
                }
                imageOut[h][w][channel] = num/weight;
            }
        }
    }
    

    // Todo: Store image
    if(!(file = fopen(argv[2],"wb"))){
        cout << "Cannot open file: " << endl;
        exit(1);
    }
    fwrite(imageOut, sizeof(unsigned char), (ImageHeight)*(ImageWidth)*BytesPerPixel, file);
    fclose(file);
    
    return 0;
} 
