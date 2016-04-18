#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class SmartPool {

	private:	
		// Window Parameters
		static const string windowName;
		static const int frameWidth = 1280;
		static const int frameHeight = 720;

		//camera device /dev/video1
		static const int cameraDevice = 1;

		//trackbars names
		static const string cannyThresholdTrackbarName;
		static const string accumulatorThresholdTrackbarName;
		static const string minCircleSizeTrackbarName;
		static const string maxCircleSizeTrackbarName;

		// initial and max values of the parameters of interests.
		static const int cannyThresholdInitialValue = 60;
		static const int accumulatorThresholdInitialValue = 20;
		static const int maxAccumulatorThreshold = 200;
	  	static const int maxCannyThreshold = 255;

		static const int minCircleSizeInitialValue = 26;
		static const int maxCircleSizeInitialValue = 30;	  	
		static const int maxMinCircleSize = 100;
	  	static const int maxMaxCircleSize = 100;

	  	//declare and initialize both parameters that are subjects to change
	  	int cannyThreshold;
    	int accumulatorThreshold;
    	int minCircleSize;
    	int maxCircleSize;

	public:
		void init();
		void detectAndDisplay( Mat frame );
		void createWindow();

};