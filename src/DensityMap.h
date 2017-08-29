//
// Created by Shah Rukh Qasim on 8/1/17.
//

#include <iostream>
#include <string>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include "csv/CSVparser.h"
#include <vector>


#ifndef DENSITYMAP_DENSITYMAP_H
#define DENSITYMAP_DENSITYMAP_H


class DensityMap {
    std::string imagePath;
    std::string pointsPath;
    std::string outputPath;

    cv::Mat image;
    std::vector<double> xCoordinates;
    std::vector<double> yCoordinates;

    std::vector<double> closestDistance;

    cv::Mat densityMap;

public:
    DensityMap(std::string imagePath, std::string pointsPath, std::string outputImageSuffix);
    void execute();
    void computeClosestDistance();
    void computeDensityMap();
    void loadData();

};


#endif //DENSITYMAP_DENSITYMAP_H
