#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class SmartPool {

  private:
    // Window Parameters
    static const string windowBoardName;
    static const string windowProjectionName;
    static const int frameWidth = 1280;
    static const int frameHeight = 720;

    // camera device /dev/video1
    static const int cameraDevice = 1;

    //video path
    static const string videoPathAfterHomeDir;
    static const string videoName;

    // trackbars names
    static const string cannyThresholdTrackbarName;
    static const string accumulatorThresholdTrackbarName;
    static const string minCircleSizeTrackbarName;
    static const string maxCircleSizeTrackbarName;
    static const string distanceBetweenCircleCentersTrackbarName;

    // initial values of the parameters of interests.
    static const int cannyThresholdInitialValue = 36;
    static const int accumulatorThresholdInitialValue = 17;
    static const int minCircleSizeInitialValue = 27;
    static const int maxCircleSizeInitialValue = 27;
    static const int distanceBetweensCentersInitialValue = 45;

    // max values of the parameters of interests.
    static const int maxAccumulatorThreshold = 200;
    static const int maxCannyThreshold = 255;
    static const int maxMinCircleSize = 100;
    static const int maxMaxCircleSize = 100;
    static const int maxDistanceBetweenCircleCenters = 100;

    // declare and initialize parameters that are subjects to change
    int cannyThreshold;
    int accumulatorThreshold;
    int minCircleSize;
    int maxCircleSize;
    int distanceBetweenCircleCenters;

  public:
    void init();
    void detectAndDisplay(Mat frame);
    void createBoardWindow();
    void createProjectionWindow();
};