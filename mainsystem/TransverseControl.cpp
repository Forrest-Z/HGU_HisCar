#include<iostream>
#include<math.h>
#include"CarData.h"
#include"DGPS.h"



double Angle2Radius(double angle) //조향각에 따른 최소 곡률 반경 계산
{
	double radiusCurvature;  //차량은 최소곡률반경 이상이 확보되어야 원하는 각으로 조향 가능하다
	angle = angle*Deg2Rad_Multi;
	radiusCurvature = (WheelBase / (tan(angle)));
	radiusCurvature = fabs(radiusCurvature);

	return (radiusCurvature / 1000); //mm to meter
}

//Calculate Min Max Steering Angle
double Radius2Angle(double radiusCurvature) //주어진 곡률 반경에 대한 조향각 계산
{
	double angle;

	if (isinf(radiusCurvature))
	{
		angle = 0;
		return angle;
	}

	radiusCurvature = fabs(radiusCurvature);
	
	radiusCurvature = (radiusCurvature * 1000); //meter to mm
	angle = atan(radiusCurvature / WheelBase);
	
	angle = 90 - angle*Rad2Deg_Multi;
	
	return angle;
	
}


//곡률 반경과 마찰계수에 의한 제한 속도, 경로 제한 속도 비교
double MaxSpeed(double radiusCurvature)
{
	//구심력에 의해 차가 미끄러 지지 않는 상태에서 최대 속력 계산
	radiusCurvature = fabs(radiusCurvature);
	double maxSpeed = sqrt((CoefficientOfFriction*GravitationalACC*radiusCurvature)); //m/s
	maxSpeed = (maxSpeed*3600)/1000; //m/s to km/h
	std::cout << "Not Limited Speed : " << maxSpeed << std::endl;
	maxSpeed = maxSpeed <= SpeedLimit ? maxSpeed : SpeedLimit;
	
	return maxSpeed;
}



double GPS_DDHead2Radius(const void*start, const void*end, double headingAngle)
{
	GPS_DD *A;
	A = (GPS_DD*)start;
	GPS_DD *B;
	B = (GPS_DD*)end;

	double distanceA2B = GPS_DD2Dist(*A, *B); //두 점 사이 거리
	double degreeAB = GPS_DD2Deg(*A, *B); //두 점 사이 방위각

	double radiusCurvature;
	double theta;

	if (degreeAB > headingAngle)
	{
		theta = degreeAB - headingAngle; //두 점 사이 헤딩 기준 Theta각
	}
	else
	{
		theta = headingAngle-degreeAB;
	}

	theta = theta*Deg2Rad_Multi;
	radiusCurvature = (distanceA2B / (2 * cos(theta)*tan(theta)));
	radiusCurvature = fabs(radiusCurvature);
	return radiusCurvature;
}
	
void GPS_DDHead2SpeedAngle(void* start, void* end, double headingAngle,double *targetSpeed, double *targetAngle) //heading Angle은 진북기준 Angle
{
	GPS_DD *A;
	A = (GPS_DD*)start;
	GPS_DD *B;
	B = (GPS_DD*)end;

	double radiusCurvature = GPS_DDHead2Radius((void*)&(*A),(void*)&(*B),headingAngle);
	double degreeAB = GPS_DD2Deg(*A, *B); //두 점 사이 방위각

	*targetSpeed = MaxSpeed(radiusCurvature);
	*targetAngle = Radius2Angle(radiusCurvature);

	*targetAngle = *targetAngle > SteeringAngle_Speed ? SteeringAngle_Speed : *targetAngle;

	if (degreeAB>headingAngle) //헤딩 기준으로 우방향 : 조향각 +
	{
		*targetAngle = *targetAngle;
	}
	else if(headingAngle>degreeAB) //헤딩 기준으로 좌방향 : 조향각 -
	{
		*targetAngle = -(*targetAngle);
	}
	else//헤딩과 조향각이 일치
	{
		*targetAngle = 0;
	}

	std::cout << std::endl << "두 점 사이 곡률 반경 : " << radiusCurvature << "(m)" << std::endl;
	std::cout << std::endl << "두 점 사이 제한 속도 : " << *targetSpeed << "(km/h)" << std::endl;
	std::cout << std::endl << "다른 한 점으로 갈 때 조향각 : " << *targetAngle << "(deg)" << std::endl;

}

