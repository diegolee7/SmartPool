    #include "smartPool.h"
#include <iostream>
#include <stdio.h>

const string SmartPool::windowBoardName = "Smart Pool";
const string SmartPool::windowProjectionName = "Projection";
const string SmartPool::cannyThresholdTrackbarName = "Canny threshold";
const string SmartPool::accumulatorThresholdTrackbarName = "Accumulator Threshold";
const string SmartPool::minCircleSizeTrackbarName = "Minimum Radius Size";
const string SmartPool::maxCircleSizeTrackbarName = "Maximum Radius Size";

void SmartPool::init() {

    VideoCapture capture;
    Mat frame;

    createBoardWindow();

    // Read the video stream
    capture.open(cameraDevice);

    if (capture.isOpened()) {
        capture.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);

        for (;;) {

            // those paramaters cannot be =0
            // so we must check here
            cannyThreshold = std::max(cannyThreshold, 1);
            accumulatorThreshold = std::max(accumulatorThreshold, 1);

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
    }
    return;
}

/**
 * @function detectAndDisplay
 */
void SmartPool::detectAndDisplay(Mat frame) {
    Mat frameGray;
    Mat projection;

    // Convert it to gray
    cvtColor(frame, frameGray, CV_BGR2GRAY);

    //Projection starts black   
    cvtColor(frame, projection, CV_BGR2GRAY);
    projection.setTo(Scalar(0));

    // Reduce the noise so we avoid false circle detection
    GaussianBlur(frameGray, frameGray, Size(9, 9), 2, 2);

    vector<Vec3f> circles;
    HoughCircles(frameGray, circles, CV_HOUGH_GRADIENT, 1, frameGray.rows / 8, cannyThreshold,
                 accumulatorThreshold, minCircleSize, maxCircleSize);

    // Draw Circles on frame
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(projection, Point(c[0], c[1]), c[2], Scalar(255), 3, CV_AA);
        circle(projection, Point(c[0], c[1]), 2, Scalar(255), 3, CV_AA);
    }

    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(frame, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
        circle(frame, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);
    }

    //-- Show results
    imshow(windowBoardName, frame);
    imshow(windowProjectionName, projection);
}

void SmartPool::createBoardWindow() {

    // initialize both parameters that are subjects to change
    cannyThreshold = cannyThresholdInitialValue;
    accumulatorThreshold = accumulatorThresholdInitialValue;

    // initizalize circleSizes
    minCircleSize = minCircleSizeInitialValue;
    maxCircleSize = maxCircleSizeInitialValue;

    // create the main window, and attach the trackbars
    namedWindow(windowBoardName, WINDOW_AUTOSIZE);
    createTrackbar(cannyThresholdTrackbarName, windowBoardName, &cannyThreshold, maxCannyThreshold);
    createTrackbar(accumulatorThresholdTrackbarName, windowBoardName, &accumulatorThreshold,
                   maxAccumulatorThreshold);
    createTrackbar(minCircleSizeTrackbarName, windowBoardName, &minCircleSize, maxMinCircleSize);
    createTrackbar(maxCircleSizeTrackbarName, windowBoardName, &maxCircleSize, maxMaxCircleSize);


}

void SmartPool::createProjectionWindow() {
    namedWindow(windowProjectionName, WINDOW_AUTOSIZE);
}

int main() {
    SmartPool smartPool;
    smartPool.init();
}