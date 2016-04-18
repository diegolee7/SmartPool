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

		// initial and max values of the parameters of interests.
		static const int cannyThresholdInitialValue = 200;
		static const int accumulatorThresholdInitialValue = 50;
		static const int maxAccumulatorThreshold = 200;
	  	static const int maxCannyThreshold = 255;

	public:
		void init();
		void detectAndDisplay( Mat frame );
		void createWindow();

};