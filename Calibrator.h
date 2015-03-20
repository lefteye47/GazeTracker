#pragma once
#include <glibmm.h>
#include <gdkmm.h>
#include "utils.h"
#include "TrackingSystem.h"
#include "Containers.h"
#include "GraphicalPointer.h"
#include "FeatureDetector.h"

const int duration = 40;

class FrameProcessing;

class FrameFunction:
public Containee<FrameProcessing, FrameFunction>
{
    const int &frameno;
    int startframe;
 protected:
    int getFrame() { return frameno - startframe; }
public:
    FrameFunction(const int &frameno): frameno(frameno), startframe(frameno) {}
    virtual void process()=0;
    virtual ~FrameFunction();
};

class FrameProcessing:
public ProcessContainer<FrameProcessing,FrameFunction> {};

class MovingTarget: public FrameFunction {
    boost::shared_ptr<WindowPointer> pointer;
 public:
    MovingTarget(const int &frameno,
		 const vector<Point>& points,
		 const boost::shared_ptr<WindowPointer> &pointer,
		 int dwelltime=duration);
    virtual ~MovingTarget();
    virtual void process();
 protected:
    vector<Point> points;
    const int dwelltime;
    int getPointNo();
    int getPointFrame();
    bool active();
};

class Calibrator: public MovingTarget {
    static const Point defaultpointarr[];
    boost::shared_ptr<TrackingSystem> trackingsystem;
    scoped_ptr<FeatureDetector> averageeye;
    vector<cv::Mat> averagemat;
public:
    static vector<Point> defaultpoints;
    static vector<Point> loadpoints(istream& in);
    Calibrator(const int &frameno,
	       const boost::shared_ptr<TrackingSystem> &trackingsystem,
	       const vector<Point>& points,
	       const boost::shared_ptr<WindowPointer> &pointer,
	       int dwelltime=duration);
    virtual ~Calibrator();
    virtual void process();
    static vector<Point> scaled(const vector<Point>& points, double x, double y);
};


