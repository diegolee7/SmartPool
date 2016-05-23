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
};

#endif // PROJECTIONWINDOW_H
