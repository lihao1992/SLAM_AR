#include <iostream>

#include <opencv2/opencv.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "../include/ORBSLAM/System.h"

using namespace std;
using namespace cv;

cv::Mat K, R, tvec;

cv::Mat cvToGl = cv::Mat::eye(4, 4, CV_64F);

glm::mat4 getViewMatrix(bool slamMode){

    glm::mat4 V;

    cv::Mat viewMatrix = cv::Mat::zeros(4, 4, CV_64FC1);

    if (slamMode){

        R.convertTo(R, CV_64F);
        tvec.convertTo(tvec, CV_64F);

        for(unsigned int row = 0; row < 3; ++row){

            for(unsigned int col = 0; col < 3; ++col){

                viewMatrix.at<double>(row, col) = R.at<double>(row, col);
            }
            viewMatrix.at<double>(row, 3) = tvec.at<double>(row, 0);
        }
        viewMatrix.at<double>(3, 3) = 1.0f;

    }
    else 
        viewMatrix = cvToGl;

    viewMatrix.convertTo(viewMatrix, CV_32F);

    for(int i = 0; i < 4; ++i){

        for(int j = 0; j < 4; ++j){

            V[i][j] = viewMatrix.at<float>(j, i); // 转换矩阵是为了后面变换坐标系时，左乘 vec4的向量时，保证 旋转和平移 分开与坐标相乘
        }
    }
    return V; 

}
