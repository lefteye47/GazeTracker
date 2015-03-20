#ifndef GLOBAL_H // header guards
#define GLOBAL_H
#include "svm.h"

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

extern bool isTesting;
extern bool isCalibrating;
extern bool checkx;
extern bool checky;

extern struct svm_parameter param;		// set by parse_command_line
//extern struct svm_problem prob;		// set by read_problem
//extern struct svm_model *model;
//extern struct svm_node *x_space;
extern int cross_validation;
extern struct svm_node *x;
extern int max_nr_attr;
extern int num4training;
extern int x_size;
extern int y_size;

class Gaze{

public:
IplImage * TargetImage;

};


#endif
