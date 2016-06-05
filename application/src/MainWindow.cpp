#include "MainWindow.hpp"

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

const string MainWindow::windowName = "Smart Pool";

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    MainWindow* window = static_cast<MainWindow *>(userdata);
    if ( event == EVENT_MOUSEMOVE ) {
        //cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
        window->setMouseCoordinates(x,y);
    }

    else if  ( event == EVENT_LBUTTONDOWN ) {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    }
    else if  ( event == EVENT_RBUTTONDOWN ) {
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        window->boardUpperLeft.x = x;
        window->boardUpperLeft.y = y;
    }
    else if  ( event == EVENT_MBUTTONDOWN ) {
         cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
         window->boardBottomRight.x = x;
         window->boardBottomRight.y = y;
    }

}


MainWindow::MainWindow() {
    namedWindow(windowName, WINDOW_AUTOSIZE);
    moveWindow(windowName, 0,0);
    //set the callback function for any mouse event
    setMouseCallback(windowName, CallBackFunc, this);
    mouseX = 0;
    mouseY = 0;
    boardUpperLeft.x = 321;
    boardUpperLeft.y = 186;
    boardBottomRight.x = 889;
    boardBottomRight.y = 504;
    whiteBallX = 0;
    whiteBallY = 0;
    whiteBallRadius = 0;

}

void MainWindow::showWindow (Mat frame, vector<Vec3f> circles, vector<Vec3f> whiteBall){
    // Draw Circles on Board frame
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(frame, Point(c[0], c[1]), c[2], Scalar(0,0,255), 1, CV_AA);
        circle(frame, Point(c[0], c[1]), 2, Scalar(0,255,0), 1, CV_AA);
    }

    if(whiteBall.size() > 0){
        for (size_t i = 0; i < whiteBall.size(); i++) {
            Vec3i c = whiteBall[i];
            whiteBallX = c[0];
            whiteBallY = c[1];
            whiteBallRadius = c[2];
        }
    }
    circle(frame, Point(whiteBallX, whiteBallY), whiteBallRadius, Scalar(255,255,0), 2, CV_AA);
    circle(frame, Point(whiteBallX, whiteBallY), 2, Scalar(0,255,0), 1, CV_AA);

    //angle between two points
    float angle = atan2(whiteBallY - mouseY, whiteBallX- mouseX);
    //angle = angle;
    int xBorder = whiteBallRadius * cos(angle);
    int yBorder = whiteBallRadius * sin(angle);

    int deltaX = whiteBallX - mouseX;
    int deltaY = whiteBallY - mouseY;

    //draw Cue
    //line(img, pt1, pt2, color, thickness=1, lineType=8, shift=0);
    //line(frame,Point(whiteBallX + deltaX, whiteBallY + deltaY), Point(xBorder+whiteBallX, yBorder+whiteBallY), Scalar(255,0,200), 4, CV_AA, 0 );

    Rect tableRectangle = Rect (boardUpperLeft,boardBottomRight);
    rectangle(frame, tableRectangle, Scalar(255,0,255), 2, 8, 0);

    //tableRectangle.in

    imshow(windowName, frame);
}

Rect MainWindow::getTableRectangle(){
	return Rect (boardUpperLeft,boardBottomRight);
}

void MainWindow::setMouseCoordinates(int x, int y){
    mouseX = x;
    mouseY = y;
}

int MainWindow::getMouseX(){
	return mouseX;
}

int MainWindow::getMouseY(){
	return mouseY;
}
