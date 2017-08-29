#include <iostream>
#include <string>
#include "DensityMap.h"
#include <csv/CSVparser.h>
#include <fstream>

using namespace std;

int main(int argc, char*argv[]) {
    string bin = "/home/srq/Datasets/people-counting-1/training-with-densities/147_0_0.bin";
    ifstream f(bin);
    char* c = new char [333*333*4];
    f.read(c, 333*333*4);

    float * f2 = reinterpret_cast<float*> (c);
    float sum;
    for ( int i = 0; i < 333*333; i++) {
        sum+=f2[i];
    }

    cout<<"Sum is "<<sum<<endl;
    delete []c;


//    DensityMap(imageFile,pointsFile,outputImageSuffix).execute();

    return 0;
}