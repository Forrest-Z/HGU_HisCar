


#ifndef Transverse_Control
#define Transverse_Control

#define FAILED -1
#define SUCCESS 1



double Angle2Radius(double);
double Radius2Angle(double);
double MaxSpeed(double radiusCurvature);
double GPS_DDHead2Radius(const void*start, const void*end, double headingAngle);
void GPS_DDHead2SpeedAngle(void* start, void* end, double headingAngle, double *targetSpeed, double *targetAngle);
#endif


