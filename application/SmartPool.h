#ifndef SMARTPOOL_H
#define SMARTPOOL_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ProjectionWindow.h"
#include "ObjectFinder.h"
#include "MainWindow.h"

using namespace std;
using namespace cv;

class SmartPool {

private:
    // Window Parameters
    static const string windowBoardName;
    static const int frameWidth = 1280;
    static const int frameHeight = 720;

    // camera device /dev/video1
    static const int cameraDevice = 1;

    //video path
    static const string videoPathAfterHomeDir;
    static const string videoName;

    ProjectionWindow projectionWindow;
    ObjectFinder objectFinder;
    MainWindow mainWindow;

    Mat frame;
    bool drawTableBoard;

public:
    SmartPool();
    void init();
    void detectAndDisplay();

};

#endif // SMARTPOOL_H
