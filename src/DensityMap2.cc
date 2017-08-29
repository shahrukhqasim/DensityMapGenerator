//
// Created by Shah Rukh Qasim on 8/1/17.
//

#include "DensityMap2.h"
#include <fstream>
#include <queue>

using namespace cv;
using namespace std;

DensityMap2::DensityMap2(std::string imagePath, std::string pointsPath, std::string outDensityMapPath, std::string outDensityMapPathDisplay) {
    this->imagePath = imagePath;
    this->pointsPath = pointsPath;
    this->outputPath = outDensityMapPath;
    this->outputPathDisplay = outDensityMapPathDisplay;
}


void DensityMap2::execute() {
    loadData();
    computeClosestDistance();
    computeDensityMap();

    ofstream densityStream(outputPath, ios::binary | ios::out);
    densityStream.write(reinterpret_cast<char *>(densityMap.data), densityMap.cols * densityMap.rows * sizeof(float));
    imwrite(outputPathDisplay, densityMapDisplay);

}

double inline evaluatePdf(double standardDeviation, double distance) {
    return (1 / (standardDeviation * sqrt(2 * M_PI))) *
           exp(-(distance * distance) / (2 * standardDeviation * standardDeviation));

}

void DensityMap2::computeDensityMap() {
    int rows = image.rows;
    int cols = image.cols;

    Mat densityMap(image.rows, image.cols, CV_32FC1, 0.00);
    Mat densityMapTemp(image.rows, image.cols, CV_32FC1, 0.00);
    Mat imageTempPixel(image.rows, image.cols, CV_32FC1, 0.00);

    float* densityMapValues = reinterpret_cast<float*>(densityMap.data); //new float[rows * cols];

    size_t numPoints = xCoordinates.size();

    for (int i = 0; i < numPoints; i++) {
        double x = xCoordinates[i];
        double y = yCoordinates[i];
        densityMapTemp *= 0;
        imageTempPixel *= 0;
        imageTempPixel.at<float>(y, x) = 1;
        GaussianBlur(imageTempPixel, densityMapTemp, Size(0, 0), closestDistance[i] * 0.02 );
        densityMap += densityMapTemp;
        cout << '\t' << ((i+1)*100.0) / numPoints << "% done!\n";
    }

    float sum = 0;
    for(int i = 0; i<rows*cols; i++) {
        sum += densityMapValues[i];
    }

//    Mat densityMapDisplay;
//    normalize(densityMap, densityMapDisplay, 1, 0,NORM_MINMAX);
//
//    namedWindow("Original Image", CV_WINDOW_NORMAL);
//    imshow("Original Image", densityMapDisplay);
//    waitKey(0);

    Mat densityMapDisp;
    double max, min;
    cv::minMaxIdx(densityMap, &min, &max);
    densityMap.convertTo(densityMapDisp,CV_8UC1, 255 / (max-min), -min);
    namedWindow("DensityMap", CV_WINDOW_NORMAL);
    Mat cm_img0;
    applyColorMap(densityMapDisp, cm_img0, COLORMAP_JET);
//    imshow("DensityMap", cm_img0);
//    waitKey(0);

    this->densityMap = densityMap.clone();
    this->densityMapDisplay = cm_img0;
}

void DensityMap2::computeClosestDistance() {
    size_t numPoints = xCoordinates.size();

    for (size_t i = 0; i < numPoints; i++) {
        double x1 = xCoordinates[i], y1 = yCoordinates[i];
        std::priority_queue<double, std::vector<double>> minimumValues;
        for (size_t j = 0; j < numPoints; j++) {
            if (i == j)
                continue;
            double x2 = xCoordinates[j], y2 = yCoordinates[j];

            double squareDistance = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
            minimumValues.push(squareDistance);
            if(minimumValues.size()>5)
                minimumValues.pop();

        }

        if (minimumValues.empty())
            minimumValues.push(15);

        double sum = 0;
        while (minimumValues.empty() == false)
        {
            sum += minimumValues.top();
            minimumValues.pop();
        }
        double average = minimumValues.size() != 0 ? sum/minimumValues.size() : image.cols*image.cols + image.rows*image.rows;

        if (average==0)
            cout<<"Here comes trouble!"<<endl;
        average = sqrt(average);
        closestDistance.push_back(average);
//        cout<<"M: "<<minimumDistance<<endl;
    }
}

void DensityMap2::loadData() {
    Mat image = imread(imagePath, 1);
    xCoordinates.clear();
    yCoordinates.clear();
    closestDistance.clear();

    vector<double> xCoordinates;
    vector<double> yCoordinates;

    csv::Parser parser(pointsPath, csv::DataType::eFILE);
    vector<string> temp = parser.getHeader();
    xCoordinates.push_back(stof(temp[0]));
    yCoordinates.push_back(stof(temp[1]));

    for (int i = 0; i < parser.rowCount(); i++) {
        vector<string> rowVector;
        for (int j = 0; j < 2; j++) {
            rowVector.push_back(parser[i][j]);
        }

        xCoordinates.push_back(stof(rowVector[0]));
        yCoordinates.push_back(stof(rowVector[1]));
    }

    for(int i = 0; i <xCoordinates.size();i++) {
        circle(image, Point(xCoordinates[i],yCoordinates[i]),3, Scalar(0,0,255));
    }

    this->image = image;
    this->xCoordinates = xCoordinates;
    this->yCoordinates = yCoordinates;
}