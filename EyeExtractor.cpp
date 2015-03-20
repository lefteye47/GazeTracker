#include "utils.h"
#include "EyeExtractor.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "PupilTracker.h"
#include "cvx.h"

#include "Global.h"

const int EyeExtractor::eyedx = 32;
const int EyeExtractor::eyedy = 16;
const CvSize EyeExtractor::eyesize = cvSize(eyedx*2, eyedy*2);
//const int EyeExtractor::eyedx = 2;
//const int EyeExtractor::eyedy = 1;
//const CvSize EyeExtractor::eyesize = cvSize(eyedx, eyedy);

void EyeExtractor::processEye(void) {
    cvConvertScale(eyegrey.get(), eyefloat2.get());
    // todo: equalize it somehow first!
    cvSmooth(eyefloat2.get(), eyefloat.get(), CV_GAUSSIAN, 3);
    cvEqualizeHist(eyegrey.get(), eyegrey.get());
}


EyeExtractor::EyeExtractor(const PointTracker &tracker):
    tracker(tracker),
    eyefloat2(cvCreateImage( eyesize, IPL_DEPTH_32F, 1 )),
    eyegrey(cvCreateImage( eyesize, 8, 1 )),
    eyefloat(cvCreateImage( eyesize, IPL_DEPTH_32F, 1 )),
    eyeimage(cvCreateImage( eyesize, 8, 3 ))
{
}

void EyeExtractor::extractEye(const IplImage *origimage)
    throw (TrackingException)
{
//    if (!tracker.status[tracker.eyepoint1])
//	throw TrackingException();

//    double x0 = tracker.currentpoints[tracker.eyepoint1].x;
//    double y0 = tracker.currentpoints[tracker.eyepoint1].y;
//    double x1 = tracker.currentpoints[tracker.eyepoint2].x;
//    double y1 = tracker.currentpoints[tracker.eyepoint2].y;
//    double factor = 0.17;
//    double xfactor = 0.05;
//    double yfactor = 0.20 * (x0 < x1 ? -1 : 1);
//    double L = factor / eyedx;
//    double LL = x0 < x1? L : -L;
//    double matrix[6] =
//	{LL*(x1-x0), LL*(y0-y1),
//	 x0 + factor * ((1-xfactor)*(x1-x0) + yfactor * (y0-y1)),
//	 LL*(y1-y0), LL*(x1-x0),
//	 y0 + factor * ((1-xfactor)*(y1-y0) + yfactor * (x1-x0))};
//    CvMat M = cvMat( 2, 3, CV_32F, matrix );
//
//    cvGetQuadrangleSubPix( origimage, eyeimage.get(), &M);
//    cvCvtColor(eyeimage.get(), eyegrey.get(), CV_RGB2GRAY);

//-----------------------------------------------------------------
//        cv::Mat EyeImage(origimage);
//
//        pupiltracker::TrackerParams Eye_params;
//        Eye_params.Radius_Min = 30;
//        Eye_params.Radius_Max = 45;
//
//        Eye_params.CannyBlur = 1;
//        Eye_params.CannyThreshold1 = 20;
//        Eye_params.CannyThreshold2 = 40;
//        Eye_params.StarburstPoints = 0;
//
//        Eye_params.PercentageInliers = 30;
//        Eye_params.InlierIterations = 2;
//        Eye_params.ImageAwareSupport = true;
//        Eye_params.EarlyTerminationPercentage = 95;
//        Eye_params.EarlyRejection = true;
//        Eye_params.Seed = -1;
//
//        pupiltracker::findPupilEllipse_out out;
//        pupiltracker::tracker_log log;
//        pupiltracker::findPupilEllipse(Eye_params, EyeImage, out, log);
//
//        pupiltracker::cvx::cross(EyeImage, out.pPupil, 5, pupiltracker::cvx::rgb(255,255,0));
//
//        cv::ellipse(EyeImage, out.elPupil, pupiltracker::cvx::rgb(255,0,255));
//
//        if(isCalibrating)
//        {
//            cv::Mat_<cv::Point> matPoint(1, 1, cv::Point(0, 0));
//            matPoint(0) = out.pPupil;
////            cout << matPoint(0, 0) << endl;
//            IplImage copy = matPoint;
//            Gaze G;
//            G.TargetImage=&copy;
//
//            cv::Mat Matrix(G.TargetImage);
//            cout<<Matrix<<endl;
//        }
//-----------------------------------------------------------------------

    processEye();
}

EyeExtractor::~EyeExtractor(void) {
}


