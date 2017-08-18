#include <fstream>
#include <vector>
#include <iostream>
#include "DGPS.h"
#include "LocalPath.h"
#include "test.h"
#include "ShortestPath_Interface.h"
#include "TransverseControl.h"

using namespace std;


void testCase1()
{
	cout << fixed;
	cout.precision(10);
	cout << "##############################################" << endl;
	cout << "# Test Case 1: Calculate Azimuth and Distance" << endl;
	cout << "##############################################" << endl<< endl;
	GPS_DD A = { 129.388734, 36.102799 }, B = { 129.388374, 36.101938}, C;
	cout << "A Point Longtitude: " << A.lon << endl;
	cout << "A Point Latitude: " << A.lat << endl;
	cout << "B Point Longtitude: " << B.lon << endl;
	cout << "B Point Latitude: " << B.lat << endl;
	
	double degree, distance;


	cout << endl << "/////////////////////////////////////////";

	distance = GPS_DD2Dist(A, B);
	degree = GPS_DD2Deg(A, B);
	cout << endl << "A-B Distance : " << distance << "m" << endl;
	cout << "A- B  Azimuth : " << degree << "(degree)" << endl;
	cout << "/////////////////////////////////////////" << endl << endl;

	C = DistDeg2GPS_DD(A, distance, degree);

	cout << "C Point Longtitude: " << C.lon << endl;
	cout << "C Point Latitude: " << C.lat << endl;
	


	distance = GPS_DD2Dist(A, C);
	degree = GPS_DD2Deg(A, C);

	cout << endl << "/////////////////////////////////////////";
	cout << endl << "A-C  Distance : " << distance << "m" << endl;
	cout << "A-C  Azimuth : " << degree << "(degree)" << endl;
	cout << "/////////////////////////////////////////" << endl << endl;

	clearScreen();



}

void testCase2() //Local Map Range Setting
{
	ofstream fout1;
	
	fout1 << fixed;
	fout1.precision(14);

	fout1.open("./LocalRange.csv");
//	fout1.open(".\\LocalRange.csv");


	cout << fixed;
	cout.precision(10);
	cout << "##############################################" << endl;
	cout << "# Test Case 2: Setting Local map range" << endl;
	cout << "##############################################" << endl << endl;
	GPS_DD A = { 129.388734, 36.102799 }, B = { 129.388374, 36.101938 }, C = { 129.389413,36.101714 };

	Rect_double LocalRange;
	LocalRange = LocalMapRange(A);

	cout << "/////////////////////////////////////////////////////////////////" << endl;
	cout << "                       Local Range A                      " << endl;
	cout << "/////////////////////////////////////////////////////////////////" << endl;
	cout << "Center Point(x,y)" << A.lon << "," << A.lat << endl;
	cout << "Top Left(x,y): " << LocalRange.tl.x << "," << LocalRange.tl.y << endl;
	cout << "Top Right(x,y): " << LocalRange.tr.x << "," << LocalRange.tr.y << endl;
	cout << "Bottom Left(x,y): " << LocalRange.bl.x << "," << LocalRange.bl.y << endl;
	cout << "Bottom Right(x,y): " << LocalRange.br.x << "," << LocalRange.br.y << endl;
	cout << "Width(meter): " << LocalRange.width << endl;
	cout << "Height(meter): " << LocalRange.height << endl;
	cout << "/////////////////////////////////////////////////////////////////" << endl;

	fout1 << "Local Range A" << endl;
	fout1 << A.lon << ',' << A.lat << endl;
	fout1 << LocalRange.tl.x << "," << LocalRange.tl.y << endl;
	fout1 << LocalRange.tr.x << "," << LocalRange.tr.y << endl;
	fout1 << LocalRange.bl.x << "," << LocalRange.bl.y << endl;
	fout1 << LocalRange.br.x << "," << LocalRange.br.y << endl;

	clearScreen();

	LocalRange = LocalMapRange(B);

	cout << "/////////////////////////////////////////////////////////////////" << endl;
	cout << "                       Local Range B                       " << endl;
	cout << "/////////////////////////////////////////////////////////////////" << endl;
	cout << "Center Point(x,y)" << B.lon << "," << B.lat << endl;
	cout << "Top Left(x,y): " << LocalRange.tl.x << "," << LocalRange.tl.y << endl;
	cout << "Top Right(x,y): " << LocalRange.tr.x << "," << LocalRange.tr.y << endl;
	cout << "Bottom Left(x,y): " << LocalRange.bl.x << "," << LocalRange.bl.y << endl;
	cout << "Bottom Right(x,y): " << LocalRange.br.x << "," << LocalRange.br.y << endl;
	cout << "Width(meter): " << LocalRange.width << endl;
	cout << "Height(meter): " << LocalRange.height << endl;
	cout << "/////////////////////////////////////////////////////////////////" << endl;

	fout1 << "Local Range B" << endl;
	fout1 << B.lon << ',' << B.lat << endl;
	fout1 << LocalRange.tl.x << "," << LocalRange.tl.y << endl;
	fout1 << LocalRange.tr.x << "," << LocalRange.tr.y << endl;
	fout1 << LocalRange.bl.x << "," << LocalRange.bl.y << endl;
	fout1 << LocalRange.br.x << "," << LocalRange.br.y << endl;

	clearScreen();

	LocalRange = LocalMapRange(C);

	cout << "/////////////////////////////////////////////////////////////////" << endl;
	cout << "                       Local Range C                      " << endl;
	cout << "/////////////////////////////////////////////////////////////////" << endl;
	cout << "Center Point(x,y)" << C.lon << "," << C.lat << endl;
	cout << "Top Left(x,y): " << LocalRange.tl.x << "," << LocalRange.tl.y << endl;
	cout << "Top Right(x,y): " << LocalRange.tr.x << "," << LocalRange.tr.y << endl;
	cout << "Bottom Left(x,y): " << LocalRange.bl.x << "," << LocalRange.bl.y << endl;
	cout << "Bottom Right(x,y): " << LocalRange.br.x << "," << LocalRange.br.y << endl;
	cout << "Width(meter): " << LocalRange.width << endl;
	cout << "Height(meter): " << LocalRange.height << endl;
	cout << "/////////////////////////////////////////////////////////////////" << endl;

	fout1 << "Local Range C" << endl;
	fout1 << C.lon << ',' << C.lat << endl;
	fout1 << LocalRange.tl.x << "," << LocalRange.tl.y << endl;
	fout1 << LocalRange.tr.x << "," << LocalRange.tr.y << endl;
	fout1 << LocalRange.bl.x << "," << LocalRange.bl.y << endl;
	fout1 << LocalRange.br.x << "," << LocalRange.br.y << endl;

	clearScreen();
	fout1.close();
}

void testCase3()
{
	cout << fixed;
	cout.precision(10);

	ofstream fout1, fout2;
	
	fout1 << fixed;
	fout1.precision(14);
	fout2 << fixed;
	fout2.precision(14);

	// fout1.open(".\\gps_not_smoothing.csv");
	// fout2.open(".\\gps_smoothing.csv");
	fout1.open("./gps_not_smoothing.csv");
	fout2.open("./gps_smoothing.csv");
	cout << "##############################################" << endl;
	cout << "# Test Case 3: Path calibration and Smoothing"  << endl;
	cout << "##############################################" << endl << endl;
	GPS_DD A = { 129.388734, 36.102799 }, B = { 129.388374, 36.101938 };
	vector<GPS_DD> localA2B = InterpolatePath(A, B);
	vector<GPS_DD> localPath;
	vector<GPS_DD>::iterator x;
	int count = 0;

	cout << endl;
	cout << "Way Point Size(A to B):" << localA2B.size() << endl;
	for (x = localA2B.begin(), count = 0; x != localA2B.end(); x++, count++)
	{
		cout << "WayPoint(" << count << "): " << (*x).lon << ',' << (*x).lat << endl;
		fout1 << (*x).lon << ',' << (*x).lat << endl;

		localPath.push_back(*x);
	}

	clearScreen();

	int smoothingSize;
	cout << "Smoothing Size : ";
	cin >> smoothingSize;
	cout << endl << endl << "After Smoothing" << endl;
	cout << "Way Point Size:" << localPath.size() << endl;

	SmoothingPath(&localPath, smoothingSize);

	for (x = localPath.begin(), count = 0; x != localPath.end(); x++, count++)
	{
		cout << "WayPoint(" << count << "): "<< (*x).lon << ',' << (*x).lat<< endl;
		fout2 << (*x).lon << ',' << (*x).lat << endl;
	}

	clearScreen();
	fout1.close();
	fout2.close();

}

void testCase4()
{
	cout << fixed;
	cout.precision(10);

	cout << "##############################################" << endl;
	cout << "# Test Case 4: Global Path calibration and Smoothing" << endl;
	cout << "##############################################" << endl << endl;
	ofstream  fout1, fout2;
	ifstream fin;
//	fin.open(".\\wayPoint.txt");
	fin.open("./wayPoint.txt");

	
	fin >> fixed;
	fin.precision(14);

	fout1 << fixed;
	fout1.precision(14);
	fout2 << fixed;
	fout2.precision(14);

	if (!fin.is_open())
	{

		cout << "Generate Path is Failed" << endl;
		return;
	}

	fout1.open("./generatePath.csv");
	fout2.open("./generatePath_Smoothing.csv");

	// fout1.open(".\\generatePath.csv");
	// fout2.open(".\\generatePath_Smoothing.csv");
	char *str;
	str = (char*)malloc(32);
	std::vector<GPS_DD> wayPoint;
	std::vector<GPS_DD> path;

	while (!fin.eof()){
		GPS_DD temp = { 0, 0 };
		fin.getline(str, 100, ' ');
		temp.lon = (double)atof(str);
		fin.getline(str, 100);
		temp.lat = (double)atof(str);
		if (temp.lat != 0 && temp.lon != 0)
		{
			wayPoint.push_back(temp);
		}
	}

	cout << "Waypoint Vector Size: " << wayPoint.size() << endl;

	path = GeneratePath(wayPoint);

	std::vector<GPS_DD>::iterator iter;

	cout << endl << "Path Size : " << path.size() << endl;

	for (iter = path.begin(); iter != path.end(); iter++)
	{
		fout1 << (*iter).lon << ',' << (*iter).lat << endl;
	}

	int smoothingSize;
	cout << "Smoothing Size : ";
	cin >> smoothingSize;

	SmoothingPath(&path, smoothingSize);

	cout << endl << "Smoothing Path Size : " << path.size() << endl;

	for (iter = path.begin(); iter != path.end(); iter++)
	{
		fout2 << (*iter).lon << ',' << (*iter).lat << endl;
	}

	cout << "Smoothing is Done" << endl;


	fin.close();
	fout1.close();
	fout2.close();

	getchar();
	clearScreen();
}

void testCase5()
{
	cout << "##############################################" << endl;
	cout << "# Test Case 5: Global Path Planning" << endl;
	cout << "##############################################" << endl << endl;

	std::vector<GPS_DD> globalPath;
	SelectInterface(&globalPath);
}

void testCase6()
{
	cout << "##############################################" << endl;
	cout << "# Test Case 6: Global Path Planning & Path calibration and Smoothing" << endl;
	cout << "##############################################" << endl << endl;

	std::vector<GPS_DD> wayPoint;
	SelectInterface(&wayPoint);

	cout << fixed;
	cout.precision(10);

	ofstream  fout1, fout2;

	fout1 << fixed;
	fout1.precision(14);
	fout2 << fixed;
	fout2.precision(14);

	if (wayPoint.size()<=0)
	{
		cout << "WayPoint is not existed" << endl;
		return;
	}
	fout1.open("./generatePath.csv");
	fout2.open("./generatePath_Smoothing.csv");

	// fout1.open(".\\generatePath.csv");
	// fout2.open(".\\generatePath_Smoothing.csv");
	char *str;
	str = (char*)malloc(32);

	std::vector<GPS_DD> path;


	cout << "Waypoint Vector Size: " << wayPoint.size() << endl;

	path = GeneratePath(wayPoint);

	std::vector<GPS_DD>::iterator iter;

	cout << endl << "Path Size : " << path.size() << endl;

	for (iter = path.begin(); iter != path.end(); iter++)
	{
		fout1 << (*iter).lon << ',' << (*iter).lat << endl;
	}

	int smoothingSize;
	cout << "Smoothing Size : ";
	cin >> smoothingSize;

	SmoothingPath(&path, smoothingSize);

	cout << endl << "Smoothing Path Size : " << path.size() << endl;

	for (iter = path.begin(); iter != path.end(); iter++)
	{
		fout2 << (*iter).lon << ',' << (*iter).lat << endl;
	}


	fout1.close();
	fout2.close();

	cout << "Smoothing is Done" << endl;
	double pathLength = 0;

	for (int x = 0; x < path.size(); x++)
	{
		if (x != path.size() - 1)
		{
			pathLength += GPS_DD2Dist(path[x], path[x + 1]);
		}
		cout << "Waypoint("<<x<<") "<< path[x].lon <<","<< path[x].lat << endl;
	}

	cout << endl << "Total length of Gobal path : " << pathLength << "(m)" << endl;

	getchar();
	clearScreen();

}

void testCase7()
{
	cout << "##############################################" << endl;
	cout << "# Test Case 7: Calculate Curvature, Steering angle" << endl;
	cout << "##############################################" << endl << endl;

	double angle;
	cout << "Input wanted Curvature, Steering angle : ";
	cin >> angle;
	double radius = Angle2Radius(angle);
	cout << "Curvature : " << radius << "(m)"<<endl;
	angle = Radius2Angle(radius);
	cout << "Target Angle by Curvature : " << angle  <<"(deg)"<< endl ;
	double maxSpeed;
	maxSpeed = MaxSpeed(radius);
	cout << "Target Speed by Curvature: " << maxSpeed<< "(km/h)"<<endl;

	cout << endl << "////////////////////////////////////////////" << endl << endl;

	cout << "Input Curvature to calcuate Steering angle : ";
	cin >> radius;
	angle = Radius2Angle(radius);
	cout << "Target Angle : " << angle << "(deg)" << endl;
	radius = Angle2Radius(angle);
	cout << "Curvature by Target Angle : " << radius << "(m)" << endl;
	maxSpeed = MaxSpeed(radius);
	cout << "Target Speed by Curvature: " << maxSpeed << "(km/h)" << endl;


	getchar();
	clearScreen();

}

void testCase8()
{
	cout << "###########################################################################" << endl;
	cout << "# Test Case 8: Calculate Curvature, Steering angle, Target velocity using two coordinates and Heading direction.           #" << endl;
	
	cout << "###########################################################################" << endl << endl;

	GPS_DD A,B;

	cout << "Input A coordinate" << endl;
	cout << "Latitude : ";
	cin >> A.lon;
	cout << "Longtitude : ";
	cin >> A.lat;
	cout << endl;

	cout << "Input B coordinate" << endl;
	cout << "Latitude : ";
	cin >> B.lon;
	cout << "Longtitude : ";
	cin >> B.lat;
	cout << endl;



	double heading;
	cout << "Input current direction of Heading of Car : ";
	cin >> heading;
	double maxSpeed, angle;

	cout << endl<<"A,B  Distance : " << GPS_DD2Dist(A, B) << "(m)" << endl;

	GPS_DDHead2SpeedAngle((void*)&A, (void*)&B, heading, &maxSpeed, &angle);

	getchar();
	clearScreen();

}

void testCase9()
{
	cout << "###########################################################################" << endl;
	cout << "#          Test Case 8: Heading direction and  Middle process using two coordinates      #" << endl;
	cout << "#                                                                         #" << endl;
	cout << "###########################################################################" << endl << endl;

	GPS_DD A = { 129.388855,36.102771}, B = { 129.388918, 36.102707 };
	

	double heading;
	cout << "Input Starting direction of Heading of Car : ";
	cin >> heading;
	double maxSpeed, angle;
	double distance = GPS_DD2Dist(A, B);
	cout << endl << "A,B  Distance : " << distance<< "(m)" << endl;
	GPS_DDHead2SpeedAngle((void*)&A, (void*)&B, heading, &maxSpeed, &angle);
	getchar(); 
	getchar();
	GPS_DD C,PreC;
	


	ofstream  fout1;
	fout1 << fixed;
	fout1.precision(14);
	fout1.open("./steerTest.csv");
//	fout1.open(".\\steerTest.csv");
	int x = 0;
	double distanceSum = 0;

	for (x = 0, C = A; distance > 0.1; x++)
	{
		fout1 << C.lon << ',' << C.lat << ',' << distance <<',' << distanceSum << endl;

		cout << endl << "(" << x << ")" << endl;

		PreC = C;
		C = DistDeg2GPS_DD(C, 0.1, heading);
		GPS_DDHead2SpeedAngle((void*)&C, (void*)&B, heading, &maxSpeed, &angle);
		heading = angle + heading;

		
		distance = GPS_DD2Dist(C, B);
		distanceSum += GPS_DD2Dist(C, PreC);

		cout << "Heading : " << heading << endl;
		cout << endl << "Two point  Distance : " << distance << "(m)" << endl;
		cout << endl << "Accumulated moving Distance : " << distanceSum << "(m)" << endl;
	}

	fout1.close();
	getchar();
	clearScreen();


}



void clearScreen()
{

	getchar();
	system("cls");

}


