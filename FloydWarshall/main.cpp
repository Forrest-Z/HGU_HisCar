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
	cout << "A ����Ʈ ����: ";
	cin >> A.lat;
	cout << "A ����Ʈ �浵: ";
	cin >> A.lon;

	cout << endl;
	cout << "B ����Ʈ ����: ";
	cin >> B.lat;
	cout << "B ����Ʈ �浵: ";
	cin >> B.lon;



	cout << endl << "/////////////////////////////////////////";
	double degree, distance;
	distance = GPS_DD2Dist(A, B);
	degree = GPS_DD2Deg(A, B);
	cout << endl << "A�� B ���� �Ÿ� : " << distance << "m" << endl;
	cout << "A�� B ���� ������ : " << degree << "(degree)" << endl;
	cout << "/////////////////////////////////////////" << endl << endl;


	GPS_DD C = DistDeg2GPS_DD(A, distance, degree);



	cout << "C ����Ʈ ����: " << C.lat << endl;
	cout << "C ����Ʈ �浵: " << C.lon << endl;


	distance = GPS_DD2Dist(A, C);
	degree = GPS_DD2Deg(A, C);

	cout << endl << "/////////////////////////////////////////";
	cout << endl << "A�� C ���� �Ÿ� : " << distance << "m" << endl;
	cout << "A�� C ���� ������ : " << degree << "(degree)" << endl;
	cout << "/////////////////////////////////////////" << endl;

	waitKey();


}