
#pragma warning(disable:4996)
#pragma warning(disable:4018)


#include<fstream>
#include<stack>
#include"opencv2/opencv.hpp"
#include "ShortestPath_Interface.h"
#include"ShortestPath.h"
#include"DGPS.h"







void SelectInterface(void* GPS_Point)
{

	std::vector<GPS_DD> *realPoint;
	realPoint = (std::vector<GPS_DD>*)GPS_Point;

	std::vector<char*> selectedVertex;

	std::vector<cv::Point> selectedPoint;

	cv::Mat image = cv::imread(IMAGE_LOCATION);
	cv::resize(image, image, cv::Size(IMAGE_WIDHT, IMAGE_HEIGHT));

	std::cout << "///////////////////////////////////////////////////////" << std::endl;
	std::cout << "             Global Way Point Setting                  " << std::endl;
	std::cout <<"        Button Double Click : 설정 완료                  " << std::endl;
	std::cout << "       Mouse Left Button   : Vertex 선택             " << std::endl;
	std::cout << "       Mouse Right Button  : 직전 선택 취소          " << std::endl;
	std::cout << "                     ESC   : 취소 및 종료            " << std::endl;
	std::cout << "-------------------------------------------------------" << std::endl;
	std::cout << "------------------------Manual-------------------------" << std::endl;
	std::cout << "1. 마우스 왼쪽 버튼을 클릭하여 시작점과 종료점 우선 선택" << std::endl;
	std::cout << "2. 마우스 왼쪽 버튼을 클릭하여 경로점 선택           " << std::endl;
	std::cout << "3. 선택 완료시 더블 클릭                          " << std::endl;
	std::cout << "////////////////////////////////////////////////////" << std::endl;

	cv::imshow("Global Map",image);
	cv::setMouseCallback("Global Map", MouseSelect, (void*)&selectedPoint);

	int key = 0;

	do{

		key = cv::waitKey();
	
		if (key == ESC)
		{
			selectedPoint.clear();
			cv::destroyWindow("Global Map");
			break;
		}

	} while (key!=ESC && key != -1);//imshow가 destroy 되었을 때 key 값은 계속 -1로 오게됨, 아닐 경우 계속 대기 상태

	if (selectedPoint.size()<2)
	{
		std::cout << "Global Way Point 설정에 실패 하였습니다." << std::endl;
		selectedVertex.clear();
		(*realPoint).clear();
		return;
	}

	else
	{
		std::cout << "Global Way Point 설정에 성공 하였습니다." << std::endl;
	}

	std::cout << "Point 보정 및 Point mapping을 시작합니다." << std::endl;
	std::vector<std::pair<char*, cv::Point>> imagePoint;

	PointRevision((void*)&selectedPoint,(void*)&imagePoint); //마우스로 지정한 좌표와 ImagePoint.txt 상에 좌표를 연결 및 보정
	
	if ((imagePoint).size() == 0)
	{
		std::cout << "Point 보정 및 Point mapping에 실패했습니다." << std::endl;
		selectedVertex.clear();
		(*realPoint).clear();
		return;
	}

	for (int x = 0; x < (imagePoint).size(); x++)
	{
		selectedVertex.push_back((imagePoint)[x].first);
	}
	std::cout << "Point 보정 및 Point mapping을 완료했습니다." << std::endl;

	std::cout << "선택된 Vertex 개수는"<< selectedVertex.size() <<" 입니다. "<< std::endl;



	std::cout<<std::endl << "선택된 Vertex들을 지나는 최단 경로를 계산합니다. " << std::endl;
	FloydWarshall(&selectedVertex);
	std::cout << std::endl << "최단 경로 알고리즘에 의해 선택된 Vertex 개수는" << selectedVertex.size() << " 입니다. " << std::endl;





	if (PointMapping((void*)&selectedVertex, (void*)&imagePoint, (void*)&(*realPoint)))
	{

		std::cout << std::endl << "최단 경로 찾기 및 Point Mapping에 성공했습니다" << std::endl << "경로 지도를 출력합니다 " << std::endl;
		drawPath((void*)&imagePoint, (void*)&image); //Path를 그려주는 역할



		cv::imshow("Global Map", image);
		cv::imwrite(".\\GlobalMap.jpg", image);
		cv::waitKey();
		cv::destroyWindow("Global Map");
	}
	else
	{
		std::cout << std::endl << "최단 경로 찾기 및 Point Mapping에 실패했습니다." << std::endl<<std::endl;
		(*realPoint).clear();
	}

	std::cout<< std::endl;

}

void MouseSelect(int event, int x, int y, int flags,void* param)
{
	static std::vector<cv::Point> startEndPoint;
	std::vector<cv::Point> *selectedPoint;
	selectedPoint = (std::vector<cv::Point>*)param;

	static cv::Mat imageOrigin = cv::imread(IMAGE_LOCATION);
	static cv::Mat image = imageOrigin.clone();

	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:

		if ((startEndPoint).size()==2)
		{
			if (sqrt(DISTANCE((startEndPoint[0]), (cv::Point(x, y))))<MOUSE_DISTANCE || sqrt(DISTANCE((startEndPoint[1]), (cv::Point(x, y))))<MOUSE_DISTANCE)
			{
				return;
			}

			for (int i = 0; i < (*selectedPoint).size(); i++)
			{
			

				if (sqrt(DISTANCE(((*selectedPoint)[i]), (cv::Point(x, y))))<MOUSE_DISTANCE)
				{
					return;
				}
				
			}
		

			(*selectedPoint).push_back(cv::Point(x, y));

			}

		else
		{
			if (startEndPoint.size() == 1)
			{
				if (sqrt(DISTANCE((startEndPoint[0]), (cv::Point(x, y))))<MOUSE_DISTANCE)
				{
					return;
				}

				(startEndPoint).push_back(cv::Point(x, y));
			}

			else{
				(startEndPoint).push_back(cv::Point(x, y));
			}
		}

		break;

	case CV_EVENT_RBUTTONDOWN:
			if ((*selectedPoint).size() > 0)
			{
				(*selectedPoint).pop_back();
				image = imageOrigin.clone();
				cv::resize(image, image, cv::Size(IMAGE_WIDHT, IMAGE_HEIGHT));
				std::cout << "포인트가 제거 되었습니다" << std::endl;
			
			}
			else if (startEndPoint.size() > 0)
				{
					startEndPoint.pop_back();
					image = imageOrigin.clone();
					cv::resize(image, image, cv::Size(IMAGE_WIDHT, IMAGE_HEIGHT));
					std::cout << "포인트가 제거 되었습니다" << std::endl;
					if (startEndPoint.size() == 0)
					{
						cv::imshow("Global Map",image);
					}
				}

			else
			std::cout << "저장된 포인트가 존재하지 않습니다." << std::endl;
				
		
		break;

	case CV_EVENT_LBUTTONDBLCLK:
		std::cout << "WayPoint 설정을 종료합니다." << std::endl;
		std::vector<cv::Point>::iterator iter = (*selectedPoint).begin();

		if (startEndPoint.size() == 2){
			(*selectedPoint).insert(iter, startEndPoint[0]);
			iter = (*selectedPoint).end();
			(*selectedPoint).insert(iter, startEndPoint[1]);
			startEndPoint.clear();

			for (int x = 0; x < (*selectedPoint).size(); x++)
			{
				std::cout << (*selectedPoint)[x] << std::endl;
			}
		}
		else
		{
			startEndPoint.clear();
			(*selectedPoint).clear();
		}

		std::cout << std::endl;
		cv::destroyWindow("Global Map");
		return;
		break;

	}



	if (startEndPoint.size()>0)
	{
		cv::circle(image, (startEndPoint)[0], 5, cv::Scalar(0, 0, 255), -1); //시작점 Red

		if (startEndPoint.size() != 1)
		{
			cv::circle(image, (startEndPoint)[1], 5, cv::Scalar(255, 0, 0), -1); //도착점 Blue
			for (int x = 0; x < (*selectedPoint).size(); x++)
			{
				cv::circle(image, (*selectedPoint)[x], 5, cv::Scalar(0, 255, 0), -1); //경유점 Green

			}
		}

		cv::imshow("Global Map", image);
	}




}

void PointRevision(void *point,void *vertex)
{

	std::ifstream fin;
	fin.open(IMAGE_POINT_LOCATION);

	std::vector<std::pair<char*, cv::Point>> *selectedVertex;
	selectedVertex = (std::vector<std::pair<char*, cv::Point>>*)vertex;

	char *str;
	int buffSize = 32;

	str = (char*)malloc(buffSize); //32byte

	if (!fin.is_open())
	{
		std::cout << "Global Point Open is Failed" << std::endl;
		(*selectedVertex).clear();
		return;
	}

	//txt파일의 첫 줄은 이미지 크기
	clearBuffer(str, buffSize);
	fin.getline(str, buffSize, ' ');
	int imageWidth = atoi(str); //original Image size

	clearBuffer(str, buffSize);
	fin.getline(str, buffSize);
	int imageHeight = atoi(str); //original Image size

	double widthRatio = IMAGE_WIDHT / (double)imageWidth;
	double heightRatio = IMAGE_HEIGHT / (double)imageHeight;


	std::vector<std::pair<char*, cv::Point>> vertexPoint;

	while (!fin.eof()){
		char *tempStr;
		cv::Point tempPoint;
		tempStr = (char*)malloc(buffSize);//4byte

		clearBuffer(str, buffSize);
		fin.getline(str, buffSize, ' ');
		strcpy(tempStr, str);

		clearBuffer(str, buffSize);
		fin.getline(str, buffSize, ' ');
		tempPoint.x = (int)(atoi(str)*widthRatio);

		clearBuffer(str, buffSize);
		fin.getline(str, buffSize);
		tempPoint.y = (int)(atoi(str)*heightRatio);


		if (tempStr != NULL&&tempPoint.x > 0 && tempPoint.y > 0) //0과 Width index는 exception handling에 사용
		{
			if (tempPoint.x < IMAGE_WIDHT&&tempPoint.y < IMAGE_HEIGHT)
			{
				if (vertexPoint.size() == 0)
				{
					vertexPoint.push_back(std::make_pair(tempStr, tempPoint));
				}
				int count = 0;
				for (int x = 0; x < vertexPoint.size(); x++) //중복 좌표 허용 X
				{
					if ((vertexPoint[x].second.x == tempPoint.x) && (vertexPoint[x].second.y == tempPoint.y) &&
						(strcmp(tempStr, vertexPoint[x].first) == 0))//같은게 있으면 break;
					{
						break;
					}
					count++;
				}

				if (count == vertexPoint.size())
				{
					vertexPoint.push_back(std::make_pair(tempStr, tempPoint));
				}

			}

		}
	}

	fin.close();

	std::vector<cv::Point> *selectedPoint;
	selectedPoint = (std::vector<cv::Point>*)point;

	

	for (int x = 0; x < (*selectedPoint).size(); x++)
	{
		int count = 0;

		for (int y = 0; y < vertexPoint.size(); y++)
		{
			count++;
			if (DISTANCE(((*selectedPoint)[x]), (vertexPoint[y].second)) <POINT_DISTANCE)
			{
				
				(*selectedVertex).push_back(vertexPoint[y]);
				break;
			}
		}

		if ((x == 0) || (x == (*selectedPoint).size() - 1))
			if (count == vertexPoint.size())
			{
				std::cout << "출발점과 도착점이 올바르게 설정되지 않았습니다." << std::endl;
				(*selectedVertex).clear();
				return;
			}	
	}
}




void clearBuffer(char *str, int bufferSize)
{

	for (int x = 0; x < bufferSize; x++)
	{
		str[x] = 0;
	}
}


void drawPath(void *wayPoint, void*image)
{

	std::vector<std::pair<char*, cv::Point>> *selectedVertex;
	selectedVertex = (std::vector<std::pair<char*, cv::Point>>*)wayPoint;





	for (int x = 0; x < (*selectedVertex).size(); x++)
	{
		if (x == 0)
		{
			std::cout << "시작점 ";
		}
		else if (x == (*selectedVertex).size() - 1)
		{
			std::cout << "도착점 ";
		}
		else
		{
			std::cout << "경로점 ";
		}

		std::cout << "(" << x << ")    " << (*selectedVertex)[x].second << std::endl;

	}

	cv::Mat *pathImage = (cv::Mat *)image;




	for (int x = 0; x < (*selectedVertex).size()-1; x++)
	{
		
		cv::line((*pathImage), (*selectedVertex)[x].second, (*selectedVertex)[x+1].second, cv::Scalar(0, 0, 0), 3);
	}

	for (int x = 0; x < (*selectedVertex).size(); x++)
	{
		if (x == 0)
		{
			cv::circle((*pathImage), (*selectedVertex)[x].second, 7, cv::Scalar(0, 0, 255), -1);
		}
		else if (x == (*selectedVertex).size() - 1)
		{
			cv::circle((*pathImage), (*selectedVertex)[x].second, 7, cv::Scalar(255, 0, 0), -1);
		}
		else
			cv::circle((*pathImage), (*selectedVertex)[x].second, 7, cv::Scalar(0, 255, 0), -1);
	}

}




int PointMapping(void *vertex, void *imagePoint, void *realPoint)
{

	std::vector<char*> *Vertex;
	Vertex = (std::vector<char*>*)vertex;

	std::vector<std::pair<char*, cv::Point>> *imageVertex;
	
	imageVertex = (std::vector<std::pair<char*, cv::Point>>*)imagePoint;
	(*imageVertex).clear();

	std::vector<GPS_DD> *realVertex;
	realVertex = (std::vector<GPS_DD>*)realPoint;



	std::ifstream finImage;
	finImage.open(IMAGE_POINT_LOCATION);
	std::ifstream finReal;
	finReal.open(GLOBAL_PATH_LOCATION);


	char *str;
	int buffSize = 32;

	str = (char*)malloc(buffSize); //32byte


	if (!finImage.is_open())
	{
		std::cout << "Image Point Open is Failed" << std::endl;
		(*realVertex).clear();
		(*imageVertex).clear();
		return false;
	}

	if (!finReal.is_open())
	{
		std::cout << "Real Point Open is Failed" << std::endl;
		(*realVertex).clear();
		(*imageVertex).clear();
		return false;
	}

	//Image txt파일의 첫 줄은 이미지 크기
	clearBuffer(str, buffSize);
	finImage.getline(str, buffSize, ' ');
	int imageWidth = atoi(str); //original Image size

	clearBuffer(str, buffSize);
	finImage.getline(str, buffSize);
	int imageHeight = atoi(str); //original Image size

	double widthRatio = IMAGE_WIDHT / (double)imageWidth;
	double heightRatio = IMAGE_HEIGHT / (double)imageHeight;


	for (int x = 0; x < (*Vertex).size(); x++)
	{
		finImage.seekg(0, std::ios::beg);
		finImage.getline(str, buffSize); //첫줄 무시

		finReal.seekg(0, std::ios::beg);

		while (!finImage.eof()){
			char *tempStr;
			cv::Point tempPoint;
			tempStr = (char*)malloc(buffSize);//4byte
			clearBuffer(str, buffSize);
			finImage.getline(str, buffSize, ' ');


			if (strcmp((*Vertex)[x], str) == 0)
			{
				strcpy(tempStr, str);

				clearBuffer(str, buffSize);
				finImage.getline(str, buffSize, ' ');
				tempPoint.x = (int)(atoi(str)*widthRatio);

				clearBuffer(str, buffSize);
				finImage.getline(str, buffSize);
				tempPoint.y = (int)(atoi(str)*heightRatio);

				if (tempStr != NULL&&tempPoint.x > 0 && tempPoint.y > 0) //0과 Width index는 exception handling에 사용
				{
					if (tempPoint.x < IMAGE_WIDHT&&tempPoint.y < IMAGE_HEIGHT)
					{
						(*imageVertex).push_back(std::make_pair(tempStr, tempPoint));
						break;
					}
				}

			}

			else{
				free(tempStr);
				clearBuffer(str, buffSize);
				finImage.getline(str, buffSize, ' ');
				clearBuffer(str, buffSize);
				finImage.getline(str, buffSize);
			}
		}

	
		while (!finReal.eof())
		{
			GPS_DD temp = { 0, 0 };

			clearBuffer(str, buffSize);
			finReal.getline(str, buffSize, ' ');
			if (strcmp((*Vertex)[x], str) == 0)
			{
				finReal.getline(str, buffSize, ' ');
				temp.lon = (double)atof(str);
				finReal.getline(str, buffSize);
				temp.lat = (double)atof(str);
				if (temp.lat != 0 && temp.lon != 0)
				{
					(*realVertex).push_back(temp);
					break;
				}
			}

			else{
				clearBuffer(str, buffSize);
				finReal.getline(str, buffSize, ' ');
				clearBuffer(str, buffSize);
				finReal.getline(str, buffSize);
			}
		}
	}


	free(str);

	if ((*realVertex).size() != (*imageVertex).size())
	{
		std::cout << "Mapping에 실패하였습니다." << std::endl;
		return false;
	}
	
	return true;


}






