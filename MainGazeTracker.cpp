#include "utils.h"
#include <fstream>
#include "MainGazeTracker.h"
#include <iostream>
#include "Global.h"

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "PupilTracker.h"
#include "cvx.h"
using namespace std;

class VideoWriter {
    CvVideoWriter *video;
public:
    VideoWriter(CvSize size) :
	video(cvCreateVideoWriter("out.avi", 0x58564944, 15.0, size))
    {}

    void write(const IplImage *image) {
	cvWriteFrame(video, image);
    }

    ~VideoWriter() {
	cvReleaseVideoWriter(&video);
    }
};



CommandLineArguments::CommandLineArguments(int argc, char** argv) {
    for(int i=1; i<argc; i++) {
	if (argv[i][0] == '-')
	    options.push_back(argv[i]);
	else
	    parameters.push_back(argv[i]);
    }
}

bool CommandLineArguments::isoption(const char* option) {
    xforeach(iter, options)
	if (!strcmp(*iter, option))
	    return true;
    return false;
}

VideoInput::VideoInput():
    capture(cvCaptureFromCAM(1)), framecount(0),
    frame(cvQueryFrame(capture)), size(cvSize(frame->width, frame->height))
{}

VideoInput::VideoInput(const char* filename):
    capture(cvCaptureFromFile(filename)), framecount(0),
    frame(cvQueryFrame(capture)), size(cvSize(frame->width, frame->height))
{}

void VideoInput::updateFrame() {
    framecount++;
    frame = cvQueryFrame(capture);
}

VideoInput::~VideoInput() {
    cvReleaseCapture( &capture );
}

MainGazeTracker::MainGazeTracker(int argc, char** argv,
				 const vector<boost::shared_ptr<AbstractStore> >
				 &stores):
    framestoreload(-1), stores(stores), autoreload(false)
//     , statemachine(shared_ptr<AlertWindow>(new AlertWindow("start")))
{
    CommandLineArguments args(argc, argv);

    if (args.parameters.size() == 0) {
	videoinput.reset(new VideoInput());
	if (args.isoption("--record"))
	    video.reset(new VideoWriter(videoinput->size));
    }
    else {
	videoinput.reset(new VideoInput(args.parameters[0]));
// 	if (args.parameters.size() >= 2)
// 	    fileinput.reset(new FileInput(parameters[1]));
    }


    canvas.reset(cvCreateImage(videoinput->size, 8, 3));
    tracking.reset(new TrackingSystem(videoinput->size));

}

void MainGazeTracker::addTracker(Point point) {
    tracking->tracker.addtracker(point);
}

void MainGazeTracker::savepoints() {
    try {
	tracking->tracker.save("tracker", "points.txt", videoinput->frame);
	autoreload = true;
    }
    catch (ios_base::failure &e) {
	cout << e.what() << endl;
    }
}

void MainGazeTracker::loadpoints() {
    try {
	tracking->tracker.load("tracker", "points.txt", videoinput->frame);
	autoreload = true;
    }
    catch (ios_base::failure &e) {
	cout << e.what() << endl;
    }
}

void MainGazeTracker::clearpoints() {
    tracking->tracker.cleartrackers();
    autoreload = false;
}

void MainGazeTracker::doprocessing(void) {

    framecount++;

    videoinput->updateFrame();
    const IplImage *frame = videoinput->frame;
///--------------------------------------------------------------------------------
        cv::Mat EyeImage(frame);

//        cv::resize(EyeImage, EyeImage, cv::Size(320, 240), 0, 0, cv::INTER_CUBIC);
//        cv::resize(EyeImage, EyeImage, cv::Size(640, 480), 0, 0, cv::INTER_CUBIC);
//
//
//        int rows = EyeImage.rows;
//        int cols = EyeImage.cols;
//
//        cout<<"size of image"<<rows<<"   "<<cols<<endl;


        pupiltracker::TrackerParams Eye_params;
        Eye_params.Radius_Min = 10;
        Eye_params.Radius_Max = 25;

        Eye_params.CannyBlur = 1;
        Eye_params.CannyThreshold1 = 20;
        Eye_params.CannyThreshold2 = 40;
        Eye_params.StarburstPoints = 0;

        Eye_params.PercentageInliers = 30;
        Eye_params.InlierIterations = 2;
        Eye_params.ImageAwareSupport = true;
        Eye_params.EarlyTerminationPercentage = 95;
        Eye_params.EarlyRejection = true;
        Eye_params.Seed = -1;

        pupiltracker::findPupilEllipse_out out;
        pupiltracker::tracker_log log;
        pupiltracker::findPupilEllipse(Eye_params, EyeImage, out, log);

        pupiltracker::cvx::cross(EyeImage, out.pPupil, 5, pupiltracker::cvx::rgb(255,255,0));

        cv::ellipse(EyeImage, out.elPupil, pupiltracker::cvx::rgb(255,0,255));
//        if(isCalibrating)
//        {
            cv::Mat_<cv::Point> matPoint(1, 1, cv::Point(0, 0));
            matPoint(0) = out.pPupil;
//            cout << matPoint(0, 0) << endl;
            IplImage copy = matPoint;
            Gaze G;
            G.TargetImage=&copy;

            cv::Mat centroid(G.TargetImage);
//        }
///----------------------------------------------------------------------------------------------

    if (video.get())
	video->write(frame);

    canvas->origin = frame->origin;
    cvCopy( frame, canvas.get(), 0 );

    try {
	tracking->doprocessing(frame, canvas.get(),centroid);
	if (tracking->gazetracker.isActive()) {
	    xforeach(iter, stores)
		(*iter)->store(tracking->gazetracker.output);
	}

// 	if (!tracking->tracker.areallpointsactive())
// 	    throw TrackingException();
	framestoreload = 20;
    }
    catch (TrackingException&) {
	framestoreload--;
    }
    framefunctions.process();
//     statemachine.handleEvent(EVENT_TICK);

    if (autoreload && framestoreload <= 0)
 	loadpoints();
}


MainGazeTracker::~MainGazeTracker(void) {
}


//void MainGazeTracker::addExemplar(Point exemplar) {
//    if (exemplar.x >= EyeExtractor::eyedx &&
//	exemplar.x + EyeExtractor::eyedx < videoinput->size.width &&
//	exemplar.y >= EyeExtractor::eyedy &&
//	exemplar.y + EyeExtractor::eyedy < videoinput->size.height)
//	tracking->gazetracker.addExemplar(exemplar,
//					  tracking->eyex.eyefloat.get(),
//					  tracking->eyex.eyegrey.get());
//}
void MainGazeTracker::addExemplar(Point exemplar) {
//    if (exemplar.x >= EyeExtractor::eyedx &&
//	exemplar.x + EyeExtractor::eyedx < videoinput->size.width &&
//	exemplar.y >= EyeExtractor::eyedy &&
//	exemplar.y + EyeExtractor::eyedy < videoinput->size.height)
//	{
//
//    cv::Mat Matrix(trackingsystem->centroid.clone());
//
//    IplImage copy = Matrix;
//    IplImage* TargetImage = &copy;
//
//	tracking->gazetracker.addExemplarSVM(exemplar,
//					  TargetImage,
//					  tracking->eyex.eyegrey.get());
//	}
}

static vector<Point> scalebyscreen(const vector<Point> &points) {
    Glib::RefPtr<Gdk::Screen> screen =
	Gdk::Display::get_default()->get_default_screen();
    return Calibrator::scaled(points, screen->get_width(), screen->get_height());
}

void MainGazeTracker::startCalibration() {

    boost::shared_ptr<WindowPointer>
	pointer(new WindowPointer(WindowPointer::PointerSpec(30,30,1,0,0)));

    ifstream calfile("calpoints.txt");

    isCalibrating = true;

    boost::shared_ptr<Calibrator>
	calibrator(new Calibrator(framecount, tracking,
				  scalebyscreen(Calibrator::loadpoints(calfile)),
				  pointer));

    framefunctions.clear();
    framefunctions.addchild(&framefunctions, calibrator);

}

void MainGazeTracker::startTesting() {
    vector<Point> points;
    for(double x=0.1; x<1; x+=0.2)
	for(double y=0.1; y<1; y+=0.2)
	    points.push_back(Point(x,y));

    isTesting=true;

    boost::shared_ptr<WindowPointer>
	pointer(new WindowPointer(WindowPointer::PointerSpec(30,30,0,1,0)));

    boost::shared_ptr<MovingTarget>
	moving(new MovingTarget(framecount, scalebyscreen(points), pointer));

    framefunctions.clear();
    framefunctions.addchild(&framefunctions, moving);
}
