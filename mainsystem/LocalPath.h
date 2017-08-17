
#include<vector>
#include "DGPS.h"
#ifndef Local_Path
#define Local_path

#define Local_Map_Size 40 //meter
#define Path_Interval 0.5 //meter

typedef struct Point_double
{
	double x;
	double y;

}Point_double;



typedef struct Rect_double{

	Point_double tl;
	Point_double tr;
	Point_double bl;
	Point_double br;

	double width;
	double height;
	

}Rect_double;

Rect_double LocalMapRange(GPS_DD center);

std::vector<GPS_DD> GeneratePath(std::vector<GPS_DD> wayPoint);

void MergePath(std::vector<GPS_DD> *Dest, std::vector<GPS_DD> Source);

std::vector<GPS_DD> InterpolatePath(GPS_DD start, GPS_DD end);

void SmoothingPath(std::vector<GPS_DD> *path,int smoothingSize);


#endif