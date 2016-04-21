#include "MainWindow.h"

const string MainWindow::windowName = "Smart Pool";

MainWindow::MainWindow() {
    namedWindow(windowName, WINDOW_AUTOSIZE);
}

void MainWindow::showWindow (Mat* frame, vector<Vec3f> circles){
    // Draw Circles on Board frame
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(*frame, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
        circle(*frame, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);
    }

    // Draw Circles on Gray frame with GaussianBlur
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        //circle(frameGray, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
        //circle(frameGray, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);
    }

    imshow(windowName, *frame);
}
