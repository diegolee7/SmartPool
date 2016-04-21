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
    Mat frame;

public:
    ProjectionWindow();
    void createProjectionWindow();
    void drawCircles(vector<Vec3f> circles);
    void showWindow();
    void clearFrame();
};

#endif // PROJECTIONWINDOW_H
