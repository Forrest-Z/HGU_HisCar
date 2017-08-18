#include <iostream>
#include "GPS.h"
#include "lanedetect.h"
#include "test.h"
#include "local.h"
#include "opencv2/opencv.hpp"
#include <thread>

using namespace std;

int main()
{
	double longitude = 0;
	double latitude = 0;
	double kmh = 0;
	double direction = 0;

	// variables used for lane detection 
	int key, frameNum = 1, frame_rate = 30, leftKept = 0, rightKept = 0;
	float prev_Rslope = 0, prev_Lslope = 0;
	Point prev_intersectionPoint(0, 0);


	//Global path part
	testCase6();


	char title[100] = "mono.wmv";
	VideoCapture capture(title);
	Mat frame;

	while(1)
	{

		GPSreceiver();

		local_path();	

	 	if (!capture.read(frame))
 		break;

		prev_intersectionPoint = findLineAndVP(preprocess(frame), frame, prev_Rslope, prev_Lslope, prev_intersectionPoint, leftKept, rightKept, key, frame_rate, frameNum);
	}
	

	//GPS part
	//receive current state of GPS

	/*	GPSreceiver(longitude,latitude,kmh,direction);
	
	printf("Longitude: %f\n",longitude);
	printf("latitude: %f\n",latitude);
	printf("Speed: %f\n",kmh);
	printf("Direction: %f\n",direction);*/


	//Local path part








	return 0;
}
