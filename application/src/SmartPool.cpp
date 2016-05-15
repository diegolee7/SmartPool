#include "SmartPool.hpp"

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>

//Video Path and Name
const string SmartPool::videoPathAfterHomeDir = "/Dropbox/Oficinas de Integração 3/images_videos/";
const string SmartPool::videoName = "smartPoolTest-2.ogv";

SmartPool::SmartPool(){
    drawTableBoard = false;
}

void SmartPool::init() {


    // get your home directory
    string homedir = getenv("HOME");

    // Read the video stream
    VideoCapture capture;
    //capture.open(homedir.append(videoPathAfterHomeDir).append(videoName));
    capture.open(cameraDevice);
    //capture.open("/home/diego/workspace/SmartPool/Video_001.avi");

    if (capture.isOpened()) {
        capture.set(CV_CAP_PROP_FRAME_WIDTH, frameWidth);
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, frameHeight);

        capture >> frame;
        for (;;) {

            capture >> frame;

            // Apply the classifier to the frame
            if (!frame.empty()) {
            	processAndDisplay();
            } else {
                printf(" --(!) No captured frame -- Break!");
                break;
            }

            int c = waitKey(10);

            if ((char)c == 'c') {
                break;
            }
        }
        capture.release();
    } else {
        printf("Problem Opening Capture \n");
    }
    return;
}

void SmartPool::processAndDisplay() {


	frameProcessor.processFrame(frame);
    //show
	vector<Vec3f> allBalls = frameProcessor.getAllBalls();
	vector<Vec3f> whiteBalls = frameProcessor.getWhiteBalls();

    mainWindow.showWindow(frame,allBalls,whiteBalls);
    projectionWindow.showWindow(allBalls);
}

int main() {
    SmartPool smartPool;
    smartPool.init();
}
