/**
 * @file objectDetection.cpp
 * @author A. Huaman ( based in the classic facedetect.cpp in samples/c )
 * @brief A simplified version of facedetect.cpp, show how to load a cascade classifier and how to find objects (Face + eyes) in a video stream
 */
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
//-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations

string window_name = "Smart Pool";

/**
 * @function main
 */
int main( void )
{
  VideoCapture capture;
  Mat frame;

  //Read the video stream
  capture.open( 1 );
  if( capture.isOpened() )
  {
    capture.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,720);
    for(;;)
    {
      capture >> frame;

      //Apply the classifier to the frame
      if( !frame.empty() )
       { detectAndDisplay( frame ); }
      else
       { printf(" --(!) No captured frame -- Break!"); break; }

      int c = waitKey(10);
      if( (char)c == 'c' ) { break; }

    }
  }
  return 0;
}

/**
 * @function detectAndDisplay
 */
void detectAndDisplay( Mat frame )
{
  
  Mat cimg;
  Mat greyMat;
  cvtColor(frame, greyMat, CV_BGR2GRAY);
  medianBlur(greyMat, greyMat, 5);
  cvtColor(greyMat, cimg, COLOR_GRAY2BGR);

  vector<Vec3f> circles;
  HoughCircles(greyMat, circles, CV_HOUGH_GRADIENT, 1, 30,
               100, 10, 7, 10 // change the last two parameters
                              // (min_radius & max_radius) to detect larger circles
               );

  for( size_t i = 0; i < circles.size(); i++ )
  {
      Vec3i c = circles[i];
      circle( frame, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
      circle( frame, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);
  }
  
   //-- Show what you got
   imshow( window_name, frame );
}
