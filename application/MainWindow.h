#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class MainWindow {

private:
    static const int frameWidth = 1280;
    static const int frameHeight = 720;
    static const string windowName;
    int mouseX;
    int mouseY;

public:
    MainWindow();
    void showWindow (Mat frame, vector<Vec3f> circles);
    void setMouseCoordinates(int x, int y);
};

#endif // MAINWINDOW_H
