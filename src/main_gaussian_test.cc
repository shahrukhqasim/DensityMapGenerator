// range heap example
#include <iostream>     // std::cout
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector
#include <queue>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;


float sumIt(Mat m) {
    float* densityMapValues = reinterpret_cast<float*>(m.data);
    float sum = 0;
    for(int i = 0; i<m.rows*m.cols; i++) {
        sum += densityMapValues[i];
    }
    return sum;
}

int main () {

    cv::Mat imageTest(30,30,CV_32FC1);
    imageTest.at<float>(15,15)=1;

    cv::Mat imageOut;
    GaussianBlur(imageTest, imageOut, Size(3,3), 3.0);

    cout<<sumIt(imageOut+imageTest);



    return 0;
}