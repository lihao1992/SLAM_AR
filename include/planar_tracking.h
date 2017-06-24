#ifndef PLANAR_TRACKING_H
#define PLANAR_TRACKING_H



using namespace std;
using namespace cv;

class Tracker
{
public:
    Tracker(Ptr<Feature2D> _detector, Ptr<DescriptorMatcher> _matcher) :
            detector(_detector),
            matcher(_matcher)
    {}
    //Tracker(){}

   // void setFirstFrame(const char * first_frame_path, const char * bb_path);

    bool process(const Mat img, bool slamMode);

   // glm::mat4 getInitModelMatrix();

   /* Ptr<Feature2D> getDetector() {
        return detector;
    } */

protected:

  //  Mat K, rvec, tvec;
    Ptr<Feature2D> detector;
    Ptr<DescriptorMatcher> matcher;
  //  Mat first_frame, first_desc;
  //  vector<KeyPoint> first_kp;
  //  vector<Point2f> object_bb;
    
};

#endif 