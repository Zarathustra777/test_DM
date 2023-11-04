#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

struct BugPixel{
    cv::Point2i p0;     //mid
    cv::Point2i p1;     //left
    cv::Point2i p2;     //right
    cv::Point2i p3;     //bot
    cv::Point2i p4;     //top
    //point weights p1,p2,p3,p4
    float w1 = 0;
    float w2 = 0;
    float w3 = 0;
    float w4 = 0;
};

cv::Mat getMapBugPixels(cv::Mat &img );

cv::Point2i findFirstGoodPixel(cv::Point2i start,const cv::Mat_<uchar>& Pixel, int step_i, int step_j);

void findLine(std::vector<BugPixel> & pool,const cv::Mat_<uchar>& Pixel);

void findDiagonalLine(BugPixel & vec,const cv::Mat_<uchar>& Pixel );
void chooseLines(BugPixel & vec);
void useAnyFirstGoodPixel(BugPixel & vec);

void weightingCoefficients(std::vector<BugPixel> & pool);

cv::Mat fixBugPixels(std::vector<BugPixel> & pool, cv::Mat & inPutImg);


