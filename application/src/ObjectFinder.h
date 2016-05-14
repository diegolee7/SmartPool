#ifndef OBJECTFINDER_H
#define OBJECTFINDER_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class ObjectFinder {

private:
    // window name
    static const string windowName;

    // trackbars names
    static const string cannyThresholdTrackbarName;
    static const string accumulatorThresholdTrackbarName;
    static const string minCircleSizeTrackbarName;
    static const string maxCircleSizeTrackbarName;
    static const string distanceBetweenCircleCentersTrackbarName;

    // initial values of the parameters of interests.
    static const int cannyThresholdInitialValue = 36;
    static const int accumulatorThresholdInitialValue = 17;
    static const int minCircleSizeInitialValue = 27;
    static const int maxCircleSizeInitialValue = 27;
    static const int distanceBetweensCentersInitialValue = 45;

    // max values of the parameters of interests.
    static const int maxAccumulatorThreshold = 400;
    static const int maxCannyThreshold = 255;
    static const int maxMinCircleSize = 100;
    static const int maxMaxCircleSize = 100;
    static const int maxDistanceBetweenCircleCenters = 100;

    // declare and initialize parameters that are subjects to change
    int cannyThreshold;
    int accumulatorThreshold;
    int minCircleSize;
    int maxCircleSize;
    int distanceBetweenCircleCenters;

    //
    int minSaturation;
    int maxSaturation;
    int minValue;
    int maxValue;
    int minHue;
    int maxHue;
    int dp;

    Mat frameGray;
    void initControlWindow();
    int maxBlue;
    int maxRed;
    int maxGreen;

    bool showBalls;

public:
    ObjectFinder();
    vector<Vec3f> getCircles(Mat frame);
    void findMostFrequentColor (Mat frame) ;
    Mat segmentTable (Mat frame);
    vector<Vec3f> findWhiteBall(Mat frame);
};

#endif // OBJECTFINDER_H
