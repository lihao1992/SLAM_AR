#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <opencv2/opencv.hpp>
#include "../include/planar_tracking.h"
#include "../include/utils.h"

using namespace std;
using namespace cv;


bool Tracker::process(const cv::Mat img, bool slamMode){

    if(slamMode)
        return 1;
    
    else
        cout << "slam mode is not be opened." << endl;

    return 0;
}