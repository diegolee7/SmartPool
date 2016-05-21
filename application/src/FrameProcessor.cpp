#include "FrameProcessor.hpp"

#include <iostream>


FrameProcessor::FrameProcessor() {

    showBalls = false;
    pMOG2 = createBackgroundSubtractorMOG2();
    maxRed = 0;
    maxBlue = 0;
    maxGreen = 0;
    updateControlVariables();
}

void FrameProcessor::updateControlVariables(){
	// Hough Circles Variables
    cannyThreshold = controlWindow.getCannyThreshold();
    accumulatorThreshold = controlWindow.getAccumulatorThreshold();
    distanceBetweenCircleCenters = controlWindow.getDistanceBetweenCircleCenters();
    minCircleSize = controlWindow.getMinCircleSize();
    maxCircleSize = controlWindow.getMaxCircleSize();
    dp = controlWindow.getDp();
    dilateSize = controlWindow.getDilateSize();
    erodeSize = controlWindow.getErodeSize();

    // inRange Parameters
    minSaturation = controlWindow.getMinSaturation();
    maxSaturation = controlWindow.getMaxSaturation();
    minValue = controlWindow.getMinValue();
    maxValue = controlWindow.getMaxValue();
    minHue = controlWindow.getMinHue();
    maxHue = controlWindow.getMaxHue();
}

void FrameProcessor::processFrame(Mat frame){
	//findMostFrequentColor(frame);
	updateControlVariables();
	//Mat subtractedFrame = backgroundSubtract(frame);

	//cout << "\nFrame segment";
	//Mat segmentedFrame = segmentTable(frame);

	//segmentedFrame = applyMedianBlur(segmentedFrame, 1, 3);
	//findCountours(segmentedFrame);
	//cout << "\nFind all balls";
	//allBalls = findAllBalls(segmentedFrame);
	//cout << "\nFind white ball";
	whiteBall = findWhiteBall(frame);
	//cout << "\nFind lines";
	//findLines(subtractedFrame);
}

void FrameProcessor::findCountours (Mat frame){
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int thresh = 100;
	RNG rng(12345);

	/// Detect edges using canny
	Canny( frame, canny_output, thresh, thresh*2, 3 );
	/// Find contours
	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Draw contours
	/*
	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ ) {
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
	}*/

	/// Get the moments
	vector<Moments> mu(contours.size() );
	for( unsigned int i = 0; i < contours.size(); i++ ) {
		mu[i] = moments( contours[i], false );
	}

	///  Get the mass centers:
	vector<Point2f> mc( contours.size() );
	for(unsigned int i = 0; i < contours.size(); i++ ) {
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	}

    // Draw Circles on Gray frame with GaussianBlur
	int j = 0;
	float contourArea;
    for (size_t i = 0; i < mc.size(); i++) {
    	contourArea = mu[i].m00;
    	if(contourArea > 400 && contourArea < 1200 ){
			Point2f c = mc[i];
			circle(frame, c, 16, Scalar(0,0,255), 1, CV_AA);
			circle(frame, c, 2, Scalar(0,255,0), 1, CV_AA);
			//ccprintf("\nitem %d size: %.2f",j, mu[i].m00);
			j++;
    	}
    }

	/// Show in a window
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", frame );
	//return vector<Point2f> mc
}

Mat FrameProcessor::applyMedianBlur(Mat frame, int iterations, int ksize){
    /// Applying Median blurs
    for ( int i = 1; i < iterations; i ++ ){
    	medianBlur ( frame, frame, ksize );
    }
	namedWindow( "Median Filter", 1 );
	imshow( "Median Filter", frame );
    return frame;
}

void FrameProcessor::findLines(Mat frame) {
	Mat dst, color_dst;
	vector<Vec4i> lines;
	Canny( frame, dst, 50, 200, 3 );
	cvtColor( dst, color_dst, CV_GRAY2BGR );
	HoughLinesP( dst, lines, 1, CV_PI/180, 80, 30, 10 );
	for( size_t i = 0; i < lines.size(); i++ )
	{
		line( color_dst, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
	}
	namedWindow( "Detected Lines", 1 );
	imshow( "Detected Lines", color_dst );
}

Mat FrameProcessor::backgroundSubtract(Mat frame) {
	//update the background model
	pMOG2->apply(frame, fgMaskMOG2);

	//get the frame number and write it on the current frame
	//stringstream ss;
	//rectangle(frame, cv::Point(10, 2), cv::Point(100,20),
	//	cv::Scalar(255,255,255), -1);
	//ss << capture.get(CV_CAP_PROP_POS_FRAMES);
	//string frameNumberString = ss.str();
	//putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
		//FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));

	//Show the FG masks;
	//imshow("FG Mask MOG", fgMaskMOG);
	//imshow("FG Mask MOG 2", fgMaskMOG2);
	return fgMaskMOG2;
}


 vector<Vec3f> FrameProcessor::findAllBalls(Mat frame) {

	//Check if frame is colored
	if (frame.channels() == 3) {
		// Convert it to gray
		cvtColor(frame, frameGray, CV_BGR2GRAY);
	} else {
		frameGray = frame;
	}

    // Reduce the noise so we avoid false circle detection
    GaussianBlur(frameGray, frameGray, Size(9 , 9), 2, 2);

    //Canny( frameGray, frameGray, cannyThreshold, cannyThreshold*3, 3 );

    vector<Vec3f> circles;
    HoughCircles(frameGray, circles, CV_HOUGH_GRADIENT, (double)dp/100, distanceBetweenCircleCenters , cannyThreshold,
                 accumulatorThreshold, minCircleSize, maxCircleSize);

    Canny( frameGray, frameGray, cannyThreshold, cannyThreshold*3, 3 );
    cvtColor(frameGray, frameGray, CV_GRAY2BGR);

    // Draw Circles on Gray frame with GaussianBlur
    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        circle(frameGray, Point(c[0], c[1]), c[2], Scalar(0,0,255), 1, CV_AA);
        circle(frameGray, Point(c[0], c[1]), 2, Scalar(0,255,0), 1, CV_AA);
    }

    imshow("Frame Blur", frameGray );
    return circles;
}


Mat FrameProcessor::segmentTable (Mat frame){
    //the commented part can be used later for auto finding table color
    /*
    //cout << "\nmax blue: "<<maxBlue;
    //cout << "\nmax green: "<<maxGreen;
    //cout << "\nmax red: "<<maxRed;

    //Vec3i color
    Mat m(1,1, CV_8UC3 ,Scalar(maxBlue,maxGreen, maxRed));
    std::cout << "\nBGR value: "<< m;
    cvtColor (m,m, COLOR_BGR2HSV);
    std::cout << "\nHSV value: "<< m;

    Vec3b valuesHSV = m.at<Vec3b>(0,0);
    int hue = valuesHSV(0);
    int saturation = valuesHSV(1);
    int value = valuesHSV(2);
    int deltaHue = 5;

    //cout << "\nMost frequent Hue: " << hue;
    //cout << "\nMost frequent Saturation: " << saturation;
    //cout << "\nMost frequent Value: " << value;
*/
    Mat frameThresholded;
    Mat frameHSV;
    cvtColor(frame, frameHSV, COLOR_BGR2HSV);

    //inRange(frameHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), frameThresholded);
    inRange(frameHSV, Scalar(minHue,minSaturation,minValue)
           ,Scalar(maxHue, maxSaturation, maxValue), frameThresholded);

    //morphological opening (removes small objects from the foreground)
    erode(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    dilate(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );

    //morphological closing (removes small holes from the foreground)
    dilate(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(dilateSize, dilateSize)) );
    erode(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(erodeSize, erodeSize)) );

    cvtColor(frameThresholded, frameThresholded, CV_GRAY2BGR);

    int c = waitKey(10);
    if ((char)c == 'n') {
        showBalls = !showBalls;
    }
    if(showBalls){
        bitwise_not(frameThresholded,frameThresholded);
        bitwise_and(frame, frameThresholded, frameThresholded);
        cout << "show balls: " << showBalls;
    }
    imshow("Frame Thresholded", frameThresholded );
    return frameThresholded;
}

vector<Vec3f> FrameProcessor::findWhiteBall(Mat frame){
    Mat frameThresholded;
    Mat frameHSV;
    //vtColor(frame, frameHSV, COLOR_BGR2HSV);

    //inRange(frameHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), frameThresholded);
    inRange(frame, Scalar(200,200,200)
           ,Scalar(255, 255, 255), frameThresholded);

    //morphological opening (removes small objects from the foreground)
    erode(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    dilate(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );

    //morphological closing (removes small holes from the foreground)
    dilate(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    erode(frameThresholded, frameThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );

    findLines(frameThresholded);

    // Reduce the noise so we avoid false circle detection
    GaussianBlur(frameThresholded, frameThresholded, Size(15, 15), 5, 5);

    vector<Vec3f> circles;
    HoughCircles(frameThresholded, circles, CV_HOUGH_GRADIENT, (double)dp/100, distanceBetweenCircleCenters , cannyThreshold,
                 30, 14, 16);

    for (size_t i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        cout << "\nWhite ball: (" << c[0] << "," << c[1] << ") radius=" << c[2];
    }


    imshow("White Ball", frameThresholded );
    return circles;
}


void FrameProcessor::findMostFrequentColor (Mat frame) {
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
    maxBlue = maxLoc.y;
    minMaxLoc(g_hist, &min, &max, &minLoc, &maxLoc);
    maxGreen = maxLoc.y;
    minMaxLoc(r_hist, &min, &max, &minLoc, &maxLoc);
    maxRed = maxLoc.y;

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

vector<Vec3f> FrameProcessor::getAllBalls() {
	return allBalls;
}

vector<Vec3f> FrameProcessor::getWhiteBalls() {
	return whiteBall;
}









