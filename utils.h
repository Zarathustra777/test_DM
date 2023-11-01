#ifndef UTILS_H
#define UTILS_H
#include "headers.h"
namespace Utils{
    float linInterp2D(int p1,int p2,int x);

    bool isLine(cv::Point2i p1, cv::Point2i p2);

    bool isPoint(cv::Point2i p1);

    bool checkMaxDistance(int start_i, int end_i, int start_j, int end_j);

    bool checkDistanceDiffthreshold(double len1, double len2);

    void setConstants(std::string & fileName);
}
#endif //UTILS_H
