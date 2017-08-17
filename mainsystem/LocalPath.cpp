#include<math.h>
#include"LocalPath.h"
#include"DGPS.h"


//define된 Local Map Size에 맞게 Local Map Range를 정해주는 함수
Rect_double LocalMapRange(GPS_DD center) 
{
	Rect_double LocalRange;

	GPS_DD clock45 = DistDeg2GPS_DD(center, sqrt(2*pow((double)(Local_Map_Size / 2),2)), 45);
	GPS_DD clock135 = DistDeg2GPS_DD(center, sqrt(2 * pow((double)(Local_Map_Size / 2), 2)), 135);
	GPS_DD clock225 = DistDeg2GPS_DD(center, sqrt(2 * pow((double)(Local_Map_Size / 2), 2)), 225);
	GPS_DD clock315 = DistDeg2GPS_DD(center, sqrt(2 * pow((double)(Local_Map_Size / 2), 2)), 315);


	//중심과 방위각 315도
	LocalRange.tl.x = clock315.lon;
	LocalRange.tl.y = clock315.lat;

	//중심과 방위각 45도
	LocalRange.tr.x = clock45.lon;
	LocalRange.tr.y = clock45.lat;

	//중심과 방위각 225도
	LocalRange.bl.x = clock225.lon;
	LocalRange.bl.y = clock225.lat;

	//중심과 방위각 135도
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
	
	
	if (distance <= Path_Interval) //거리가 0일경우(같은 점이 입력되었을 경우) 해당 Point 한개만을 가지는 벡터 리턴
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


// 한 Point에 대하여 전 후 smoothingSize 개수 + 자기 자신의 값의 평균을 다시 취하는 smoothing 진행
void SmoothingPath(std::vector<GPS_DD> *path,int smoothingSize) 
{
	
	if ((smoothingSize > (*path).size())||(smoothingSize<=0)) //exception handling
	{
		return;
	}

	std::vector<GPS_DD>::iterator iter;

	for (iter = (*path).begin(); iter != (*path).end(); iter++)
	{
		//양 옆으로 smoothingSize보다 좌표 개수가 적을 경우 해당 좌표는 Smoothing을 진행하지 않는다.
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


//여러개의 WayPoint를 담고 있는 벡터를 가지고 경로를 생성
std::vector<GPS_DD> GeneratePath(std::vector<GPS_DD> wayPoint)
{
	std::vector<GPS_DD> path;
	std::vector<GPS_DD>::iterator iter;


	if (wayPoint.size() <= 1) //wayPoint가 1개만 있거나 없을 경우 그대로 반환 
	{
		return wayPoint;
	}

	for (iter = wayPoint.begin(); iter != (wayPoint.end() - 1); iter++)
	{
		MergePath(&path, InterpolatePath((*iter), (*(iter + 1))));
	}
	
	return path;
}

//두 좌표가 같은 위치인지 판별 

//Path를 합쳐주는 함수
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

