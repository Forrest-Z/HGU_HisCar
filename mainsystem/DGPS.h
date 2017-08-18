
#ifndef _GPS_UNIT
#define _GPS_UNIT

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define Deg2Rad_Multi PI/180
#define Rad2Deg_Multi 180/PI

#define Deviation_Angle 8.14
/*WGS84*/
#define Earth_R 6378.137 //earth mean radius defined by WGS84

/*GRS80*/
#define Minor_Axis 6356752.314140910
#define Major_Axis 6378137.000000000
#define Ellipicty 0.0033528107




typedef struct GPS_DD{
	double lon;//Longitude Longtitude
	double lat;//Latitude Latitude
}GPS_DD;

typedef struct DMS{
	int degree;
	int min;
	double sec;
}DMS;


typedef struct GPS_DMS{
	DMS lat;
	DMS lon;
}GPS_DMS;


GPS_DMS GPS_DD2DMS(GPS_DD input);
GPS_DD GPS_DMS2DD(GPS_DMS input);
GPS_DMS GPS_DMS_Diff(GPS_DMS A, GPS_DMS B);

double GPS_DD2Dist(GPS_DD A, GPS_DD B);
double GPS_DD2Deg(GPS_DD A, GPS_DD B);
//void GPS_DD2DistDeg(GPS_DD DD_A, GPS_DD DD_B, double *distance, double *degree);

GPS_DD DistDeg2GPS_DD(GPS_DD DD_A, double distance, double degree);

bool IsSamePoint(GPS_DD DD_A, GPS_DD DD_B);
#endif

