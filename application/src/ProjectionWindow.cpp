#include "ProjectionWindow.hpp"

const string ProjectionWindow::windowProjectionName = "Projection";
const string ProjectionWindow::windowControlName = "Projection Control";

ProjectionWindow::ProjectionWindow() {
    namedWindow(windowProjectionName, WINDOW_AUTOSIZE);
    moveWindow(windowProjectionName, 1980,0);
    setWindowProperty(windowProjectionName, WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    frame = Mat(frameHeight, frameWidth, CV_8UC3, Scalar(0,0,0));

    boardUpperLeft.x = 269;
	boardUpperLeft.y = 122;
	boardBottomRight.x = 1158;
	boardBottomRight.y = 613;
	namedWindow(windowControlName, WINDOW_AUTOSIZE);
    createTrackbar("x1", windowControlName, &boardUpperLeft.x, 1000);
    createTrackbar("y1", windowControlName, &boardUpperLeft.y, 1000);
    createTrackbar("x2", windowControlName, &boardBottomRight.x, 2000);
    createTrackbar("y2", windowControlName, &boardBottomRight.y, 2000);
}

void ProjectionWindow::drawCircles(vector<Vec3f> circles){

    // Draw Circles on Projection frame
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(frame, Point(c[0], c[1]), c[2], Scalar(0,255,0), 3, CV_AA);
        circle(frame, Point(c[0], c[1]), 2, Scalar(255,0,0), 3, CV_AA);
    }
}

void ProjectionWindow::drawBoard(){

    Rect tableRectangle = Rect (boardUpperLeft,boardBottomRight);
    rectangle(frame, tableRectangle, Scalar(255,255,255), 2, 8, 0);

}

void ProjectionWindow::showWindow(vector<Vec3f> circles){
    clearFrame();
    drawCircles(circles);
    drawBoard();
    imshow(windowProjectionName, frame);
}

void ProjectionWindow::clearFrame(){
    frame = Mat(frameHeight, frameWidth, CV_8UC3, Scalar(0,0,0));
}
