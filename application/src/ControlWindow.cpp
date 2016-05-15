/*
 * ControlWindow.cpp
 *
 *  Created on: May 15, 2016
 *      Author: diego
 */

#include "ControlWindow.hpp"

ControlWindow::ControlWindow() {
	initControlWindow();

}

ControlWindow::~ControlWindow() {

}

//Window Names
const string ControlWindow::controlWindowName = "Control";

//TrackBar Names
const string ControlWindow::cannyThresholdTrackbarName = "Canny threshold";
const string ControlWindow::accumulatorThresholdTrackbarName = "Accumulator Threshold";
const string ControlWindow::maxCircleSizeTrackbarName = "Maximum Radius Size";
const string ControlWindow::minCircleSizeTrackbarName = "Minimum Radius Size";
const string ControlWindow::distanceBetweenCircleCentersTrackbarName = "Distance Between centers";


void ControlWindow::initControlWindow(){
    // create the main window
    namedWindow(controlWindowName, WINDOW_AUTOSIZE);

    // initialize parameters that are subjects to change
    cannyThreshold = cannyThresholdInitialValue;
    accumulatorThreshold = accumulatorThresholdInitialValue;
    minCircleSize = minCircleSizeInitialValue;
    maxCircleSize = maxCircleSizeInitialValue;
    distanceBetweenCircleCenters = distanceBetweensCentersInitialValue;

    // Attach the trackbars
    createTrackbar(cannyThresholdTrackbarName, controlWindowName, &cannyThreshold, maxCannyThreshold);
    createTrackbar(accumulatorThresholdTrackbarName, controlWindowName, &accumulatorThreshold,
                   maxAccumulatorThreshold);
    createTrackbar(minCircleSizeTrackbarName, controlWindowName, &minCircleSize, maxMinCircleSize);
    createTrackbar(maxCircleSizeTrackbarName, controlWindowName, &maxCircleSize, maxMaxCircleSize);
    createTrackbar(distanceBetweenCircleCentersTrackbarName, controlWindowName,
                   &distanceBetweenCircleCenters, maxDistanceBetweenCircleCenters);

    dp = 100;
    createTrackbar("dp", controlWindowName, &dp, 500);

    //threshold trackbars
    minHue = 60;
    maxHue = 98;
    minSaturation = 66;
    maxSaturation = 158;
    minValue = 30;
    maxValue = 220;
    createTrackbar("minHue", controlWindowName, &minHue, 180);
    createTrackbar("maxHue", controlWindowName, &maxHue, 180);
    createTrackbar("minSaturation", controlWindowName, &minSaturation, 255);
    createTrackbar("maxSaturation", controlWindowName, &maxSaturation, 255);
    createTrackbar("minValue", controlWindowName, &minValue, 255);
    createTrackbar("maxValue", controlWindowName, &maxValue, 255);
}


int ControlWindow::getAccumulatorThreshold() {
	max(accumulatorThreshold, 1);
	return accumulatorThreshold;
}

int ControlWindow::getCannyThreshold() {
	max(cannyThreshold, 1);
	return cannyThreshold;
}

int ControlWindow::getDistanceBetweenCircleCenters() {
	max(distanceBetweenCircleCenters, 1);
	return distanceBetweenCircleCenters;
}

int ControlWindow::getDp() {
	return dp;
}

int ControlWindow::getMaxCircleSize() {
	return maxCircleSize;
}

int ControlWindow::getMaxHue() {
	return maxHue;
}

int ControlWindow::getMaxSaturation() {
	return maxSaturation;
}

int ControlWindow::getMaxValue() {
	return maxValue;
}

int ControlWindow::getMinCircleSize() {
	return minCircleSize;
}

int ControlWindow::getMinHue() {
	return minHue;
}

int ControlWindow::getMinSaturation() {
	return minSaturation;
}

int ControlWindow::getMinValue() {
	return minValue;
}