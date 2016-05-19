/*
 * ControlWindow.h
 *
 *  Created on: May 15, 2016
 *      Author: diego
 */

#ifndef CONTROLWINDOW_HPP_
#define CONTROLWINDOW_HPP_

#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

class ControlWindow {
public:
	ControlWindow();
	virtual ~ControlWindow();
	int getAccumulatorThreshold();
	int getCannyThreshold();
	int getDistanceBetweenCircleCenters();
	int getDp();
	int getMaxCircleSize();
	int getMaxHue();
	int getMaxSaturation();
	int getMaxValue();
	int getMinCircleSize();
	int getMinHue();
	int getMinSaturation();
	int getMinValue();

private:
    // window name
    static const string controlWindowName;

    // trackbars names
    static const string cannyThresholdTrackbarName;
    static const string accumulatorThresholdTrackbarName;
    static const string minCircleSizeTrackbarName;
    static const string maxCircleSizeTrackbarName;
    static const string distanceBetweenCircleCentersTrackbarName;

    // initial values of the parameters of interests.
    static const int cannyThresholdInitialValue = 36;
    static const int accumulatorThresholdInitialValue = 14;
    static const int minCircleSizeInitialValue = 13;
    static const int maxCircleSizeInitialValue = 15;
    static const int distanceBetweensCentersInitialValue = 45;

    // max values of the parameters of interests.
    static const int maxAccumulatorThreshold = 400;
    static const int maxCannyThreshold = 255;
    static const int maxMinCircleSize = 100;
    static const int maxMaxCircleSize = 100;
    static const int maxDistanceBetweenCircleCenters = 100;

    // Hough Circles Variables
    int cannyThreshold;
    int accumulatorThreshold;
    int minCircleSize;
    int maxCircleSize;
    int distanceBetweenCircleCenters;

    // inRange Parameters
    int minSaturation;
    int maxSaturation;
    int minValue;
    int maxValue;
    int minHue;
    int maxHue;
    int dp;

    void initControlWindow();


};

#endif /* CONTROLWINDOW_HPP_ */
