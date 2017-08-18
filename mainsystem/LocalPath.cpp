#include<math.h>
#include"LocalPath.h"
#include"DGPS.h"


Rect_double LocalMapRange(GPS_DD center) 
{
	Rect_double LocalRange;

	GPS_DD clock45 = DistDeg2GPS_DD(center, sqrt(2*pow((double)(Local_Map_Size / 2),2)), 45);
	GPS_DD clock135 = DistDeg2GPS_DD(center, sqrt(2 * pow((double)(Local_Map_Size / 2), 2)), 135);
	GPS_DD clock225 = DistDeg2GPS_DD(center, sqrt(2 * pow((double)(Local_Map_Size / 2), 2)), 225);
	GPS_DD clock315 = DistDeg2GPS_DD(center, sqrt(2 * pow((double)(Local_Map_Size / 2), 2)), 315);


	
	LocalRange.tl.x = clock315.lon;
	LocalRange.tl.y = clock315.lat;

	
	LocalRange.tr.x = clock45.lon;
	LocalRange.tr.y = clock45.lat;

	
	LocalRange.bl.x = clock225.lon;
	LocalRange.bl.y = clock225.lat;

	
	LocalRange.br.x = clock135.lon;
	LocalRange.br.y = clock135.lat;


	LocalRange.width = GPS_DD2Dist(clock315, clock45);
	LocalRange.height = GPS_DD2Dist(clock315, clock225);

	return LocalRange;

}


std::vector<GPS_DD> InterpolatePath(GPS_DD start, GPS_DD end)
{
	std::vector<GPS_DD> interpolatedPath;
	interpolatedPath.clear();


	double distance = GPS_DD2Dist(start, end);
	
	
	if (distance <= Path_Interval) 
	{
		if (distance == 0)
		{
			interpolatedPath.push_back(start);
		}
		else
		{
			interpolatedPath.push_back(start);
			interpolatedPath.push_back(end);
		}
		return interpolatedPath;
	}

	int wayPointNums = (int)(distance / Path_Interval);
		
	GPS_DD incrementValue;
	
	incrementValue.lon = (end.lon - start.lon) / wayPointNums;
	incrementValue.lat = (end.lat - start.lat) / wayPointNums;



	for (int x = 0; x <= wayPointNums; x++)
	{
		double incrementLon = incrementValue.lon*x;
		double incrementLat = incrementValue.lat*x;
		GPS_DD temp = { start.lon + incrementLon, start.lat + incrementLat };
		interpolatedPath.push_back(temp);
	}

	return interpolatedPath;

}



void SmoothingPath(std::vector<GPS_DD> *path,int smoothingSize) 
{
	
	if ((smoothingSize > (*path).size())||(smoothingSize<=0)) 
	{
		return;
	}

	std::vector<GPS_DD>::iterator iter;

	for (iter = (*path).begin(); iter != (*path).end(); iter++)
	{
	
		if ((iter >= ((*path).begin() + smoothingSize)) && (iter <= ((*path).end() - smoothingSize - 1))) 
		{
			int count = 0;
			GPS_DD temp = { 0, 0 };

			std::vector<GPS_DD>::iterator itr;

			for (itr = iter - smoothingSize, count = 0; count < (smoothingSize * 2) + 1; itr++, count++)
			{
				temp.lon += (*itr).lon;
				temp.lat += (*itr).lat;
				
			}

			(*iter).lon = temp.lon / ((smoothingSize * 2) + 1);
			(*iter).lat = temp.lat/((smoothingSize*2)+1);
		
		}
		
	}
	
}



std::vector<GPS_DD> GeneratePath(std::vector<GPS_DD> wayPoint)
{
	std::vector<GPS_DD> path;
	std::vector<GPS_DD>::iterator iter;


	if (wayPoint.size() <= 1) 
	{
		return wayPoint;
	}

	for (iter = wayPoint.begin(); iter != (wayPoint.end() - 1); iter++)
	{
		MergePath(&path, InterpolatePath((*iter), (*(iter + 1))));
	}
	
	return path;
}

void MergePath(std::vector<GPS_DD> *Dest, std::vector<GPS_DD> Source)
{
	std::vector<GPS_DD>::iterator iter = Source.begin();


	if ((*Dest).size() > 0){ //exception handling 
		if (IsSamePoint(*((*Dest).end() - 1), *(Source.begin())))
		{
			iter = Source.begin() + 1;
		}
	}

	for (; iter != Source.end(); iter++)
	{
		(*Dest).push_back((*iter));
	}

}

