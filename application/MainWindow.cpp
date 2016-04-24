#include "MainWindow.h"
#include <iostream>

const string MainWindow::windowName = "Smart Pool";

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    MainWindow* window = static_cast<MainWindow *>(userdata);
    if ( event == EVENT_MOUSEMOVE ) {
        //cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
        window->setMouseCoordinates(x,y);
    }

    else if  ( event == EVENT_LBUTTONDOWN ) {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")";
        window->boardUpperLeft.x = x;
        window->boardUpperLeft.y = y;
    }
    else if  ( event == EVENT_RBUTTONDOWN ) {
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")";
        window->boardBottomRight.x = x;
        window->boardBottomRight.y = y;
    }
    else if  ( event == EVENT_MBUTTONDOWN ) {
         cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }

}

MainWindow::MainWindow() {
    namedWindow(windowName, WINDOW_AUTOSIZE);

    //set the callback function for any mouse event
    setMouseCallback(windowName, CallBackFunc, this);
    mouseX = 0;
    mouseY = 0;
    boardUpperLeft.x = 170;
    boardUpperLeft.y = 88;
    boardBottomRight.x = 1127;
    boardBottomRight.y = 608;

}

void MainWindow::showWindow (Mat frame, vector<Vec3f> circles){
    // Draw Circles on Board frame
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(frame, Point(c[0], c[1]), c[2], Scalar(0,0,255), 1, CV_AA);
        circle(frame, Point(c[0], c[1]), 2, Scalar(0,255,0), 1, CV_AA);
    }

    //draw Cue
    //line(img, pt1, pt2, color, thickness=1, lineType=8, shift=0)
    //line(frame,Point(640, 360), Point(mouseX, mouseY), Scalar(255,255,0), 4, CV_AA, 0 );

    rectangle(frame, boardUpperLeft, boardBottomRight, Scalar(255,0,255), 2, 8, 0);

    imshow(windowName, frame);
}

void MainWindow::setMouseCoordinates(int x, int y){
    mouseX = x;
    mouseY = y;
}
