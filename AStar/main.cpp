#include <iostream>
#include <cstdlib>
#include <ctime>
#include "AStar.hpp"
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

#define LOCALMAPSIZE 200

//void local_path

int main()
{
	int countPoint = 0;
	srand((unsigned)time(0));
	Mat local_map(LOCALMAPSIZE, LOCALMAPSIZE, CV_8UC1, Scalar(255));

	// Open window
	namedWindow("My Window", 0);
	imshow("My Window", local_map);

	AStar::Generator generator;
	// Set 2d map size.
	generator.setWorldSize({ LOCALMAPSIZE, LOCALMAPSIZE });
	// You can use a few heuristics : manhattan, euclidean or octagonal.
	generator.setHeuristic(AStar::Heuristic::euclidean);
	generator.setDiagonalMovement(true);



	local_map = Scalar(255);
	int loop = 0;
	while (loop < LOCALMAPSIZE)
	{
		int rand_x = rand() % LOCALMAPSIZE;
		if (rand_x < 10 || rand_x > LOCALMAPSIZE - 10 )
			rand_x = 100;
		int rand_y = rand() % LOCALMAPSIZE;
		if (rand_y < 10 || rand_y > LOCALMAPSIZE - 10 )
			rand_y = 100;
		circle(local_map, Point(rand_x, rand_y), 1, Scalar(0), 2);
		loop++;
	}

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
	//auto path = generator.findPath( { endingPoint.x, endingPoint.y }, { startingPoint.x, startingPoint.y });
	auto path = generator.findPath({ LOCALMAPSIZE / 2, 5 }, { LOCALMAPSIZE / 2, LOCALMAPSIZE - 5 });

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
			line(local_map, Point(prev_point.x, prev_point.y), Point(coordinate.x, coordinate.y), Scalar(100));
		}

	}

	generator.clearCollisions();

	cv::imshow("My Window", local_map);

	cv::waitKey(500);


	return 0;
}
