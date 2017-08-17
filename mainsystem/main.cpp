#include <iostream>
#include "GPS.h"
#include "lanedetect.h"

using namespace std;

int main()
{
	double longitude = 0;
	double latitude = 0;
	double kmh = 0;
	double direction = 0;

//	while(1)
//	{

			//receive current state of GPS
	GPSreceiver();
	/*	GPSreceiver(longitude,latitude,kmh,direction);
	printf("Longitude: %f\n",longitude);
	printf("latitude: %f\n",latitude);
	printf("Speed: %f\n",kmh);
	printf("Direction: %f\n",direction);*/


//	}
	
	lanedetection();




	return 0;
}