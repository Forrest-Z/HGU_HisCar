
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
	std::cout <<"        Button Double Click : ���� �Ϸ�                  " << std::endl;
	std::cout << "       Mouse Left Button   : Vertex ����             " << std::endl;
	std::cout << "       Mouse Right Button  : ���� ���� ���          " << std::endl;
	std::cout << "                     ESC   : ��� �� ����            " << std::endl;
	std::cout << "-------------------------------------------------------" << std::endl;
	std::cout << "------------------------Manual-------------------------" << std::endl;
	std::cout << "1. ���콺 ���� ��ư�� Ŭ���Ͽ� �������� ������ �켱 ����" << std::endl;
	std::cout << "2. ���콺 ���� ��ư�� Ŭ���Ͽ� ����� ����           " << std::endl;
	std::cout << "3. ���� �Ϸ�� ���� Ŭ��                          " << std::endl;
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

	} while (key!=ESC && key != -1);//imshow�� destroy �Ǿ��� �� key ���� ��� -1�� ���Ե�, �ƴ� ��� ��� ��� ����

	if (selectedPoint.size()<2)
	{
		std::cout << "Global Way Point ������ ���� �Ͽ����ϴ�." << std::endl;
		selectedVertex.clear();
		(*realPoint).clear();
		return;
	}

	else
	{
		std::cout << "Global Way Point ������ ���� �Ͽ����ϴ�." << std::endl;
	}

	std::cout << "Point ���� �� Point mapping�� �����մϴ�." << std::endl;
	std::vector<std::pair<char*, cv::Point>> imagePoint;

	PointRevision((void*)&selectedPoint,(void*)&imagePoint); //���콺�� ������ ��ǥ�� ImagePoint.txt �� ��ǥ�� ���� �� ����
	
	if ((imagePoint).size() == 0)
	{
		std::cout << "Point ���� �� Point mapping�� �����߽��ϴ�." << std::endl;
		selectedVertex.clear();
		(*realPoint).clear();
		return;
	}

	for (int x = 0; x < (imagePoint).size(); x++)
	{
		selectedVertex.push_back((imagePoint)[x].first);
	}
	std::cout << "Point ���� �� Point mapping�� �Ϸ��߽��ϴ�." << std::endl;

	std::cout << "���õ� Vertex ������"<< selectedVertex.size() <<" �Դϴ�. "<< std::endl;



	std::cout<<std::endl << "���õ� Vertex���� ������ �ִ� ��θ� ����մϴ�. " << std::endl;
	FloydWarshall(&selectedVertex);
	std::cout << std::endl << "�ִ� ��� �˰��� ���� ���õ� Vertex ������" << selectedVertex.size() << " �Դϴ�. " << std::endl;





	if (PointMapping((void*)&selectedVertex, (void*)&imagePoint, (void*)&(*realPoint)))
	{

		std::cout << std::endl << "�ִ� ��� ã�� �� Point Mapping�� �����߽��ϴ�" << std::endl << "��� ������ ����մϴ� " << std::endl;
		drawPath((void*)&imagePoint, (void*)&image); //Path�� �׷��ִ� ����



		cv::imshow("Global Map", image);
		cv::imwrite(".\\GlobalMap.jpg", image);
		cv::waitKey();
		cv::destroyWindow("Global Map");
	}
	else
	{
		std::cout << std::endl << "�ִ� ��� ã�� �� Point Mapping�� �����߽��ϴ�." << std::endl<<std::endl;
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
				std::cout << "����Ʈ�� ���� �Ǿ����ϴ�" << std::endl;
			
			}
			else if (startEndPoint.size() > 0)
				{
					startEndPoint.pop_back();
					image = imageOrigin.clone();
					cv::resize(image, image, cv::Size(IMAGE_WIDHT, IMAGE_HEIGHT));
					std::cout << "����Ʈ�� ���� �Ǿ����ϴ�" << std::endl;
					if (startEndPoint.size() == 0)
					{
						cv::imshow("Global Map",image);
					}
				}

			else
			std::cout << "����� ����Ʈ�� �������� �ʽ��ϴ�." << std::endl;
				
		
		break;

	case CV_EVENT_LBUTTONDBLCLK:
		std::cout << "WayPoint ������ �����մϴ�." << std::endl;
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
		cv::circle(image, (startEndPoint)[0], 5, cv::Scalar(0, 0, 255), -1); //������ Red

		if (startEndPoint.size() != 1)
		{
			cv::circle(image, (startEndPoint)[1], 5, cv::Scalar(255, 0, 0), -1); //������ Blue
			for (int x = 0; x < (*selectedPoint).size(); x++)
			{
				cv::circle(image, (*selectedPoint)[x], 5, cv::Scalar(0, 255, 0), -1); //������ Green

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

	//txt������ ù ���� �̹��� ũ��
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


		if (tempStr != NULL&&tempPoint.x > 0 && tempPoint.y > 0) //0�� Width index�� exception handling�� ���
		{
			if (tempPoint.x < IMAGE_WIDHT&&tempPoint.y < IMAGE_HEIGHT)
			{
				if (vertexPoint.size() == 0)
				{
					vertexPoint.push_back(std::make_pair(tempStr, tempPoint));
				}
				int count = 0;
				for (int x = 0; x < vertexPoint.size(); x++) //�ߺ� ��ǥ ��� X
				{
					if ((vertexPoint[x].second.x == tempPoint.x) && (vertexPoint[x].second.y == tempPoint.y) &&
						(strcmp(tempStr, vertexPoint[x].first) == 0))//������ ������ break;
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
				std::cout << "������� �������� �ùٸ��� �������� �ʾҽ��ϴ�." << std::endl;
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
			std::cout << "������ ";
		}
		else if (x == (*selectedVertex).size() - 1)
		{
			std::cout << "������ ";
		}
		else
		{
			std::cout << "����� ";
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

	//Image txt������ ù ���� �̹��� ũ��
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
		finImage.getline(str, buffSize); //ù�� ����

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

				if (tempStr != NULL&&tempPoint.x > 0 && tempPoint.y > 0) //0�� Width index�� exception handling�� ���
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
		std::cout << "Mapping�� �����Ͽ����ϴ�." << std::endl;
		return false;
	}
	
	return true;


}






