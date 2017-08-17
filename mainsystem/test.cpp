#include<fstream>
#include<vector>
#include <iostream>
#include "DGPS.h"
#include "LocalPath.h"
#include"test.h"
#include"ShortestPath_Interface.h"
#include"TransverseControl.h"

using namespace std;


void testCase1() //거리, 방위각 계산 및 역계산
{
	cout << fixed;
	cout.precision(10);
	cout << "##############################################" << endl;
	cout << "# Test Case 1: 거리, 방위각 계산 및 역계산" << endl;
	cout << "##############################################" << endl<< endl;
	GPS_DD A = { 129.388734, 36.102799 }, B = { 129.388374, 36.101938}, C;
	cout << "A 포인트 경도: " << A.lon << endl;
	cout << "A 포인트 위도: " << A.lat << endl;
	cout << "B 포인트 경도: " << B.lon << endl;
	cout << "B 포인트 위도: " << B.lat << endl;
	
	double degree, distance;


	cout << endl << "/////////////////////////////////////////";

	distance = GPS_DD2Dist(A, B);
	degree = GPS_DD2Deg(A, B);
	cout << endl << "A와 B 사이 거리 : " << distance << "m" << endl;
	cout << "A와 B 사이 방위각 : " << degree << "(degree)" << endl;
	cout << "/////////////////////////////////////////" << endl << endl;

	C = DistDeg2GPS_DD(A, distance, degree);

	cout << "C 포인트 경도: " << C.lon << endl;
	cout << "C 포인트 위도: " << C.lat << endl;
	


	distance = GPS_DD2Dist(A, C);
	degree = GPS_DD2Deg(A, C);

	cout << endl << "/////////////////////////////////////////";
	cout << endl << "A와 C 사이 거리 : " << distance << "m" << endl;
	cout << "A와 C 사이 방위각 : " << degree << "(degree)" << endl;
	cout << "/////////////////////////////////////////" << endl << endl;

	clearScreen();



}

void testCase2() //Local Map Range Setting
{
	ofstream fout1;
	//저장해줄때 precision 자리수 Setting 해줘야 한다 안해줄 경우 정수 포함 6자리만 유효
	fout1 << fixed;
	fout1.precision(14);

	fout1.open(".\\LocalRange.csv");



	cout << fixed;
	cout.precision(10);
	cout << "##############################################" << endl;
	cout << "# Test Case 2: 로컬맵 영역 설정" << endl;
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

void testCase3() //경로 보간과 스무딩
{
	cout << fixed;
	cout.precision(10);

	ofstream fout1, fout2;
	//저장해줄때 precision 자리수 Setting 해줘야 한다 안해줄 경우 정수 포함 6자리만 유효
	fout1 << fixed;
	fout1.precision(14);
	fout2 << fixed;
	fout2.precision(14);

	fout1.open(".\\gps_not_smoothing.csv");
	fout2.open(".\\gps_smoothing.csv");
	cout << "##############################################" << endl;
	cout << "# Test Case 3: 경로 보간 및 스무딩"  << endl;
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
	cout << "# Test Case 4: 전체 경로 생성(보간) 및 스무딩" << endl;
	cout << "##############################################" << endl << endl;
	ofstream  fout1, fout2;
	ifstream fin;
	fin.open(".\\wayPoint.txt");

	//저장해줄때 precision 자리수 Setting 해줘야 한다 안해줄 경우 정수 포함 6자리만 유효
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

	fout1.open(".\\generatePath.csv");
	fout2.open(".\\generatePath_Smoothing.csv");
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
	cout << "# Test Case 6: Global Path Planning & 경로 보간 및 스무딩" << endl;
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

	fout1.open(".\\generatePath.csv");
	fout2.open(".\\generatePath_Smoothing.csv");
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
		cout << "경로점("<<x<<") "<< path[x].lon <<","<< path[x].lat << endl;
	}

	cout << endl << "전체 경로 길이 : " << pathLength << "(m)" << endl;

	getchar();
	clearScreen();

}

void testCase7()
{
	cout << "##############################################" << endl;
	cout << "# Test Case 7: 곡률 반경 계산, 조향각 계산" << endl;
	cout << "##############################################" << endl << endl;

	double angle;
	cout << "곡률 반경을 계산할 조향각을 입력해주세요 : ";
	cin >> angle;
	double radius = Angle2Radius(angle);
	cout << "곡률 반경 : " << radius << "(m)"<<endl;
	angle = Radius2Angle(radius);
	cout << "곡률 반경에 의한 Target Angle: " << angle  <<"(deg)"<< endl ;
	double maxSpeed;
	maxSpeed = MaxSpeed(radius);
	cout << "곡률반경에 의한 Target Speed : " << maxSpeed<< "(km/h)"<<endl;

	cout << endl << "////////////////////////////////////////////" << endl << endl;

	cout << "조향각을 계산할 곡률 반경을 입력해주세요 : ";
	cin >> radius;
	angle = Radius2Angle(radius);
	cout << "Target Angle : " << angle << "(deg)" << endl;
	radius = Angle2Radius(angle);
	cout << "Target Angle에 의한 곡률반경 : " << radius << "(m)" << endl;
	maxSpeed = MaxSpeed(radius);
	cout << "곡률반경에 의한 Target Speed : " << maxSpeed << "(km/h)" << endl;


	/*
	GPS_DD A = { 129.388734, 36.102799 }, B = { 129.388734, 36.101938 };
	double heading = 90;

	GPS_DDHead2SpeedAngle((void*)&A, (void*)&B, heading, &maxSpeed, &angle);

	cout << endl << "A에서 B로 가기 위한 목표 속도와 조향각 " << endl;
	cout << "Max Speed : " << maxSpeed << endl;
	cout << "Steering Angle : " << angle << endl;
	*/
	getchar();
	clearScreen();

}

void testCase8()
{
	cout << "###########################################################################" << endl;
	cout << "# Test Case 8: Heading 방향과 두 점 좌표를 통한 곡률 반경 계산,           #" << endl;
	cout << "#               조향각 계산, 목표 속도 계산                               #" << endl;
	cout << "###########################################################################" << endl << endl;

	GPS_DD A,B;

	cout << "A 지점의 좌표를 입력해주세요" << endl;
	cout << "위도 : ";
	cin >> A.lon;
	cout << "경도 : ";
	cin >> A.lat;
	cout << endl;

	cout << "B 지점의 좌표를 입력해주세요" << endl;
	cout << "위도 : ";
	cin >> B.lon;
	cout << "경도 : ";
	cin >> B.lat;
	cout << endl;



	double heading;
	cout << "차량의 현재 Heading 방향을 입력해주세요 : ";
	cin >> heading;
	double maxSpeed, angle;

	cout << endl<<"두 점 A,B 사이 거리 : " << GPS_DD2Dist(A, B) << "(m)" << endl;

	GPS_DDHead2SpeedAngle((void*)&A, (void*)&B, heading, &maxSpeed, &angle);

	getchar();
	clearScreen();

}

void testCase9()
{
	cout << "###########################################################################" << endl;
	cout << "#          Test Case 8: Heading 방향과 두 점 좌표를 이용한 중간 과정      #" << endl;
	cout << "#                                                                         #" << endl;
	cout << "###########################################################################" << endl << endl;

	GPS_DD A = { 129.388855,36.102771}, B = { 129.388918, 36.102707 };
	/*
	cout << "A 지점의 좌표를 입력해주세요" << endl;
	cout << "위도 : ";
	cin >> A.lon;
	cout << "경도 : ";
	cin >> A.lat;
	cout << endl;

	cout << "B 지점의 좌표를 입력해주세요" << endl;
	cout << "위도 : ";
	cin >> B.lon;
	cout << "경도 : ";
	cin >> B.lat;
	cout << endl;
	*/


	double heading;
	cout << "차량의 시작 Heading 방향을 입력해주세요 : ";
	cin >> heading;
	double maxSpeed, angle;
	double distance = GPS_DD2Dist(A, B);
	cout << endl << "두 점 A,B 사이 거리 : " << distance<< "(m)" << endl;
	GPS_DDHead2SpeedAngle((void*)&A, (void*)&B, heading, &maxSpeed, &angle);
	getchar(); 
	getchar();
	GPS_DD C,PreC;
	


	ofstream  fout1;
	fout1 << fixed;
	fout1.precision(14);
	fout1.open(".\\steerTest.csv");
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
		cout << endl << "두 점 사이 거리 : " << distance << "(m)" << endl;
		cout << endl << "누적 이동 거리 : " << distanceSum << "(m)" << endl;
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


