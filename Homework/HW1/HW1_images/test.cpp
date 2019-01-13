#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(){
    // Define variables
    FILE *file;         
    long size;
    cout << "Hello";
    if(file = fopen("cat_ori_mod.raw","rb")){
        cout << "World" << endl;
        fseek (file, 0, SEEK_END);
        size = ftell(file);
        cout << size << endl;
    }
    else {
        cout << "Error opening file" << endl;
    }
    fclose(file);

    return 0;
}             