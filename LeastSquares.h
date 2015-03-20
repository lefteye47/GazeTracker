#pragma once
//#include <vnl/vnl_vector.h>
//#include <vnl/vnl_matrix.h>

//typedef vnl_vector<double> Vector;
//typedef vnl_matrix<double> Matrix;
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


class LeastSquares {
//    Matrix X;
//    Vector Y;
      cv::Mat X;
      vector<double> Y;
public:
//    const int n;
    int n;

//    LeastSquares(int n): X.at<double>(n,n), Y(n), n(n) {}
    LeastSquares(int n){this->n = n;}

    void addSample(double xs[], double y);
    void addSample(double x1, double x2, double y);
    void addSample(double x1, double x2, double x3, double y);

    vector<double> solve(void);
    void solve(double &a0, double &a1);
    void solve(double &a0, double &a1, double &a2);
};


