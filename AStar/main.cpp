#include <iostream>
#include <cstdlib>
#include <ctime>
#include "AStar.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define LOCALMAPSIZE 100

void CallBackFunc(int event, int x, int y, int flags, void* userdata);

int main()
{
	int countPoint = 0;
	srand((unsigned)time(0));
	Mat local_map(LOCALMAPSIZE, LOCALMAPSIZE, CV_8UC1, Scalar(255));

	std::vector<cv::Point> selectedPoint;

	namedWindow("My Window", 0);
	imshow("My Window", local_map);
	setMouseCallback("My Window", CallBackFunc, (void*)&selectedPoint);

	cv::waitKey(0);

	rectangle(local_map, selectedPoint[1], selectedPoint[2], Scalar(0));

	countPoint = selectedPoint.size();

	if (countPoint >= 2)
	{
		Point startingPoint(selectedPoint[0].x, selectedPoint[0].y);
		Point endingPoint(selectedPoint[countPoint - 1].x, selectedPoint[countPoint - 1].y);

		AStar::Generator generator;
		// Set 2d map size.
		generator.setWorldSize({ LOCALMAPSIZE, LOCALMAPSIZE });
		// You can use a few heuristics : manhattan, euclidean or octagonal.
		generator.setHeuristic(AStar::Heuristic::euclidean);
		generator.setDiagonalMovement(true);

		/*
		for (int i = 1; i < countPoint - 1; i++)
		{
			generator.addCollision({ selectedPoint[i].x, selectedPoint[i].y });
			circle(local_map, Point(selectedPoint[i].x, selectedPoint[i].y), 1, Scalar(0));
		}
		
		int loop = 0;
		while (loop < LOCALMAPSIZE)
		{
			int rand_x = rand() % LOCALMAPSIZE;
			int rand_y = rand() % LOCALMAPSIZE;
			cout << "rand_x " << rand_x << " rand_y " << rand_y << endl;
			generator.addCollision({ rand_x, rand_y });
			loop++;
		}
		*/

		// Create walls
		uchar* data = local_map.data;
		for (int i = 0; i < local_map.rows; i++)
		{
			for (int j = 0; j < local_map.cols; j++)
			{
				if (!(data[i * local_map.cols + j]))
				{
					generator.addCollision({ j, i });
				}
			}
		}
		
		std::cout << "Generate path ... \n";
		// This method returns vector of coordinates from target to source.
		auto path = generator.findPath( { endingPoint.x, endingPoint.y },
		{ startingPoint.x, startingPoint.y });

		int evenCheck = 0;
		AStar::Vec2i prev_point;

		// Checking last node is needed
		for (auto& coordinate : path) {
			evenCheck++;
			if (evenCheck %= 2)
			{
				prev_point = coordinate;
			}
			else
			{
				line(local_map, Point(prev_point.x, prev_point.y), Point(coordinate.x, coordinate.y), Scalar(0));
				/*
				std::cout << prev_point.x << " " << prev_point.y << "\n";
				std::cout << coordinate.x << " " << coordinate.y << "\n";
				*/
			}
			
		}
	}

	imshow("My Window", local_map);

	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	int r = 0, g = 0, b = 0;
	if (event == EVENT_LBUTTONDOWN) {
		std::vector<cv::Point>* selectedPoint = (std::vector<cv::Point>*) userdata;
		(*selectedPoint).push_back(cv::Point(x, y));

		cout << "Left button is clicked - position (" << x << "," << y << ")" << endl;

		/*
		b = local_map.at<Vec3b>(y, x)[0];
		g = local_map.at<Vec3b>(y, x)[1];
		r = local_map.at<Vec3b>(y, x)[2];
		cout << "BGR is : (" << b << "," << g << "," << r << ")" << endl;
*/
	}
	else if (event == EVENT_RBUTTONDOWN)
		cout << "Right button is clicked - position (" << x << "," << y << ")" << endl;
	else if (event == EVENT_MBUTTONDOWN)
		cout << "Middle button is clicked - position (" << x << "," << y << ")" << endl;

}
