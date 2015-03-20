#pragma once
//#include <vnl/algo/vnl_cholesky.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "svm.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Global.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

void parse_command_line();

template <class T>

class SVM_train {

private:
    vector<T> inputs;
    struct svm_model *model;
    struct svm_node *x_space;
    struct svm_problem prob;


public:
    SVM_train(vector<T> const& inputs,
		    vector<double> const& targets,int xy_scalor,int shift,int rescale):
    inputs(inputs)
    {
//    ofstream log;
//    log.open("log",fstream::app);

    size_t elements, j;
    j=0;
    int i;
    elements=3*num4training;
    prob.l=num4training;

    prob.y = Malloc(double,prob.l);
    prob.x = Malloc(struct svm_node *,prob.l);
    x_space = Malloc(struct svm_node,elements);


    for(int i=0; i<inputs.size(); i++) {

    prob.x[i] = &x_space[j];

    prob.y[i]=targets[i]*xy_scalor+shift;

    SharedImage tmpshare = inputs[i];
    const IplImage* tmpIpl = tmpshare.get();
    cv::Mat K(tmpIpl);

    for (int k =0 ;k<2;k++){
    x_space[j].index = (k+1);

    x_space[j].value =K.at<double>(0,k);

    j++;
    }
    x_space[j++].index = -1;
    }

    for(int i=inputs.size(); i<prob.l; i++) {
        prob.x[i] = &x_space[j];
        prob.y[i]=0.0;

        for (int k =0 ;k<2;k++){
        x_space[j].index = (k+1);

        x_space[j].value = 0.0;

        j++;
        }
        x_space[j++].index = -1;
    }
        // default values
        param.svm_type = 3;
        param.kernel_type = 2;
        param.degree = 3;
        param.gamma = 0.0000078;	// 1/num_features
        param.coef0 = 0;
        param.nu = 0.5;
        param.cache_size = 100;
        param.C = xy_scalor+shift;
        param.eps = 1e-8;
        param.p = 0.000000001;
        param.shrinking = 1;
        param.probability = 0;
        param.nr_weight = 0;
        param.weight_label = NULL;
        param.weight = NULL;
        cross_validation = 0;

        model = svm_train(&prob,&param);

//        char model_file_name[1024];
//
//        if(checkx) strcpy(model_file_name,"x.Model");
//        if(checky) strcpy(model_file_name,"y.Model");
//
//        svm_save_model(model_file_name,model);


    }

    ~SVM_train() {
    svm_free_and_destroy_model(&model);
    free(prob.y);
	free(prob.x);
    free(x_space);
    }

    double SVM_Predicts(vector<T> const &tests,int shift,int rescale) const {

    x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct svm_node));

    cv::Mat M(tests[0].get());


//    ofstream predictimage;
//    predictimage.open("inside",fstream::app);
//    predictimage<<M<<endl;
//
//    ofstream data;
//    data.open("inside data",fstream::app);

    double tmp;
    int i = 0;
    for (int k =0 ;k<2;k++)
    {
      x[i].index = (k+1);
//      x[i].value =M.at<int>(0,k);
      tmp=M.at<int>(0,k);
      x[i].value=tmp;
//      data<<x[i].value<<",";
      ++i;
    }
//    data<<endl;
    x[i].index = -1;

//    ofstream log;
//    log.open("log",fstream::app);
//
//    i=0;
//    log<<23<<" ";
//    for(int k = 0;k<32;k++){
//       log<<x[i].index<<":";
//       log<<x[i].value<<"  ";
//       ++i;
//    }
//    log<<endl;

//    double predict_label = (svm_predict(model,x)-shift)*rescale;
    double predict_label = svm_predict(model,x);

    if(predict_label<=5)
        predict_label=0;
    else
        predict_label=predict_label-shift;

    if(predict_label<0)
        predict_label=0;
    if(predict_label>27)
        predict_label=predict_label-2;

    predict_label = predict_label*rescale;

    free(x);
    return predict_label;
    }
    double SVM_Predict(T const& test,int shift,int rescale) const {
	return SVM_Predicts(vector<T>(1,test),shift,rescale);
    }
};


template <class T>
class SVMProcess {
    const SVM_train<T> svm;
    int xy_scalor;
    int shift;
    int rescale;

public:
    SVMProcess(vector<T> const& inputs,
				vector<double> const& targets,int xy_scalor,int shift,int rescale):
	svm(SVM_train<T>(inputs, targets,xy_scalor,shift,rescale))
    {}
    double SVM_Predict(T const& test,int shift,int rescale) const {
	return svm.SVM_Predict(test,shift,rescale);
    }
};


void parse_command_line();
