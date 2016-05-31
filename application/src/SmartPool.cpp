#include "SmartPool.hpp"

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>

//Video Path and Name
const string SmartPool::videoPathAfterHomeDir = "/Dropbox/Oficinas de Integração 3/images_videos/";
//const string SmartPool::videoName = "smartPoolTest-2.ogv";
const string SmartPool::videoName = "SmartPool_ap.ogv";

SmartPool::SmartPool(){
    drawTableBoard = false;
}

void SmartPool::init() {

    // get your home directory
    string homedir = getenv("HOME");

    // Read the video stream
    VideoCapture capture;
    //capture.open(homedir.append(videoPathAfterHomeDir).append(videoName));
    //capture.open(cameraDevice);
    //capture.open("/Dropbox/Oficinas de Integração 3/images_videos/SmartPool_ap.ogv");
    capture.open(homedir.append(videoPathAfterHomeDir).append("SmartPool_ap.ogv"));

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
    Rect tableRectangle = mainWindow.getTableRectangle();

    projectionWindow.setTableRectangle(tableRectangle);
    projectionWindow.setAllBalls(allBalls);
    projectionWindow.setWhiteBalls(whiteBalls);
    projectionWindow.setMousePosition(mainWindow.getMouseX(),mainWindow.getMouseY());
    projectionWindow.showWindow();
}

int main() {
    SmartPool smartPool;
    smartPool.init();
}
