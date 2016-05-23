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

    mouseX = mouseX - tableRectangle.x;
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


    //line equation: y - y1 = m (x - x1);
    // m = (y - y1)/ (x - x1)
    vector<Vec4i> lines;

    // if it's 0 we can't divide per 0
/*
	if(x1 - whiteBallX != 0){
		double slope = double(y1 - whiteBallY)/ (x1 - whiteBallX);
		cout << "Slope: " << slope << endl;
		if(x1 < projectionRectangle.x) {
			int y2 = int(slope *(projectionRectangle.x - whiteBallX) + whiteBallY);
			lines.push_back(Vec4i(whiteBallX,whiteBallY,projectionRectangle.x,y2));
			int x2 = projectionRectangle.x + projectionRectangle.x - x1;
			lines.push_back(Vec4i(projectionRectangle.x,y2,x2,y1));
		}
	}
*/

    //draw Cue
    //line(img, pt1, pt2, color, thickness=1, lineType=8, shift=0);

	bool foundAllCollisions = false;
	float delta = 0;
	Point2f intersectionPoint;
	Point2f trajectoryStartPoint;
	trajectoryStartPoint.x = whiteBallX;
	trajectoryStartPoint.y = whiteBallY;
	Point2f trajectoryEndPoint;
	trajectoryEndPoint.x = x1;
	trajectoryEndPoint.y = y1;

	//prevent for loop forever
	int maxIterations = 5;

	for (int i = 0; foundAllCollisions == false && i < maxIterations; i++){
		if( intersection(table.p1, table.p2,
				trajectoryStartPoint,trajectoryEndPoint, intersectionPoint)){
			lines.push_back(Vec4i(trajectoryStartPoint.x,trajectoryStartPoint.y,
					intersectionPoint.x,intersectionPoint.y));
			trajectoryStartPoint.x = intersectionPoint.x;
			trajectoryStartPoint.y = intersectionPoint.y + 1;
			delta = trajectoryEndPoint.y - table.p1.y;
			trajectoryEndPoint.y = table.p1.y - delta;
		} else if (intersection(table.p2, table.p3,
				trajectoryStartPoint,trajectoryEndPoint, intersectionPoint)){
			lines.push_back(Vec4i(trajectoryStartPoint.x,trajectoryStartPoint.y,
					intersectionPoint.x,intersectionPoint.y));
			trajectoryStartPoint.x = intersectionPoint.x - 1;
			trajectoryStartPoint.y = intersectionPoint.y;
			delta = trajectoryEndPoint.x - table.p2.x;
			trajectoryEndPoint.x = table.p2.x - delta;
		} else if (intersection(table.p3, table.p4,
				trajectoryStartPoint,trajectoryEndPoint, intersectionPoint)){
			lines.push_back(Vec4i(trajectoryStartPoint.x,trajectoryStartPoint.y,
					intersectionPoint.x,intersectionPoint.y));
			trajectoryStartPoint.x = intersectionPoint.x;
			trajectoryStartPoint.y = intersectionPoint.y - 1;
			delta = trajectoryEndPoint.y - table.p3.y;
			trajectoryEndPoint.y = table.p3.y - delta;
		} else if (intersection(table.p4, table.p1,
				trajectoryStartPoint,trajectoryEndPoint, intersectionPoint)){
			lines.push_back(Vec4i(trajectoryStartPoint.x,trajectoryStartPoint.y,
					intersectionPoint.x,intersectionPoint.y));
			trajectoryStartPoint.x = intersectionPoint.x + 1;
			trajectoryStartPoint.y = intersectionPoint.y;
			delta = trajectoryEndPoint.x - table.p4.x;
			trajectoryEndPoint.x = table.p4.x - delta;
		} else {
			lines.push_back(Vec4i(trajectoryStartPoint.x,trajectoryStartPoint.y,
					trajectoryEndPoint.x,trajectoryEndPoint.y));
			foundAllCollisions = true;
		}
		//cout << "Intersection: "<< foundAllCollisions << intersectionPoint << " "<< table.p1
		//		<< table.p2 << trajectoryStartPoint << trajectoryEndPoint << endl;
	}

	//draw trajectory without colisions
	line(frame,trajectoryStartPoint, trajectoryEndPoint, Scalar(255,255,0), 4, CV_AA, 0 );

	//Point2f point1
    for(unsigned int i = 0; i < lines.size(); i++){
    	Vec4i tempLine = lines[i];
    	line(frame,Point(tempLine[0],tempLine[1]), Point(tempLine[2], tempLine[3]), Scalar(255,255,0), 4, CV_AA, 0 );
    	//cout << "Drawing trajectory " << i << "(" << tempLine[0] << "," << tempLine[1] << ") "
    	//		<< "(" << tempLine[2] << "," << tempLine[3] << ")" << endl ;
    }

    Rect tableRectangle = Rect (boardUpperLeft,boardBottomRight);
    rectangle(frame, tableRectangle, Scalar(255,0,255), 2, 8, 0);
}

// Finds the intersection of two lines, or returns false.
// The lines are defined by (o1, p1) and (o2, p2).
bool ProjectionWindow::intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2,
                      Point2f &r){
    Point2f x = o2 - o1;
    Point2f d1 = p1 - o1;
    Point2f d2 = p2 - o2;

    float cross = d1.x*d2.y - d1.y*d2.x;
    if (abs(cross) < /*EPS*/1e-8)
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    r = o1 + d1 * t1;

    //check if point is on both line segments
    float maxX1 = max (o1.x,p1.x);
    float maxY1 = max (o1.y,p1.y);
    float minX1 = min (o1.x,p1.x);
    float minY1 = min (o1.y,p1.y);

    float maxX2 = max (o2.x,p2.x);
    float maxY2 = max (o2.y,p2.y);
    float minX2 = min (o2.x,p2.x);
    float minY2 = min (o2.y,p2.y);

    //We can do that because all table line segments ae on x or y axis
    if (r.x >= minX1 && r.x <= maxX1 && r.y >= minY1 && r.y <= maxY1 &&
    	r.x >= minX2 && r.x <= maxX2 && r.y >= minY2 && r.y <= maxY2){
        return true;
    }
    return false;
}

void ProjectionWindow::showWindow(){

	yProportion = (double)projectionRectangle.height/
						(double)tableRectangle.height;
	xProportion = (double)projectionRectangle.width/
						(double)tableRectangle.width;

	//clockwise points
	table.p1.x = projectionRectangle.x;
	table.p1.y = projectionRectangle.y;
	table.p2.x = projectionRectangle.x + projectionRectangle.width;
	table.p2.y = projectionRectangle.y;
	table.p3.x = projectionRectangle.x + projectionRectangle.width;
	table.p3.y = projectionRectangle.y + projectionRectangle.height;
	table.p4.x = projectionRectangle.x;
	table.p4.y = projectionRectangle.y + projectionRectangle.height;




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
	if(whiteBalls.size() > 0){
		this->whiteBalls = whiteBalls;
	}
}

void ProjectionWindow::setMousePosition(int mouseX, int mouseY){
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}
