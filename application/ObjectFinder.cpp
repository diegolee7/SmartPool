#include "ObjectFinder.h"

//Window Names
const string ObjectFinder::windowName = "Object Finder Parameters";

//TrackBar Names
const string ObjectFinder::cannyThresholdTrackbarName = "Canny threshold";
const string ObjectFinder::accumulatorThresholdTrackbarName = "Accumulator Threshold";
const string ObjectFinder::maxCircleSizeTrackbarName = "Maximum Radius Size";
const string ObjectFinder::minCircleSizeTrackbarName = "Minimum Radius Size";
const string ObjectFinder::distanceBetweenCircleCentersTrackbarName = "Distance Between centers";


ObjectFinder::ObjectFinder() {
    // create the main window
    namedWindow(windowName, WINDOW_AUTOSIZE);

    // initialize parameters that are subjects to change
    cannyThreshold = cannyThresholdInitialValue;
    accumulatorThreshold = accumulatorThresholdInitialValue;
    minCircleSize = minCircleSizeInitialValue;
    maxCircleSize = maxCircleSizeInitialValue;
    distanceBetweenCircleCenters = distanceBetweensCentersInitialValue;

    // Attach the trackbars
    createTrackbar(cannyThresholdTrackbarName, windowName, &cannyThreshold, maxCannyThreshold);
    createTrackbar(accumulatorThresholdTrackbarName, windowName, &accumulatorThreshold,
                   maxAccumulatorThreshold);
    createTrackbar(minCircleSizeTrackbarName, windowName, &minCircleSize, maxMinCircleSize);
    createTrackbar(maxCircleSizeTrackbarName, windowName, &maxCircleSize, maxMaxCircleSize);
    createTrackbar(distanceBetweenCircleCentersTrackbarName, windowName,
                   &distanceBetweenCircleCenters, maxDistanceBetweenCircleCenters);
}

 vector<Vec3f> ObjectFinder::getCircles(Mat* frame) {
    // those paramaters cannot be =0
    // so we must check here
    cannyThreshold = std::max(cannyThreshold, 1);
    accumulatorThreshold = std::max(accumulatorThreshold, 1);
    distanceBetweenCircleCenters = std::max(distanceBetweenCircleCenters, 1);

    // Convert it to gray
    cvtColor(*frame, frameGray, CV_BGR2GRAY);

    // Reduce the noise so we avoid false circle detection
    GaussianBlur(frameGray, frameGray, Size(3, 3), 2, 2);

    vector<Vec3f> circles;
    HoughCircles(frameGray, circles, CV_HOUGH_GRADIENT, 1, distanceBetweenCircleCenters , cannyThreshold,
                 accumulatorThreshold, minCircleSize, maxCircleSize);
    return circles;
}

