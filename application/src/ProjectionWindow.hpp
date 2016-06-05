#ifndef PROJECTIONWINDOW_H
#define PROJECTIONWINDOW_H

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

class ProjectionWindow {

private:
	struct Table {
		Point2f p1;
		Point2f p2;
		Point2f p3;
		Point2f p4;
	} table;

	struct Holes {
		Point2f h[6][2];
	} holes;

	static const int ballsRadius = 16;
	static const int lightOnWhiteBallSize = ballsRadius*10;
    static const int frameWidth = 1280;
    static const int frameHeight = 720;
    static const string windowProjectionName;
    static const string windowControlName;
    Mat frame;
    Rect tableRectangle;
    Rect projectionRectangle;
    vector<Vec3f> transformCoordinates(vector<Vec3f> circlesPointer);
    vector<Vec3f> allBalls;
    vector<Vec3f> whiteBalls;
    int mouseX;
	int mouseY;
	double yProportion;
	double xProportion;
	Vec4i cue;
	void startProjectionScaleAndHoles();
	Vec3i whiteBallLightArea;

public:
    ProjectionWindow();
    void createProjectionWindow();
    void drawAllBalls();
    void showWindow();
    void clearFrame();
    void drawBoard();
    Point boardUpperLeft;
    Point boardBottomRight;
    void setTableRectangle(Rect tableRectangle);
    void setAllBalls ( vector<Vec3f> allBalls);
    void setWhiteBalls ( vector<Vec3f> whiteBalls);
    void setMousePosition(int mouseX, int mouseY);
    void drawTrajectory();
    bool intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2,
                          Point2f &r);
    int checkHoles(Point2f trajectoryStartPoint, Point2f trajectoryEndPoint, Point2f &r);
    bool circleLineIntersect(Point2f trajectoryStartPoint, Point2f trajectoryEndPoint, Vec3f circle);
    void setCue (Vec4i cue);
    Point2f getCuePointNearWhiteBall(Vec4i tempCue);
    bool isPointInsideCircle (Point p,Vec3i c);
    float pointLineDistance(Point2f line1, Point2f line2, Point2f point);
};

#endif // PROJECTIONWINDOW_H
