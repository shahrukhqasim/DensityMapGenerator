//
// Created by Shah Rukh Qasim on 8/1/17.
//

#include <iostream>
#include <string>
#include "DensityMap.h"
#include <csv/CSVparser.h>

using namespace std;

int main(int argc, char*argv[]) {
    if (argc != 4)
        return -1;

    string imageFile = argv[1];
    string pointsFile = argv[2];
    string outputPath = argv[3];

    DensityMap(imageFile,pointsFile,outputPath).execute();

    return 0;
}