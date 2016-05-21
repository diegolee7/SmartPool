#include "ProjectionWindow.hpp"
#include <iostream>
#include <cmath>

#define _USE_MATH_DEFINES

const string ProjectionWindow::windowProjectionName = "Projection";
const string ProjectionWindow::windowControlName = "Projection Control";

ProjectionWindow::ProjectionWindow() {
    namedWindow(windowProjectionName, WINDOW_AUTOSIZE);
    moveWindow(windowProjectionName, 1980,0);
    setWindowProperty(windowProjectionName, WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    frame = Mat(frameHeight, frameWidth, CV_8UC3, Scalar(0,0,0));

    boardUpperLeft.x = 249;
	boardUpperLeft.y = 113;
	boardBottomRight.x = 1210;
	boardBottomRight.y = 637;
	xProportion = 1;
	yProportion = 1;
	mouseY = 0;
	mouseX = 0;
	namedWindow(windowControlName, WINDOW_AUTOSIZE);
    createTrackbar("x1", windowControlName, &boardUpperLeft.x, 1000);
    createTrackbar("y1", windowControlName, &boardUpperLeft.y, 1000);
    createTrackbar("x2", windowControlName, &boardBottomRight.x, 2000);
    createTrackbar("y2", windowControlName, &boardBottomRight.y, 2000);
}



void ProjectionWindow::drawAllBalls(){


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
        circle(frame, Point(newX, newY),c[2]*2 , Scalar(0,255,0), 3, CV_AA);
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
        circle(frame, Point(newX, newY),16*6 , Scalar(0,255,0), -1, CV_AA);
    }
}

void ProjectionWindow::drawBoard(){

    projectionRectangle = Rect (boardUpperLeft,boardBottomRight);
    rectangle(frame, projectionRectangle, Scalar(255,255,255), 2, 8, 0);

}

void ProjectionWindow::drawTrajectory(){
    int whiteBallX = 0;
    int whiteBallY = 0;
    int whiteBallRadius = 16;

    for (size_t i = 0; i < whiteBalls.size(); i++) {
        Vec3i c = whiteBalls[i];
        whiteBallX = c[0]- tableRectangle.x;
        whiteBallX = whiteBallX * xProportion;
        whiteBallX += projectionRectangle.x;
        //std::cout << "\nx: " << c[0] << "\tnew x: " << newX << endl;
        whiteBallY = c[1]- tableRectangle.y;
        whiteBallY = whiteBallY * yProportion;
        whiteBallY += projectionRectangle.y;
		//std::cout << "\ny: " << c[1] << "\tnew y: " << newX << endl;
    }

    mouseX = mouseX- tableRectangle.x;
    mouseX = mouseX * xProportion;
    mouseX += projectionRectangle.x;
    //std::cout << "\nx: " << c[0] << "\tnew x: " << newX << endl;
    mouseY = mouseY - tableRectangle.y;
    mouseY = mouseY * yProportion;
    mouseY += projectionRectangle.y;

	//angle between two points
    float angle = atan2(whiteBallY - mouseY, whiteBallX- mouseX);
    //angle = angle;
    int xBorder = whiteBallRadius * cos(angle);
    int yBorder = whiteBallRadius * sin(angle);

    int deltaX = whiteBallX - mouseX;
    int deltaY = whiteBallY - mouseY;

    int x1 = whiteBallX + deltaX;
    int y1 = whiteBallY + deltaY;

    int x2 = xBorder+whiteBallX;
    x2 = x2 * xProportion;
    x2 += projectionRectangle.x;
    int y2 = yBorder+whiteBallY;
    y2 = y2 * yProportion;
    y2 += projectionRectangle.y;

    //draw Cue
    //line(img, pt1, pt2, color, thickness=1, lineType=8, shift=0);

    line(frame,Point(whiteBallX,whiteBallY), Point(x1, y1), Scalar(255,255,0), 4, CV_AA, 0 );

    Rect tableRectangle = Rect (boardUpperLeft,boardBottomRight);
    rectangle(frame, tableRectangle, Scalar(255,0,255), 2, 8, 0);
}

void ProjectionWindow::showWindow(){

	yProportion = (double)projectionRectangle.height/
						(double)tableRectangle.height;
	xProportion = (double)projectionRectangle.width/
						(double)tableRectangle.width;
    clearFrame();
    drawBoard();
    drawAllBalls();
    drawTrajectory();
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

void ProjectionWindow::setMousePosition(int mouseX, int mouseY){
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}
