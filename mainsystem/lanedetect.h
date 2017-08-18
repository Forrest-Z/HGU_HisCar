#include "opencv2/opencv.hpp"
#include <cmath>
#include "opencv/cv.hpp"
#include "opencv/highgui.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <queue>

using namespace std;
using namespace cv;

Mat preprocess(Mat& frame);
void findandDrawContour(Mat& roi, char* windowName, int type);
void getMinMax(Mat& roi, double& min, double& max);
Point findLineAndVP(Mat white, Mat& frame, float& prev_Rslope, float& prev_Lslope, Point intersectionPoint, int& leftKept, int& rightKept, int& key, int& frame_rate, int& frameNum);
//void lanedetection();
void lanedetection(Mat frame);