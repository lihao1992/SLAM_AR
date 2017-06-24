#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core.hpp>
#include <vector>
#include <iomanip>
#include "stats.h"

using namespace std;
using namespace cv;

vector<Point2f> Points(vector<KeyPoint> keypoints);

vector<Point2f> Points(vector<KeyPoint> keypoints){

    vector<Point2f> res;
    for(unsigned int i = 0; i < keypoints.size(); ++i){

        res.push_back(keypoints[i].pt);
    }
    return res;
}

#endif