#include "pixelsmap.h"
#include "utils.h"
#include "constant.h"

using namespace cv;
using namespace Utils;

Mat getMapBugPixels(Mat &img) {
    const Mat_<uchar> Pixel = img;
    std::vector<BugPixel> pool_bug_pixel;
    for (int row = 0; row < Pixel.rows; ++row) {
        for (int col = 0; col < Pixel.cols; ++col) {
            if (Pixel(row, col) > constant::bug_pixel) {
                pool_bug_pixel.push_back({{col, row}});
            }
        }
    }
    findLine(pool_bug_pixel, img);
    weightingCoefficients(pool_bug_pixel);

    return fixBugPixels(pool_bug_pixel, img);
}

Point2i findFirstGoodPixel(Point2i start, const Mat_<uchar> &Pixel, int step_i, int step_j) {
    int i = start.y;
    int j = start.x;
    Point2i res{-1, -1};
    while (checkBounds(i,j,start,Pixel)) {
        if (Pixel(i, j) <= constant::bug_pixel) {
            res.x = j;
            res.y = i;
            break;
        }
        i += step_i;
        j += step_j;
    }
    return res;
}

void findLine(std::vector<BugPixel> &pool, const Mat_<uchar> &Pixel) {
    for (auto &vec: pool) {
        //find left ---> right
        vec.p1 = findFirstGoodPixel(vec.p0, Pixel, 0, -1);
        vec.p2 = findFirstGoodPixel(vec.p0, Pixel, 0, 1);
        //find bottom --->top
        vec.p3 = findFirstGoodPixel(vec.p0, Pixel, -1, 0);
        vec.p4 = findFirstGoodPixel(vec.p0, Pixel, 1, 0);
    }
    // if vertical and horizontal line is not found
    for (auto &vec: pool) {
        findDiagonalLine(vec, Pixel);
    }
}

void findDiagonalLine(BugPixel &vec, const Mat_<uchar> &Pixel) {
    if (!isLine(vec.p1, vec.p2) && !isLine(vec.p3, vec.p4)) {
        auto tmp = vec;
        //find left_bottom to right_up line
        vec.p1 = findFirstGoodPixel(vec.p0, Pixel, -1, -1);
        vec.p2 = findFirstGoodPixel(vec.p0, Pixel, 1, 1);
        //find right_up to left_bottom line
        vec.p3 = findFirstGoodPixel(vec.p0, Pixel, 1, -1);
        vec.p4 = findFirstGoodPixel(vec.p0, Pixel, -1, 1);
        if (!isLine(vec.p1, vec.p2) && !isLine(vec.p3, vec.p4)) {
            swap(vec, tmp);
        }
    }
}

void weightingCoefficients(std::vector<BugPixel> &pool) {
    for (auto &vec: pool) {
        //for horizontal
        if (isLine(vec.p1, vec.p2)) {
            vec.w1 = linInterp2D(vec.p1.x, vec.p2.x, vec.p0.x);
            vec.w2 = 1 - vec.w1;
        }
        //for vertical
        if (isLine(vec.p3, vec.p4)) {
            //get weighting Coefficients for top
            vec.w3 = linInterp2D(vec.p3.y, vec.p4.y, vec.p0.y);
            vec.w4 = 1 - vec.w3;
        }
        //if two lines found
        if (isLine(vec.p1, vec.p2) && isLine(vec.p3, vec.p4)) {
            chooseLines(vec);
        }
            // if all bad
        else if (!isLine(vec.p1, vec.p2) && !isLine(vec.p3, vec.p4)) {
            useAnyFirstGoodPixel(vec);
        }
    }
}

void useAnyFirstGoodPixel(BugPixel &vec) {
    int count = 0;
    float w = 0;
    for (int i = 0; i < 2; ++i) {
        if (vec.p1.x != -1) {
            vec.w1 = w;
            ++count;
        }
        if (vec.p2.x != -1) {
            vec.w2 = w;
            ++count;
        }
        if (vec.p3.y != -1) {
            vec.w3 = w;
            ++count;
        }
        if (vec.p4.y != -1) {
            vec.w4 = w;
            ++count;
        }
        //get weighting Coefficients
        w = 1.0f / static_cast<float>(count);
    }
}

void chooseLines(BugPixel &vec) {
    double len1 = norm(vec.p1 - vec.p2);
    double len2 = norm(vec.p3 - vec.p4);
    if (checkDistanceDiffthreshold(len1, len2)) { //use two
        vec.w1 *= 0.5;
        vec.w2 *= 0.5;
        vec.w3 *= 0.5;
        vec.w4 *= 0.5;
    } else {//use one line
        if (len2 < len1) {
            vec.w3 = vec.w4 = 0; //
        } else {
            vec.w1 = vec.w2 = 0; //
        }
    }
}

Mat fixBugPixels(std::vector<BugPixel> &pool, Mat &inPutImg) {
    Mat_<uchar> Pixel_out = inPutImg.clone();
    for (auto &vec: pool) {
        float fixPixel = 0;
        if (isPoint(vec.p1)) {
            fixPixel += vec.w1 * Pixel_out(vec.p1.y, vec.p1.x); //for left pixel
            Pixel_out(vec.p0.y, vec.p0.x) = fixPixel;
        }
        if (isPoint(vec.p2)) {
            fixPixel += vec.w2 * Pixel_out(vec.p2.y, vec.p2.x); //for right pixel
            Pixel_out(vec.p0.y, vec.p0.x) = fixPixel;

        }
        if (isPoint(vec.p3)) {
            fixPixel += vec.w3 * Pixel_out(vec.p3.y, vec.p3.x); //for top pixel
            Pixel_out(vec.p0.y, vec.p0.x) = fixPixel;

        }
        if (isPoint(vec.p4)) {
            fixPixel += vec.w4 * Pixel_out(vec.p4.y, vec.p4.x); //for bot pixel
            Pixel_out(vec.p0.y, vec.p0.x) = fixPixel;

        }
    }
    return Pixel_out;
}