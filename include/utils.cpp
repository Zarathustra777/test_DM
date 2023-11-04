#include "utils.h"
#include "constant.h"
#include <fstream>
namespace Utils{

    float linInterp2D(int p1,int p2,int x){
        float w = static_cast<float>(std::abs(x - p1))  / static_cast<float>(std::abs(p2 - p1));
        return w;
    }

    bool isLine(cv::Point2i p1, cv::Point2i p2){
        return p1.x != -1 && p2.x != -1 && p1.y != -1 && p2.y != -1;
    }

    bool isPoint(cv::Point2i p1){
        return p1.x != -1 && p1.y != -1;
    }

    bool checkMaxDistance(int start_i, int end_i, int start_j, int end_j){
        return std::abs(end_i - start_i) < constant::MAX_DISTANCE
            && std::abs(end_j - start_j) < constant::MAX_DISTANCE;
    }
    bool checkDistanceDiffthreshold(double len1, double len2){
        return  std::abs(len1 - len2) < constant::DISTANCE_DIFFERENCE_THRESHOLD;
    }
    void setConstants(std::string & fileName){
        std::ifstream cFile (fileName);
        if (cFile.is_open()){
            cFile>>constant::MAX_DISTANCE;
            cFile>>constant::DISTANCE_DIFFERENCE_THRESHOLD;
        }
        else{
            std::cerr << "Couldn't open config file for reading.\n";
        }
    }
    bool  checkBounds(int i,int j,cv::Point2i start, cv::Mat_<uchar> Pixel){
       return  i >= 0 && j >= 0 && i < Pixel.rows && j < Pixel.cols &&
        checkMaxDistance(i, start.y, j, start.x);
    }
}//end Utils