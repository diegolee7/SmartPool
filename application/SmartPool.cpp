#include "SmartPool.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>

//Window Names
const string SmartPool::windowBoardName = "Smart Pool";

//TrackBar Names
const string SmartPool::cannyThresholdTrackbarName = "Canny threshold";
const string SmartPool::accumulatorThresholdTrackbarName = "Accumulator Threshold";
const string SmartPool::minCircleSizeTrackbarName = "Minimum Radius Size";
const string SmartPool::maxCircleSizeTrackbarName = "Maximum Radius Size";
const string SmartPool::distanceBetweenCircleCentersTrackbarName = "Distance Between centers";

//Video Path and Name
const string SmartPool::videoPathAfterHomeDir = "/Dropbox/Oficinas de Integração 3/images_videos/";
const string SmartPool::videoName = "smartPoolTest-3.ogv";


void SmartPool::init() {
    createBoardWindow();


    // get your home directory
    string homedir = getenv("HOME");

    // Read the video stream
    VideoCapture capture;
    capture.open(homedir.append(videoPathAfterHomeDir).append(videoName));
    //capture.open(cameraDevice);

    if (capture.isOpened()) {
        capture.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);

        Mat frame;
        for (;;) {

            // those paramaters cannot be =0
            // so we must check here
            cannyThreshold = std::max(cannyThreshold, 1);
            accumulatorThreshold = std::max(accumulatorThreshold, 1);
            distanceBetweenCircleCenters = std::max(distanceBetweenCircleCenters, 1);

            capture >> frame;

            // Apply the classifier to the frame
            if (!frame.empty()) {
                detectAndDisplay(frame);
            } else {
                printf(" --(!) No captured frame -- Break!");
                break;
            }

            int c = waitKey(10);
            if ((char)c == 'c') {
                break;
            }
        }
    } else {
        printf("Problem Opening Capture \n");
    }
    return;
}

/**
 * @function detectAndDisplay
 */
void SmartPool::detectAndDisplay(Mat frame) {

    Mat frameGray;

    // Convert it to gray
    cvtColor(frame, frameGray, CV_BGR2GRAY);

    // Reduce the noise so we avoid false circle detection
    GaussianBlur(frameGray, frameGray, Size(9, 9), 2, 2);

    vector<Vec3f> circles;
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

    //-- Show results
    projectionWindow.clearFrame();
    projectionWindow.drawCircles(circles);
    projectionWindow.showWindow();
    imshow("Frame Gray", frameGray);
    imshow(windowBoardName, frame);
}

void SmartPool::createBoardWindow() {

    // create the main window
    namedWindow(windowBoardName, WINDOW_AUTOSIZE);

    // initialize parameters that are subjects to change
    cannyThreshold = cannyThresholdInitialValue;
    accumulatorThreshold = accumulatorThresholdInitialValue;
    minCircleSize = minCircleSizeInitialValue;
    maxCircleSize = maxCircleSizeInitialValue;
    distanceBetweenCircleCenters = distanceBetweensCentersInitialValue;

    // Attach the trackbars
    createTrackbar(cannyThresholdTrackbarName, windowBoardName, &cannyThreshold, maxCannyThreshold);
    createTrackbar(accumulatorThresholdTrackbarName, windowBoardName, &accumulatorThreshold,
                   maxAccumulatorThreshold);
    createTrackbar(minCircleSizeTrackbarName, windowBoardName, &minCircleSize, maxMinCircleSize);
    createTrackbar(maxCircleSizeTrackbarName, windowBoardName, &maxCircleSize, maxMaxCircleSize);
    createTrackbar(distanceBetweenCircleCentersTrackbarName, windowBoardName, 
                   &distanceBetweenCircleCenters, maxDistanceBetweenCircleCenters);
}

int main() {
    SmartPool smartPool;
    smartPool.init();
}
