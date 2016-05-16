#ifndef PROJECTIONWINDOW_H
#define PROJECTIONWINDOW_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class ProjectionWindow {

private:
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

};

#endif // PROJECTIONWINDOW_H
