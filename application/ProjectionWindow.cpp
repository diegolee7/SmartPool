#include "ProjectionWindow.h"

const string ProjectionWindow::windowProjectionName = "Projection";

ProjectionWindow::ProjectionWindow() {
    namedWindow(windowProjectionName, WINDOW_AUTOSIZE);
    frame = Mat(frameHeight, frameWidth, CV_BGR2GRAY);
    frame.setTo(Scalar(0));
}

void ProjectionWindow::drawCircles(vector<Vec3f> circles){

    // Draw Circles on Projection frame
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(frame, Point(c[0], c[1]), c[2], Scalar(255), 3, CV_AA);
        circle(frame, Point(c[0], c[1]), 2, Scalar(255), 3, CV_AA);
    }
}

void ProjectionWindow::showWindow(){
    imshow(windowProjectionName, frame);
}

void ProjectionWindow::clearFrame(){
    frame = Mat(frameHeight, frameWidth, CV_BGR2GRAY);
    frame.setTo(Scalar(0));
}
