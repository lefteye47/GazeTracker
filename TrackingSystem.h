#pragma once
#include "utils.h"
#include "PointTracker.h"
#include "HeadTracker.h"
#include "HeadCompensation.cpp"
#include "EyeExtractor.h"
#include "OutputMethods.h"
#include "GazeTracker.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

struct TrackingSystem {
    PointTracker tracker;
    HeadTracker headtracker;
    HeadCompensation headcomp;
    EyeExtractor eyex;
    cv::Mat centroid;
    GazeTracker gazetracker;

    TrackingSystem(CvSize size);
    void doprocessing(const IplImage *frame,
		      IplImage *image,cv::Mat centroid);
    void displayeye(IplImage *image, int basex, int basey,
		    int stepx, int stepy);

};
