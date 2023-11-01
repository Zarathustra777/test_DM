#include <fstream>
#include "include/headers.h"
#include "include/utils.h"

int main(int argc, char** argv ){

    if( argc < 2) {
        printf("usage: %s <directory path>\n", argv[0]);
        exit(-1);
    }

    std::string image_path = cv::samples::findFile(argv[1]);

    cv::Mat img = imread(image_path,cv::IMREAD_GRAYSCALE);

    if(img.empty()){
        std::cout << "Could not read the image: " << image_path << std::endl;
        return -1;
    }
    //use config.txt for constant
    std::string config = cv::samples::findFile("config.txt");
    Utils::setConstants(config);

    cv::Mat imgFixed;
    if (!img.isContinuous()) {
        img = img.clone();
    }

    imgFixed = std::move(getMapBugPixels(img));

    namedWindow("source img", cv::WINDOW_NORMAL);
    imshow("source img", img);

    namedWindow("fix img", cv::WINDOW_NORMAL);
    imshow("fix img", imgFixed);

    cv::waitKey(0); // Wait for a keystroke in the window
    return 0;
}