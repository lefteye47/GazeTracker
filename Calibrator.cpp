#include "Calibrator.h"
#include <fstream>
#include "Global.h"
#include <iostream>
#include "utils.h"
//#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "PupilTracker.h"
#include "cvx.h"
#include <time.h>

Calibrator::~Calibrator() {
#ifdef DEBUG
    cout << "Destroying calibrator" << endl;
#endif
}

FrameFunction::~FrameFunction() {
#ifdef DEBUG
    cout << "Destroying framefunction" << endl;
#endif
}

MovingTarget::MovingTarget(const int &frameno,
			   const vector<Point>& points,
			   const boost::shared_ptr<WindowPointer> &pointer,
			   int dwelltime):
    FrameFunction(frameno),
    points(points), dwelltime(dwelltime), pointer(pointer)
{
};

MovingTarget::~MovingTarget() {
    int id = getFrame() / dwelltime;
}

void MovingTarget::process() {
    if (active()) {
	int id = getPointNo();
	if (getPointFrame() == 1)
	    pointer->setPosition((int)points[id].x, (int)points[id].y);

	    cout<< points[id].x << "," <<points[id].y<<"\n";

// wirte to a file
//    if(isCalibrating)
//{
//    char fixedpoint [50];
//    sprintf (fixedpoint, "%.2lf  %.2lf \n",points[id].x,points[id].y);
//    ofstream myfile;
//    myfile.open ("CP.txt",fstream::app);
//    myfile << fixedpoint;
//    myfile.close();
//}

//    if(isTesting)
//{
//    char fixedpoint [50];
//    sprintf (fixedpoint, "%.2lf  %.2lf \n",points[id].x,points[id].y);
//    ofstream myfile;
//    myfile.open ("target.txt",fstream::app);
//    myfile << fixedpoint;
//    myfile.close();
//}
    }
    else
	detach();
}

bool MovingTarget::active() {
    return getPointNo() < (int) points.size();
}

int MovingTarget::getPointNo() {
    return getFrame() / dwelltime;
}

int MovingTarget::getPointFrame() {

    return getFrame() % dwelltime;
}

Calibrator::Calibrator(const int &framecount,
		       const boost::shared_ptr<TrackingSystem> &trackingsystem,
		       const vector<Point>& points,
		       const boost::shared_ptr<WindowPointer> &pointer,
		       int dwelltime):
    MovingTarget(framecount, points, pointer, dwelltime),
    trackingsystem(trackingsystem)
{
    trackingsystem->gazetracker.clear();
    // todo: remove all calibration points
}
void Calibrator::process() {
    int x,y;
    clock_t t;
    if (active()) {

	int id = getPointNo();
	int frame = getPointFrame();

	if (frame == 1) {// start
	    averagemat.clear();
	}

	if (frame >= dwelltime/2){ // middle
        averagemat.push_back(trackingsystem->centroid.clone());
	}

	if (frame == dwelltime-1) { // end
      double averagex,averagey,sumx = 0,sumy=0;

      for(int i=0;i<(averagemat.size());i++){
      cv::Mat gaze=averagemat.at(i);
      x = gaze.at<int>(0,0);
      y = gaze.at<int>(0,1);
      sumx+=x;
      sumy+=y;
      }

      averagex=(double)sumx/averagemat.size();
      averagey=(double)sumy/averagemat.size();

      cv::Mat gazematrix = (cv::Mat_<double>(1,2) << averagex,averagey);

      IplImage copy = gazematrix;
      IplImage* TargetImage = &copy;
//      ofstream M;
//      M.open("Input Calibrating",fstream::app);
//      M<<gazematrix<<endl;

//        trackingsystem->gazetracker.addExemplarSVM(points[id], TargetImage,
//			    trackingsystem->eyex.eyegrey.get());
//      cout<<points[id]<<"     "<<TargetImage<<endl;
      t=clock();
        trackingsystem->gazetracker.addExemplarSVM(points[id], TargetImage,
			    TargetImage);
      t=clock()-t;
      cout<<"Interval time t is equal to =  "<<t<<endl;
	}
    }
    MovingTarget::process();
}
///13 calibration points
//const Point Calibrator::defaultpointarr[] = {Point(0.5, 0.5),
//					     Point(0.1, 0.5), Point(0.9, 0.5),
//					     Point(0.5, 0.1), Point(0.5, 0.9),
//					     Point(0.1, 0.1), Point(0.1, 0.9),
//					     Point(0.9, 0.9), Point(0.9, 0.1),
//					     Point(0.3, 0.3), Point(0.3, 0.7),
//					     Point(0.7, 0.7), Point(0.7, 0.3)};

//36 calibration points
//const Point Calibrator::defaultpointarr[] = {Point(0.0, 0.0),
//					     Point(0.0, 0.2), Point(0.0, 0.4),
//					     Point(0.0, 0.6), Point(0.0, 0.8),
//					     Point(0.0, 0.9), Point(0.2, 0.0),
//					     Point(0.2, 0.2), Point(0.2, 0.4),
//					     Point(0.2, 0.6), Point(0.2, 0.8),
//					     Point(0.2, 0.9), Point(0.4, 0.0),
//					     Point(0.4, 0.2), Point(0.4, 0.4),
//					     Point(0.4, 0.6), Point(0.4, 0.8),
//					     Point(0.4, 0.9), Point(0.6, 0.0),
//					     Point(0.6, 0.2), Point(0.6, 0.4),
//					     Point(0.6, 0.6), Point(0.6, 0.8),
//					     Point(0.6, 0.9), Point(0.8, 0.0),
//					     Point(0.8, 0.2), Point(0.8, 0.4),
//					     Point(0.8, 0.6), Point(0.8, 0.8),
//					     Point(0.8, 0.9), Point(0.9, 0.0),
//					     Point(0.9, 0.2), Point(0.9, 0.4),
//					     Point(0.9, 0.6), Point(0.9, 0.8),
//					     Point(0.9,0.9)};

const Point Calibrator::defaultpointarr[] = {Point(0.0, 0.0),Point(0.0, 0.15),
					     Point(0.0, 0.3), Point(0.0, 0.45),
					     Point(0.0, 0.6), Point(0.0, 0.75),
					     Point(0.0, 0.9), Point(0.15, 0.0),
					     Point(0.15, 0.15), Point(0.15, 0.3),Point(0.15, 0.45),
					     Point(0.15, 0.6), Point(0.15, 0.75),
					     Point(0.15, 0.9), Point(0.3, 0.0),
					     Point(0.3, 0.15), Point(0.3, 0.3),
					     Point(0.3, 0.45), Point(0.3, 0.6),
					     Point(0.3, 0.75),Point(0.3, 0.9),Point(0.6, 0.0),
					     Point(0.6, 0.15), Point(0.6, 0.3),
					     Point(0.6, 0.45),Point(0.6, 0.6), Point(0.6, 0.75),
					     Point(0.6, 0.9), Point(0.75, 0.0),
					     Point(0.75, 0.15), Point(0.75, 0.3),
					     Point(0.75, 0.45),Point(0.75, 0.6), Point(0.75, 0.75),
					     Point(0.75, 0.9), Point(0.9, 0.0),
					     Point(0.9, 0.15), Point(0.9, 0.3),
					     Point(0.9, 0.45),Point(0.9, 0.6), Point(0.9, 0.75),
					     Point(0.9,0.9)};
vector<Point>
Calibrator::defaultpoints(Calibrator::defaultpointarr,
			  Calibrator::defaultpointarr+
			  (sizeof(Calibrator::defaultpointarr) /
			   sizeof(Calibrator::defaultpointarr[0])));

vector<Point> Calibrator::loadpoints(istream& in) {
    vector<Point> result;

    for(;;) {
	double x, y;
	in >> x >> y;
	if (in.rdstate()) break; // break if any error
	result.push_back(Point(x, y));
    }

    return result;
}

vector<Point> Calibrator::scaled(const vector<Point> &points,
				      double x, double y)
{
//     double dx = x > y ? (x-y)/2 : 0.0;
//     double dy = y > x ? (y-x)/2 : 0.0;
//     double scale = x > y ? y : x;

    vector<Point> result;

    xforeach(iter, points)
	result.push_back(Point(iter->x * x, iter->y * y));
// 	result.push_back(Point(iter->x * scale + dx, iter->y * scale + dy));

    return result;
}
