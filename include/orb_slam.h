#ifndef ORB_SLAM_H
#define ORB_SLAM_H

#include "ORBSLAM/System.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

glm::mat4 getViewMatrix(bool slamMode);

// Mat getCameraMatrix();

#endif