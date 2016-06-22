/*
 * ControlWindow.cpp
 *
 *  Created on: May 15, 2016
 *      Author: diego
 */

#include "ControlWindow.hpp"
#include <iostream>

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

    // initialize HoughCircles trackbars
    cannyThreshold = cannyThresholdInitialValue;
    accumulatorThreshold = accumulatorThresholdInitialValue;
    minCircleSize = minCircleSizeInitialValue;
    maxCircleSize = maxCircleSizeInitialValue;
    distanceBetweenCircleCenters = distanceBetweensCentersInitialValue;
    dp = 100;
    erodeSize = 10;
    dilateSize = 10;

    //initialize inRange threshold trackbars
    minHue = 40;
    maxHue = 107;
    minSaturation = 58	;
    maxSaturation = 153;
    minValue = 124;
    maxValue = 255;

    // Attach HoughCircles trackbars
    createTrackbar(cannyThresholdTrackbarName, controlWindowName, &cannyThreshold, maxCannyThreshold);
    createTrackbar(accumulatorThresholdTrackbarName, controlWindowName, &accumulatorThreshold,
                   maxAccumulatorThreshold);
    createTrackbar(minCircleSizeTrackbarName, controlWindowName, &minCircleSize, maxMinCircleSize);
    createTrackbar(maxCircleSizeTrackbarName, controlWindowName, &maxCircleSize, maxMaxCircleSize);
    createTrackbar(distanceBetweenCircleCentersTrackbarName, controlWindowName,
                   &distanceBetweenCircleCenters, maxDistanceBetweenCircleCenters);
    createTrackbar("dp", controlWindowName, &dp, 500);
    createTrackbar("erodeSize", controlWindowName, &erodeSize, 10);
    createTrackbar("dilateSize", controlWindowName, &dilateSize, 10);

    // Attach inRange trackbars
    createTrackbar("minHue", controlWindowName, &minHue, 180);
    createTrackbar("maxHue", controlWindowName, &maxHue, 180);
    createTrackbar("minSaturation", controlWindowName, &minSaturation, 255);
    createTrackbar("maxSaturation", controlWindowName, &maxSaturation, 255);
    createTrackbar("minValue", controlWindowName, &minValue, 255);
    createTrackbar("maxValue", controlWindowName, &maxValue, 255);
}

int ControlWindow::getAccumulatorThreshold() {
	accumulatorThreshold = max(accumulatorThreshold, 1);
	return accumulatorThreshold;
}

int ControlWindow::getCannyThreshold() {
	cannyThreshold = max(cannyThreshold, 1);
	return cannyThreshold;
}

int ControlWindow::getDistanceBetweenCircleCenters() {
	distanceBetweenCircleCenters = max(distanceBetweenCircleCenters, 1);
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

int ControlWindow::getErodeSize() {
	erodeSize = max(erodeSize, 1);
	return erodeSize;
}

int ControlWindow::getDilateSize() {
	dilateSize = max(dilateSize, 1);
	return dilateSize;
}
