#include "ProjectionWindow.hpp"
#include <iostream>
#include <cmath>

#define _USE_MATH_DEFINES

const string ProjectionWindow::windowProjectionName = "Projection";
const string ProjectionWindow::windowControlName = "Projection Control";

ProjectionWindow::ProjectionWindow() {
    namedWindow(windowProjectionName, CV_WINDOW_NORMAL);
    moveWindow(windowProjectionName, 1980,0);
    //setWindowProperty(windowProjectionName, WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    setWindowProperty(windowProjectionName, WND_PROP_FULLSCREEN, CV_WINDOW_AUTOSIZE);
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
    cue = {0,0,0,0};

}

void ProjectionWindow::showWindow(){

	//cout << "Cue: "<< cue << endl;
	startProjectionScaleAndHoles();
    clearFrame();
    drawBoard();
    drawAllBalls();
    drawTrajectory();
    imshow(windowProjectionName, frame);
}


void ProjectionWindow::drawAllBalls(){


    // Draw Circles on Projection frame
	int newX;
	int newY;
    for (size_t i = 0; i < allBalls.size(); i++) {
        Vec3i c = allBalls[i];
        circle(frame, Point(c[0], c[1]),c[2]*2 , Scalar(0,255,0), 3, CV_AA);
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
        circle(frame, Point(newX, newY),lightOnWhiteBallSize, Scalar(0,255,0), -1, CV_AA);
        whiteBallLightArea[0] = newX;
        whiteBallLightArea[1] = newY;
        whiteBallLightArea[2] = lightOnWhiteBallSize;

    }
}

bool ProjectionWindow::isPointInsideCircle(Point p,Vec3i c){
	//cout << p << c << endl;
	//cout << pow(c[2],2) << endl;
	//cout << pow(p.x-c[0],2) + pow(p.y - c[1],2) << endl;
	return (pow(c[2],2) > pow(p.x-c[0],2) + pow(p.y - c[1],2));
}

void ProjectionWindow::drawBoard(){

    projectionRectangle = Rect (boardUpperLeft,boardBottomRight);
    rectangle(frame, projectionRectangle, Scalar(255,255,255), 2, 8, 0);

}

Point2f ProjectionWindow::getCuePointNearWhiteBall(Vec4i tempCue){
	//cout << "Cue: " << cue << endl;
	Vec3i ball = whiteBalls[0];
	int whiteBallX = ball[0];
	int whiteBallY = ball[1];

	Point2f a(whiteBallX,whiteBallY);
	Point2f b(tempCue[0],tempCue[1]);
	Point2f c(tempCue[2],tempCue[3]);
	double res1 = cv::norm(a-b);
	double res2 = cv::norm(a-c);
	if (res1 < res2){
		return b;
	} else {
		return c;
	}
}

void ProjectionWindow::drawTrajectory(){
    int whiteBallX = 0;
    int whiteBallY = 0;
    int whiteBallRadius = ballsRadius;

    Point2f cuePoint = getCuePointNearWhiteBall(cue);

    //comment two lines below to use mouse as cue
    //mouseX = cuePoint.x;
    //mouseY = cuePoint.y;

    mouseX = mouseX - tableRectangle.x;
    mouseX = mouseX * xProportion;
    mouseX += projectionRectangle.x;
    //std::cout << "\nx: " << c[0] << "\tnew x: " << newX << endl;
    mouseY = mouseY - tableRectangle.y;
    mouseY = mouseY * yProportion;
    mouseY += projectionRectangle.y;

    if (!isPointInsideCircle(Point(mouseX,mouseY),whiteBallLightArea)){
    	//cout << "Cue outside white ball light area" << endl;
    	return;
    }

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



	//angle between two points
    float angle = atan2(whiteBallY - mouseY, whiteBallX- mouseX);
    //angle = angle;
    int xBorder = whiteBallRadius * cos(angle);
    int yBorder = whiteBallRadius * sin(angle);

    int deltaX = whiteBallX - mouseX;
    int deltaY = whiteBallY - mouseY;

    int x1 = whiteBallX + deltaX;
    int y1 = whiteBallY + deltaY;

    x1 = whiteBallX + 1500*cos(angle);
    y1 = whiteBallY + 1500*sin(angle);

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
	Point2f trajStart;
	trajStart.x = whiteBallX;
	trajStart.y = whiteBallY;
	Point2f trajEnd;
	trajEnd.x = x1;
	trajEnd.y = y1;

	Vec3f c;
	c[0] = 500;
	c[1] = 200;
	c[2] = 50;
	//circle(frame, Point(c[0], c[1]),c[2] , Scalar(0,255,0), 3, CV_AA);
	//bool circleLine = circleLineIntersect(trajStart,trajectoryEndPoint,c);
	//cout << "Circle Line Intersection: " << circleLine <<endl;
	//pointLineDistance(trajStart,trajectoryEndPoint,Point2f(500,500));
	//circle(frame, Point(500,500),5 , Scalar(255,255,255), 3, CV_AA);

	//check if white ball collides with any other
	//cout << "Balls found: " << allBalls.size() << endl;

	Point2f pointOnLine;
	vector<Point2f> collisionsWhite;
	vector<Point2f> collisionsNumbered;

	for (size_t i = 0; i < allBalls.size(); i++) {
        Vec3i c = allBalls[i];
		//check if this is not the white ball itself
        //check if the distance from white ball center is bigger than radius
		if(norm(trajStart-Point2f(c[0],c[1])) >= ballsRadius){

			//if distance is < radius*2 then it collides
			if(pointLineDistance(trajStart,trajEnd,Point2f(c[0],c[1]),pointOnLine) <= ballsRadius*2){
				cout << "Collision: " << i << c[0] << "," <<c[1] << endl;
				cout << "Projection Point on line " << pointOnLine << endl;

				//check if point is on line segment, not on the back (lines are infinite)
				float maxX = max(trajStart.x,trajEnd.x);
				float maxY = max(trajStart.y,trajEnd.y);
				float minX = min(trajStart.x,trajEnd.x);
				float minY = min(trajStart.y,trajEnd.y);
				if (pointOnLine.x < maxX && pointOnLine.x > minX &&
					pointOnLine.y < maxY && pointOnLine.y > minY){
					//circle(frame, Point(c[0],c[1]),ballsRadius, Scalar(255,255,255), -1, CV_AA);
					collisionsNumbered.push_back(Point2f(c[0],c[1]));

					//so here I have to apply Pitagoras do find out where was the first collision
					float dist1 = norm(pointOnLine-Point2f(c[0],c[1]));
					float dist2 = ballsRadius*2;
					float dist3 = sqrt(dist2*dist2 - dist1*dist1);
					Point2f collision;
					collision.x = pointOnLine.x - (pointOnLine.x - trajStart.x) / norm(pointOnLine - trajStart) * dist3;
					collision.y = pointOnLine.y - (pointOnLine.y - trajStart.y) / norm(pointOnLine - trajStart) * dist3;
					collisionsWhite.push_back(Point2f(collision.x,collision.y));
				}
			}
		} else {
			//cout << "White ball is: " << i << trajStart <<endl;
		}
    }

	//check which collision is the first one
	int closestBall = 0;
	float dist = 99999;
	if(collisionsWhite.size() > 0){
		for(int i = 0; i< collisionsWhite.size() ; i++){
			if(norm (collisionsWhite[i] - collisionsWhite[closestBall]) < dist){
				closestBall = i;
				dist = norm (collisionsWhite[i] - collisionsWhite[closestBall]);
			}
		}
		circle(frame, collisionsWhite[closestBall], ballsRadius, Scalar(255,255,255), -1, CV_AA);
		circle(frame, collisionsNumbered[closestBall], ballsRadius, Scalar(255,255,255), -1, CV_AA);
	}

	// the code below check if the ball goes into any pocket or rails
	//prevent for loop forever
	int maxIterations = 5;
	int hole = 0;
	Point2f holePoint;
	for (int i = 0; foundAllCollisions == false && i < maxIterations; i++){
		hole = checkHoles(trajStart,trajEnd, holePoint);
		if(hole > -1){
			trajEnd = holePoint;
			//cout << "Ball is going into Hole: " << holePoint << endl;
			lines.push_back(Vec4i(trajStart.x,trajStart.y,
					trajEnd.x,trajEnd.y));
			foundAllCollisions = true;
		} else if( intersection(table.p1, table.p2,
				trajStart,trajEnd, intersectionPoint)){
			lines.push_back(Vec4i(trajStart.x,trajStart.y,
					intersectionPoint.x,intersectionPoint.y));
			trajStart.x = intersectionPoint.x;
			trajStart.y = intersectionPoint.y + 1;
			delta = trajEnd.y - table.p1.y;
			trajEnd.y = table.p1.y - delta;
		} else if (intersection(table.p2, table.p3,
				trajStart,trajEnd, intersectionPoint)){
			lines.push_back(Vec4i(trajStart.x,trajStart.y,
					intersectionPoint.x,intersectionPoint.y));
			trajStart.x = intersectionPoint.x - 1;
			trajStart.y = intersectionPoint.y;
			delta = trajEnd.x - table.p2.x;
			trajEnd.x = table.p2.x - delta;
		} else if (intersection(table.p3, table.p4,
				trajStart,trajEnd, intersectionPoint)){
			lines.push_back(Vec4i(trajStart.x,trajStart.y,
					intersectionPoint.x,intersectionPoint.y));
			trajStart.x = intersectionPoint.x;
			trajStart.y = intersectionPoint.y - 1;
			delta = trajEnd.y - table.p3.y;
			trajEnd.y = table.p3.y - delta;
		} else if (intersection(table.p4, table.p1,
				trajStart,trajEnd, intersectionPoint)){
			lines.push_back(Vec4i(trajStart.x,trajStart.y,
					intersectionPoint.x,intersectionPoint.y));
			trajStart.x = intersectionPoint.x + 1;
			trajStart.y = intersectionPoint.y;
			delta = trajEnd.x - table.p4.x;
			trajEnd.x = table.p4.x - delta;
		} else {
			lines.push_back(Vec4i(trajStart.x,trajStart.y,
					trajEnd.x,trajEnd.y));
			foundAllCollisions = true;
		}
		//cout << "Intersection: "<< foundAllCollisions << intersectionPoint << " "<< table.p1
		//		<< table.p2 << trajectoryStartPoint << trajectoryEndPoint << endl;
	}

	//draw trajectory without colisions
	//line(frame,trajectoryStartPoint, trajectoryEndPoint, Scalar(255,255,0), 4, CV_AA, 0 );

	//Point2f point1
    for(unsigned int i = 0; i < lines.size(); i++){
    	//cout << "Lines size: " << lines.size() << endl;
    	Vec4i tempLine = lines[i];
    	line(frame,Point(tempLine[0],tempLine[1]), Point(tempLine[2], tempLine[3]), Scalar(255,255,0), 4, CV_AA, 0 );
    	//cout << "Drawing trajectory " << i << "(" << tempLine[0] << "," << tempLine[1] << ") "
    	//		<< "(" << tempLine[2] << "," << tempLine[3] << ")" << endl ;
    }

    //holes
    for (int i = 0; i< 6; i++) {
    	line(frame, holes.h[i][0] , holes.h[i][1], Scalar(255,255,0), 4, CV_AA, 0 );
    }

    if (hole > -1){
    	circle(frame, Point((holes.h[hole][0].x + holes.h[hole][1].x)/2 ,
    			(holes.h[hole][0].y + holes.h[hole][1].y)/2 ),ballsRadius*4, Scalar(25,180,0), -1, CV_AA);
    }


    Rect tableRectangle = Rect (boardUpperLeft,boardBottomRight);
    rectangle(frame, tableRectangle, Scalar(255,0,255), 2, 8, 0);
}

float ProjectionWindow::pointLineDistance(Point2f line1, Point2f line2, Point2f point, Point2f &pointOnLine){
	float dx = line2.x - line1.x;
	float dy = line2.y - line1.y;
	float distance = 0;

	//not vertical line
	if(dy != 0){
	    // y = mx + c
	    // intercept c = y - mx
		float slope = dy/dx;
		float intercept = line1.y - slope * line1.x; // which is same as y2 - slope * x2
		// mx - y + c = 0
		// fabsf(m*point.x + (-1)*point.y + c)/sqrt(m*m+(-1)*(-1))
		distance =  fabsf(slope*point.x + (-1)*point.y + intercept)/
						sqrt(slope*slope+(-1)*(-1));

		// ax + by + c = 0;
		// x = (b*(b*x0 - a*y0) - a*c)/(a*a+b*b)
		// y = (a*(-b*x0 + a*y0)-b*c)/(a*a+b*b)
		float b = -1;
		float x = (b*(b*point.x - slope*point.y) - slope*intercept) / (slope*slope + b*b);
		float y = (slope*(-b*point.x + slope*point.y) - b*intercept) / (slope*slope + b*b);
		pointOnLine.x = x;
		pointOnLine.y = y;

	} else {
		//vertical line
		distance = line2.x - point.x;
	}

	//cout << "Distance: " << distance << endl;
	return distance;

}

bool ProjectionWindow::circleLineIntersect(Point2f p1, Point2f p2, Vec3f circle) {
	float dx = p2.x - p1.x	;
	float dy = p2.y - p1.y;
	float dr = sqrt(dx*dx + dy*dy);
	float D = p1.x*p2.y - p1.y*p2.x;
	float radius = circle[2];
	float discriminant = radius*radius*	dr*dr - D*D;
	if (discriminant > 0){
		return true;
	}
	return false;
}

int ProjectionWindow::checkHoles(Point2f trajectoryStartPoint, Point2f trajectoryEndPoint, Point2f &r){
	int holePocketed = -1;
	for(int i = 0; i < 6; i++){
		if(intersection(trajectoryStartPoint, trajectoryEndPoint , holes.h[i][0] , holes.h[i][1], r)){
			holePocketed = i;
			break;
		}
	}
	return holePocketed;
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


void ProjectionWindow::startProjectionScaleAndHoles(){

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

	//hole 1
	holes.h[0][0].x = projectionRectangle.x + (float)projectionRectangle.width * 0.05;
	holes.h[0][0].y = projectionRectangle.y;
	holes.h[0][1].x = projectionRectangle.x;
	holes.h[0][1].y = projectionRectangle.y + (float)projectionRectangle.height * 0.09;

	//hole 2
	holes.h[1][0].x = projectionRectangle.x + projectionRectangle.width/2 + (float)projectionRectangle.width * 0.04;
	holes.h[1][0].y = projectionRectangle.y;
	holes.h[1][1].x = projectionRectangle.x + projectionRectangle.width/2 - (float)projectionRectangle.width * 0.04;
	holes.h[1][1].y = projectionRectangle.y;

	//hole 3
	holes.h[2][0].x = projectionRectangle.x + projectionRectangle.width - (float)projectionRectangle.width * 0.05;
	holes.h[2][0].y = projectionRectangle.y;
	holes.h[2][1].x = projectionRectangle.x + projectionRectangle.width;
	holes.h[2][1].y = projectionRectangle.y + (float)projectionRectangle.height * 0.09;

	//hole 4
	holes.h[3][0].x = projectionRectangle.x + projectionRectangle.width - (float)projectionRectangle.width * 0.05;
	holes.h[3][0].y = projectionRectangle.y + projectionRectangle.height;
	holes.h[3][1].x = projectionRectangle.x + projectionRectangle.width;
	holes.h[3][1].y = projectionRectangle.y + projectionRectangle.height - (float)projectionRectangle.height * 0.09;

	//hole 5
	holes.h[4][0].x = projectionRectangle.x + projectionRectangle.width/2 + (float)projectionRectangle.width * 0.04;
	holes.h[4][0].y = projectionRectangle.y + projectionRectangle.height;
	holes.h[4][1].x = projectionRectangle.x + projectionRectangle.width/2 - (float)projectionRectangle.width * 0.04;
	holes.h[4][1].y = projectionRectangle.y + projectionRectangle.height;

	//hole 6
	holes.h[5][0].x = projectionRectangle.x + (float)projectionRectangle.width * 0.05;
	holes.h[5][0].y = projectionRectangle.y + projectionRectangle.height;
	holes.h[5][1].x = projectionRectangle.x;
	holes.h[5][1].y = projectionRectangle.y + projectionRectangle.height - (float)projectionRectangle.height * 0.09;

	//cout << "h11: " << holes.h11

}


void ProjectionWindow::clearFrame(){
    frame = Mat(frameHeight, frameWidth, CV_8UC3, Scalar(0,0,0));
}

void ProjectionWindow::setTableRectangle (Rect tableRectangle){
	this->tableRectangle = tableRectangle;
}

void ProjectionWindow::setAllBalls ( vector<Vec3f> tempAllBalls){
    // Draw Circles on Projection frame
	int newX;
	int newY;
	allBalls.clear();
    for (size_t i = 0; i < tempAllBalls.size(); i++) {
        Vec3i c = tempAllBalls[i];
        newX = c[0]- tableRectangle.x;
        newX = newX * xProportion;
        newX += projectionRectangle.x;
        //std::cout << "\nx: " << c[0] << "\tnew x: " << newX << endl;
        newY = c[1]- tableRectangle.y;
		newY = newY * yProportion;
		newY += projectionRectangle.y;
		//std::cout << "\ny: " << c[1] << "\tnew y: " << newX << endl;
		allBalls.push_back(Vec3f(newX,newY,c[2]));
    }
}

void ProjectionWindow::setWhiteBalls ( vector<Vec3f> whiteBalls){
	if(whiteBalls.size() > 0){
		this->whiteBalls = whiteBalls;
	} else {
		cue[0] = 0;
		cue[1] = 0;
		cue[2] = 0;
		cue[3] = 0;
	}
}

void ProjectionWindow::setMousePosition(int mouseX, int mouseY){
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}

void ProjectionWindow::setCue(Vec4i cue){

	Point2f cuePoint = getCuePointNearWhiteBall(cue);
    int cuePointX = cuePoint.x;
    int cuePointY = cuePoint.y;
    mouseX = cuePointX;
    mouseY = cuePointY;

    mouseX = mouseX - tableRectangle.x;
    mouseX = mouseX * xProportion;
    mouseX += projectionRectangle.x;
    //std::cout << "\nx: " << c[0] << "\tnew x: " << newX << endl;
    mouseY = mouseY - tableRectangle.y;
    mouseY = mouseY * yProportion;
    mouseY += projectionRectangle.y;

    if (isPointInsideCircle(Point(mouseX,mouseY),whiteBallLightArea)){
    	//cout << "Cue outside white ball light area" << endl;
    	this->cue = cue;
    }

    return;
}
