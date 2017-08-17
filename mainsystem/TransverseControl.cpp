#include<iostream>
#include<math.h>
#include"CarData.h"
#include"DGPS.h"



double Angle2Radius(double angle) //���Ⱒ�� ���� �ּ� ��� �ݰ� ���
{
	double radiusCurvature;  //������ �ּҰ���ݰ� �̻��� Ȯ���Ǿ�� ���ϴ� ������ ���� �����ϴ�
	angle = angle*Deg2Rad_Multi;
	radiusCurvature = (WheelBase / (tan(angle)));
	radiusCurvature = fabs(radiusCurvature);

	return (radiusCurvature / 1000); //mm to meter
}

//Calculate Min Max Steering Angle
double Radius2Angle(double radiusCurvature) //�־��� ��� �ݰ濡 ���� ���Ⱒ ���
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


//��� �ݰ�� ��������� ���� ���� �ӵ�, ��� ���� �ӵ� ��
double MaxSpeed(double radiusCurvature)
{
	//���ɷ¿� ���� ���� �̲��� ���� �ʴ� ���¿��� �ִ� �ӷ� ���
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

	double distanceA2B = GPS_DD2Dist(*A, *B); //�� �� ���� �Ÿ�
	double degreeAB = GPS_DD2Deg(*A, *B); //�� �� ���� ������

	double radiusCurvature;
	double theta;

	if (degreeAB > headingAngle)
	{
		theta = degreeAB - headingAngle; //�� �� ���� ��� ���� Theta��
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
	
void GPS_DDHead2SpeedAngle(void* start, void* end, double headingAngle,double *targetSpeed, double *targetAngle) //heading Angle�� ���ϱ��� Angle
{
	GPS_DD *A;
	A = (GPS_DD*)start;
	GPS_DD *B;
	B = (GPS_DD*)end;

	double radiusCurvature = GPS_DDHead2Radius((void*)&(*A),(void*)&(*B),headingAngle);
	double degreeAB = GPS_DD2Deg(*A, *B); //�� �� ���� ������

	*targetSpeed = MaxSpeed(radiusCurvature);
	*targetAngle = Radius2Angle(radiusCurvature);

	*targetAngle = *targetAngle > SteeringAngle_Speed ? SteeringAngle_Speed : *targetAngle;

	if (degreeAB>headingAngle) //��� �������� ����� : ���Ⱒ +
	{
		*targetAngle = *targetAngle;
	}
	else if(headingAngle>degreeAB) //��� �������� �¹��� : ���Ⱒ -
	{
		*targetAngle = -(*targetAngle);
	}
	else//����� ���Ⱒ�� ��ġ
	{
		*targetAngle = 0;
	}

	std::cout << std::endl << "�� �� ���� ��� �ݰ� : " << radiusCurvature << "(m)" << std::endl;
	std::cout << std::endl << "�� �� ���� ���� �ӵ� : " << *targetSpeed << "(km/h)" << std::endl;
	std::cout << std::endl << "�ٸ� �� ������ �� �� ���Ⱒ : " << *targetAngle << "(deg)" << std::endl;

}

