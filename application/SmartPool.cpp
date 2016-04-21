#include "SmartPool.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>

//Video Path and Name
const string SmartPool::videoPathAfterHomeDir = "/Dropbox/Oficinas de Integração 3/images_videos/";
const string SmartPool::videoName = "smartPoolTest-3.ogv";

SmartPool::SmartPool(){

}

void SmartPool::init() {

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

void SmartPool::detectAndDisplay(Mat frame) {

    objectFinder.showWindow(frame);
    vector<Vec3f> circles = objectFinder.getCircles();

    //-- Show results
    projectionWindow.clearFrame();
    projectionWindow.drawCircles(circles);
    projectionWindow.showWindow();


    //imshow("Frame Gray", frameGray);

}


int main() {
    SmartPool smartPool;
    smartPool.init();
}
