
#include<math.h>
#include<float.h>
#include"DGPS.h"


GPS_DMS GPS_DD2DMS(GPS_DD input)
{
	GPS_DMS output;

	output.lon.degree = (int)input.lon;
	output.lon.min = (int)((input.lon - output.lon.degree) * 60);
	output.lon.sec = (((input.lon - output.lon.degree) * 60) - output.lon.min) * 60;

	output.lat.degree = (int)input.lat;
	output.lat.min = (int)((input.lat - output.lat.degree) * 60);
	output.lat.sec = (((input.lat - output.lat.degree) * 60) - output.lat.min) * 60;

	return output;
}

GPS_DD GPS_DMS2DD(GPS_DMS input)
{
	GPS_DD output;

	output.lon = input.lon.degree + (input.lon.min / 60) + (input.lon.sec / 3600);
	output.lat = input.lat.degree + (input.lat.min / 60) + (input.lat.sec / 3600);

	return output;
}

GPS_DMS GPS_DMS_Diff(GPS_DMS A, GPS_DMS B)
{
	GPS_DMS diff;

	diff.lon.degree = A.lon.degree - B.lon.degree;
	diff.lon.min = A.lon.min - B.lon.min;
	diff.lon.sec = A.lon.sec - B.lon.sec;

	diff.lat.degree = A.lat.degree - B.lat.degree;
	diff.lat.min = A.lat.min - B.lat.min;
	diff.lat.sec = A.lat.sec - B.lat.sec;

	return diff;
}


//distance(meter),degree(degree)
double GPS_DD2Dist(GPS_DD DD_A, GPS_DD DD_B) {

	if (IsSamePoint(DD_A,DD_B))
	{
		return 0;
	}

	double Cur_Lon_radian = DD_A.lon * Deg2Rad_Multi;
	double Cur_Lat_radian = DD_A.lat * Deg2Rad_Multi;
	
	double Dest_Lon_radian = DD_B.lon * Deg2Rad_Multi;
	double Dest_Lat_radian = DD_B.lat * Deg2Rad_Multi;
	

	double diff_lon = Dest_Lon_radian - Cur_Lon_radian;
	double distance = acos(sin(Cur_Lat_radian)*sin(Dest_Lat_radian) + cos(Cur_Lat_radian)
		*cos(Dest_Lat_radian)*cos(diff_lon))*Earth_R;

	return distance * 1000;
}

double GPS_DD2Deg(GPS_DD DD_A, GPS_DD DD_B) {
   // 두 점 사이 방위각(북극점 기준,진북)

   double Cur_Lon_radian = DD_A.lon * Deg2Rad_Multi;
   double Cur_Lat_radian = DD_A.lat * Deg2Rad_Multi;

   double Dest_Lon_radian = DD_B.lon * Deg2Rad_Multi;
   double Dest_Lat_radian = DD_B.lat * Deg2Rad_Multi;


   double diff_lon = Dest_Lon_radian - Cur_Lon_radian;

   double x = (cos(Cur_Lat_radian)*sin(Dest_Lat_radian)) - (sin(Cur_Lat_radian)*cos(Dest_Lat_radian)*cos(diff_lon));
   double y = sin(diff_lon)*cos(Dest_Lat_radian);
   
   double degree = atan2(y, x)*Rad2Deg_Multi;

   return (degree >= 0) ? degree : (degree + 360);
   
}



GPS_DD DistDeg2GPS_DD(GPS_DD DD_A, double distance, double degree)
{
	if (distance == 0)
	{
		return DD_A;
	}

	GPS_DD DD_B = { 0, 0 };

	DD_A.lon = DD_A.lon*Deg2Rad_Multi;
	DD_A.lat = DD_A.lat*Deg2Rad_Multi;


	degree = degree*Deg2Rad_Multi;

	distance = distance / 1000; //meter to km


	DD_B.lat = asin(sin(DD_A.lat)*cos(distance / Earth_R) +
		cos(DD_A.lat)*sin(distance / Earth_R)*cos(degree));

	DD_B.lon = DD_A.lon + atan2(sin(degree)*sin(distance / Earth_R)*cos(DD_A.lat),
		cos(distance / Earth_R) - sin(DD_A.lat)*sin(DD_B.lat));

	DD_B.lon = DD_B.lon*Rad2Deg_Multi;
	DD_B.lat = DD_B.lat*Rad2Deg_Multi;


	return DD_B;

}


double DegreeMag2True(double degree)
{
	degree = degree - Deviation_Angle;

	if (degree<0)
	{
		degree = 360 + degree;
	}
	return degree;
}

double DegreeTrue2Mag(double degree)
{

	degree = degree + Deviation_Angle;

	if (degree >= 360)
	{
		degree = degree - 360;
	}

	return degree;
}



bool IsSamePoint(GPS_DD DD_A, GPS_DD DD_B)
{
	if ((DD_A.lon == DD_B.lon)&&(DD_A.lat == DD_B.lat))
	{
		return true;
	}
	else
	{
		return false;
	}
}

