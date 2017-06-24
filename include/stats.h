#ifndef STATS_H
#define STATS_H

struct Stats{

    int matches;
    int inliers;
    int keypoints;
    double ratio;

    Stats():matches(0), inliers(0), keypoints(0), ratio(0){ }

    Stats& operator+=(const Stats& op){

        matches += op.matches;
        inliers += op.inliers;
        ratio += op.ratio;
        keypoints += op.keypoints;

        return *this;
    }

    Stats& operator/=(int num){

        matches /= num;
        inliers /= num;
        ratio /= num;
        keypoints /= num;

        return *this;
    }
};

#endif