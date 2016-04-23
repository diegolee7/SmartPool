#include "ObjectFinder.h"
#include <iostream>

//Window Names
const string ObjectFinder::windowName = "Object Finder Parameters";

//TrackBar Names
const string ObjectFinder::cannyThresholdTrackbarName = "Canny threshold";
const string ObjectFinder::accumulatorThresholdTrackbarName = "Accumulator Threshold";
const string ObjectFinder::maxCircleSizeTrackbarName = "Maximum Radius Size";
const string ObjectFinder::minCircleSizeTrackbarName = "Minimum Radius Size";
const string ObjectFinder::distanceBetweenCircleCentersTrackbarName = "Distance Between centers";


ObjectFinder::ObjectFinder() {
    initControlWindow();
    namedWindow("Histogram", WINDOW_AUTOSIZE );
}

void ObjectFinder::initControlWindow(){
    // create the main window
    namedWindow(windowName, WINDOW_AUTOSIZE);

    // initialize parameters that are subjects to change
    cannyThreshold = cannyThresholdInitialValue;
    accumulatorThreshold = accumulatorThresholdInitialValue;
    minCircleSize = minCircleSizeInitialValue;
    maxCircleSize = maxCircleSizeInitialValue;
    distanceBetweenCircleCenters = distanceBetweensCentersInitialValue;

    // Attach the trackbars
    createTrackbar(cannyThresholdTrackbarName, windowName, &cannyThreshold, maxCannyThreshold);
    createTrackbar(accumulatorThresholdTrackbarName, windowName, &accumulatorThreshold,
                   maxAccumulatorThreshold);
    createTrackbar(minCircleSizeTrackbarName, windowName, &minCircleSize, maxMinCircleSize);
    createTrackbar(maxCircleSizeTrackbarName, windowName, &maxCircleSize, maxMaxCircleSize);
    createTrackbar(distanceBetweenCircleCentersTrackbarName, windowName,
                   &distanceBetweenCircleCenters, maxDistanceBetweenCircleCenters);
}

 vector<Vec3f> ObjectFinder::getCircles(Mat frame) {
    // those paramaters cannot be =0
    // so we must check here
    cannyThreshold = std::max(cannyThreshold, 1);
    accumulatorThreshold = std::max(accumulatorThreshold, 1);
    distanceBetweenCircleCenters = std::max(distanceBetweenCircleCenters, 1);

    // Convert it to gray
    cvtColor(frame, frameGray, CV_BGR2GRAY);

    // Reduce the noise so we avoid false circle detection
    GaussianBlur(frameGray, frameGray, Size(3, 3), 2, 2);

    vector<Vec3f> circles;
    HoughCircles(frameGray, circles, CV_HOUGH_GRADIENT, 1, distanceBetweenCircleCenters , cannyThreshold,
                 accumulatorThreshold, minCircleSize, maxCircleSize);

    // Draw Circles on Gray frame with GaussianBlur
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        //circle(frameGray, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
        //circle(frameGray, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);
    }

    return circles;
}

void ObjectFinder::getMostFrequentColor (Mat frame, int* maxBlue, int* maxRed, int* maxGreen) {

    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( frame, bgr_planes );

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );


    Vec3b color = b_hist.at<Vec3b>(Point(0,255));

    // ... do something to the color ....
    cout << "\ncolor: "<<color;
    // set pixel
    b_hist.at<Vec3b>(Point(0,255)) = color;
    // save values

    double min, max ;
    Point minLoc, maxLoc;
    minMaxLoc(b_hist, &min, &max, &minLoc, &maxLoc);
    *maxBlue = maxLoc.y;
    minMaxLoc(g_hist, &min, &max, &minLoc, &maxLoc);
    *maxGreen = maxLoc.y;
    minMaxLoc(r_hist, &min, &max, &minLoc, &maxLoc);
    *maxRed = maxLoc.y;

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    //b_hist = b_hist/100;
    //g_hist = g_hist/100;
    //r_hist = r_hist/100;


    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 1, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 1, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 1, 8, 0  );
    }

    /// Display
    imshow("Histogram", histImage );
}

void ObjectFinder::segmentTable (Mat frame){
    int maxBlue = 0,maxRed = 0,maxGreen = 0;
    getMostFrequentColor(frame, &maxBlue, &maxRed, &maxGreen);

    //cout << "\nmax blue: "<<maxBlue;
    //cout << "\nmax green: "<<maxGreen;
    //cout << "\nmax red: "<<maxRed;

    //Vec3i color
    Mat m(1,1, CV_8UC3 ,Scalar(maxBlue,maxGreen, maxRed));
    std::cout << "\nBGR value: "<< m;
    cvtColor (m,m, COLOR_BGR2HSV);
    std::cout << "\nHSV value: "<< m;

    Mat frameHSV;
    cvtColor(frame, frameHSV, COLOR_BGR2HSV);
    Mat frameThresholded;
    Vec3b valuesHSV = m.at<Vec3b>(0,0);
    int hue = valuesHSV(0);
    int saturation = valuesHSV(1);
    int value = valuesHSV(2);
    int deltaHue = 10;
    int deltaSaturation = 50;
    int deltaValue = 50;
    //inRange(frameHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), frameThresholded);
    inRange(frameHSV, Scalar(hue-deltaHue, saturation-deltaSaturation, value - deltaValue)
           ,Scalar(hue+deltaHue, saturation+deltaSaturation, value + deltaValue), frameThresholded);

    //morphological opening (removes small objects from the foreground)
    erode(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    dilate(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );

    //morphological closing (removes small holes from the foreground)
    dilate(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    erode(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );

    imshow("Frame Thresholded", frameThresholded );
}













