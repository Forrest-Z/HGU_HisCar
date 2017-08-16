#include "opencv2/opencv.hpp"
#include <iostream>
#include "DGPS.h"

using namespace std;
using namespace cv;

int main()
{
	namedWindow("cmd");

	cout << fixed;
	cout.precision(6);

	GPS_DD A, B;
	cout << "A 포인트 위도: ";
	cin >> A.lat;
	cout << "A 포인트 경도: ";
	cin >> A.lon;

	cout << endl;
	cout << "B 포인트 위도: ";
	cin >> B.lat;
	cout << "B 포인트 경도: ";
	cin >> B.lon;



	cout << endl << "/////////////////////////////////////////";
	double degree, distance;
	distance = GPS_DD2Dist(A, B);
	degree = GPS_DD2Deg(A, B);
	cout << endl << "A와 B 사이 거리 : " << distance << "m" << endl;
	cout << "A와 B 사이 방위각 : " << degree << "(degree)" << endl;
	cout << "/////////////////////////////////////////" << endl << endl;


	GPS_DD C = DistDeg2GPS_DD(A, distance, degree);



	cout << "C 포인트 위도: " << C.lat << endl;
	cout << "C 포인트 경도: " << C.lon << endl;


	distance = GPS_DD2Dist(A, C);
	degree = GPS_DD2Deg(A, C);

	cout << endl << "/////////////////////////////////////////";
	cout << endl << "A와 C 사이 거리 : " << distance << "m" << endl;
	cout << "A와 C 사이 방위각 : " << degree << "(degree)" << endl;
	cout << "/////////////////////////////////////////" << endl;

	waitKey();


}