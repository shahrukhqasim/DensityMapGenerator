//
// Created by Shah Rukh Qasim on 8/1/17.
//

#include "DensityMap.h"
#include <fstream>
#include <queue>

using namespace cv;
using namespace std;

DensityMap::DensityMap(string imagePath, string pointsPath, string outputImageSuffix) {
    this->imagePath = imagePath;
    this->pointsPath = pointsPath;
    this->outputPath = outputImageSuffix;
}


void DensityMap::execute() {
    loadData();
    computeClosestDistance();
    computeDensityMap();

    int H = image.rows / 3, W = image.cols / 3;
    Mat imageFull = imread(imagePath, 1);
    int H2 = imageFull.rows / 3, W2 = imageFull.cols / 3;
    float totalSum = 0;
    for(int i = 0; i < 3; i++) {
        for (int k = 0; k < 3; k++) {
            Rect cropArea(i*W, k*H, W, H);
            Rect cropAreaImage(i*W2, k*H2, W2, H2);

            Mat imageCropped = imageFull(cropAreaImage).clone();
            resize(imageCropped, imageCropped, Size(400,400));
            Mat mapCropped = densityMap(cropArea).clone();

            imwrite(outputPath+"/"+to_string(i)+"_"+to_string(k)+".jpg", imageCropped);
            ofstream densityStream(outputPath+"/"+to_string(i)+"_"+to_string(k)+".bin", ios::binary | ios::out);
            densityStream.write(reinterpret_cast<char*>(mapCropped.data), mapCropped.cols*mapCropped.rows* sizeof(float));

            float sum = 0;
            float * densityMapValues = reinterpret_cast<float*>(mapCropped.data);
            for(int i = 0; i<mapCropped.cols*mapCropped.rows; i++) {
                sum += densityMapValues[i];
            }
//            cout<<"Sum "<<sum<<endl;
            totalSum += sum;
        }
    }
    cout<<"Total sum " << totalSum<<endl;
}

double inline evaluatePdf(double standardDeviation, double distance) {
    return (1 / (standardDeviation * sqrt(2 * M_PI))) *
           exp(-(distance * distance) / (2 * standardDeviation * standardDeviation));

}

void DensityMap::computeDensityMap() {
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
        GaussianBlur(imageTempPixel, densityMapTemp, Size(31, 31), closestDistance[i] * 0.3);
        densityMap += densityMapTemp;
        cout << '\t' << ((i+1)*100.0) / numPoints << "% done!\n";
    }

    float sum = 0;
    for(int i = 0; i<rows*cols; i++) {
        sum += densityMapValues[i];
    }
    cout<<"Sum before: "<<sum<<endl;
//    densityMap *= numPoints/sum;
//
//    sum = 0;
//
//    for(int i = 0; i<rows*cols; i++) {
//        sum += densityMapValues[i];
//    }
    cout<<"Sum after: "<<sum<<endl;

//    Mat densityMapDisplay;
//    normalize(densityMap, densityMapDisplay, 1, 0,NORM_MINMAX);
//
//    Mat orig = imread(imagePath, 1);
//    resize(orig, orig, Size(1000,1000));
//
//    namedWindow("Original Image", CV_WINDOW_NORMAL);
//    imshow("Original Image", orig);
//    waitKey(0);
//
//    Mat densityMapDisp;
//    double max, min;
//    cv::minMaxIdx(densityMap, &min, &max);
//    densityMap.convertTo(densityMapDisp,CV_8UC1, 255 / (max-min), -min);
//    namedWindow("DensityMap", CV_WINDOW_NORMAL);
//    Mat cm_img0;
//    applyColorMap(densityMapDisp, cm_img0, COLORMAP_JET);
//    imshow("DensityMap", cm_img0);
//    waitKey(0);

    this->densityMap = densityMap;
}

void DensityMap::computeClosestDistance() {
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

void DensityMap::loadData() {
    Mat image = imread(imagePath, 1);
    xCoordinates.clear();
    yCoordinates.clear();
    closestDistance.clear();

    double resizeTo = 150;

    double xScale = resizeTo / image.cols;
    double yScale = resizeTo / image.rows;

    resize(image, image, Size(resizeTo,resizeTo));

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

    for (auto &i : xCoordinates) {
        i *= xScale;
    }
    for (auto &i : yCoordinates) {
        i *= yScale;
    }

    for(int i = 0; i <xCoordinates.size();i++) {
        circle(image, Point(xCoordinates[i],yCoordinates[i]),3, Scalar(0,0,255));
    }

    this->image = image;
    this->xCoordinates = xCoordinates;
    this->yCoordinates = yCoordinates;
}