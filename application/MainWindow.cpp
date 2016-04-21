#include "MainWindow.h"

//Window Names
const string MainWindow::windowName = "Smart Pool";

//TrackBar Names
const string MainWindow::cannyThresholdTrackbarName = "Canny threshold";
const string MainWindow::accumulatorThresholdTrackbarName = "Accumulator Threshold";
const string MainWindow::minCircleSizeTrackbarName = "Minimum Radius Size";
const string MainWindow::maxCircleSizeTrackbarName = "Maximum Radius Size";
const string MainWindow::distanceBetweenCircleCentersTrackbarName = "Distance Between centers";


MainWindow::MainWindow() {
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

void MainWindow::showWindow(Mat frame) {
    // those paramaters cannot be =0
    // so we must check here
    cannyThreshold = std::max(cannyThreshold, 1);
    accumulatorThreshold = std::max(accumulatorThreshold, 1);
    distanceBetweenCircleCenters = std::max(distanceBetweenCircleCenters, 1);

    // Convert it to gray
    cvtColor(frame, frameGray, CV_BGR2GRAY);

    // Reduce the noise so we avoid false circle detection
    GaussianBlur(frameGray, frameGray, Size(9, 9), 2, 2);


    HoughCircles(frameGray, circles, CV_HOUGH_GRADIENT, 1, distanceBetweenCircleCenters , cannyThreshold,
                 accumulatorThreshold, minCircleSize, maxCircleSize);

    // Draw Circles on Board frame
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(frame, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
        circle(frame, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);
    }

    // Draw Circles on Gray frame with GaussianBlur
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(frameGray, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
        circle(frameGray, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);
    }
    imshow(windowName, frame);
}

 vector<Vec3f> MainWindow::getCircles(){
     return circles;
 }
