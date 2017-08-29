//
// Created by Shah Rukh Qasim on 8/1/17.
//

#include <iostream>
#include <string>
#include "DensityMap2.h"
#include <csv/CSVparser.h>

using namespace std;

int main(int argc, char*argv[]) {
    if (argc != 5)
        return -1;

    string imageFile = argv[1];
    string pointsFile = argv[2];
    string outputPath = argv[3];
    string outputPath2 = argv[4];

    DensityMap2(imageFile,pointsFile,outputPath,outputPath2).execute();

    return 0;
}