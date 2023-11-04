#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

namespace Utils{
    float linInterp2D(int p1,int p2,int x);

    bool isLine(cv::Point2i p1, cv::Point2i p2);

    bool isPoint(cv::Point2i p1);

    bool checkMaxDistance(int start_i, int end_i, int start_j, int end_j);

    bool checkDistanceDiffthreshold(double len1, double len2);

    void setConstants(std::string & fileName);

    bool  checkBounds(int i,int j, cv::Point2i start, cv::Mat_<uchar> Pixel);
}

