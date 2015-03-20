#include "Global.h"
// declaration of g_nValue
bool isTesting = false;
bool isCalibrating = false;
bool checkx = false;
bool checky = false;

struct svm_parameter param;		// set by parse_command_line
//struct svm_problem prob;		// set by read_problem
//struct svm_model *model;
//struct svm_node *x_space;
int cross_validation;
struct svm_node *x;
int max_nr_attr = 64;

//int num4training = 13;
int num4training = 49;
int x_size=1366;
int y_size=768;
