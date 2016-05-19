#ifndef FRAMEPROCESSOR_HPP
#define FRAMEPROCESSOR_HPP

#include "ControlWindow.hpp"

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

class FrameProcessor {

private:

    Mat frameGray;
    int maxBlue;
    int maxRed;
    int maxGreen;
    bool showBalls;

    Mat fgMaskMOG; //fg mask generated by MOG method
	Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
	Ptr<BackgroundSubtractor> pMOG; //MOG Background subtractor
	Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor

	ControlWindow controlWindow;

    // Hough Circles Parameters
    int cannyThreshold;
    int accumulatorThreshold;
    int minCircleSize;
    int maxCircleSize;
    int distanceBetweenCircleCenters;
    int dp;

    // inRange Parameters
    int minSaturation;
    int maxSaturation;
    int minValue;
    int maxValue;
    int minHue;
    int maxHue;

    vector<Vec3f> allBalls;
    vector<Vec3f> whiteBall;

    void findMostFrequentColor (Mat frame);
    void updateControlVariables();
    Mat segmentTable (Mat frame);
    vector<Vec3f> findAllBalls(Mat frame);
    vector<Vec3f> findWhiteBall(Mat frame);
    Mat backgroundSubtract(Mat frame);

public:
    FrameProcessor();
    void processFrame(Mat frame);
    vector<Vec3f> getWhiteBalls();
    vector<Vec3f> getAllBalls();
    void findLines(Mat frame);
};

#endif // OBJECTFINDER_H
