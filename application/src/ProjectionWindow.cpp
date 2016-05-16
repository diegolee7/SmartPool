#include "ProjectionWindow.hpp"
#include <iostream>

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



void ProjectionWindow::drawAllBalls(){

	double yProportion = (double)projectionRectangle.height/
						(double)tableRectangle.height;
	double xProportion = (double)projectionRectangle.width/
						(double)tableRectangle.width;

    // Draw Circles on Projection frame
	int newX;
	int newY;
    for (size_t i = 0; i < allBalls.size(); i++) {
        Vec3i c = allBalls[i];
        newX = c[0]- tableRectangle.x;
        newX = newX * xProportion;
        newX += projectionRectangle.x;
        //std::cout << "\nx: " << c[0] << "\tnew x: " << newX << endl;
        newY = c[1]- tableRectangle.y;
		newY = newY * yProportion;
		newY += projectionRectangle.y;
		//std::cout << "\ny: " << c[1] << "\tnew y: " << newX << endl;
        circle(frame, Point(newX, newY),c[2]*3 , Scalar(0,255,0), 3, CV_AA);
    }

    for (size_t i = 0; i < whiteBalls.size(); i++) {
        Vec3i c = whiteBalls[i];
        newX = c[0]- tableRectangle.x;
        newX = newX * xProportion;
        newX += projectionRectangle.x;
        //std::cout << "\nx: " << c[0] << "\tnew x: " << newX << endl;
        newY = c[1]- tableRectangle.y;
		newY = newY * yProportion;
		newY += projectionRectangle.y;
		//std::cout << "\ny: " << c[1] << "\tnew y: " << newX << endl;
        circle(frame, Point(newX, newY),c[2]*6 , Scalar(0,255,0), -1, CV_AA);
    }
}

void ProjectionWindow::drawBoard(){

    projectionRectangle = Rect (boardUpperLeft,boardBottomRight);
    rectangle(frame, projectionRectangle, Scalar(255,255,255), 2, 8, 0);

}

void ProjectionWindow::showWindow(){
    clearFrame();
    drawBoard();
    drawAllBalls();
    imshow(windowProjectionName, frame);
}

void ProjectionWindow::clearFrame(){
    frame = Mat(frameHeight, frameWidth, CV_8UC3, Scalar(0,0,0));
}

void ProjectionWindow::setTableRectangle (Rect tableRectangle){
	this->tableRectangle = tableRectangle;
}

void ProjectionWindow::setAllBalls ( vector<Vec3f> allBalls){
	this->allBalls = allBalls;
}

void ProjectionWindow::setWhiteBalls ( vector<Vec3f> whiteBalls){
	this->whiteBalls = whiteBalls;
}
