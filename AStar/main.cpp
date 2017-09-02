#include <iostream>
#include <cstdlib>
#include <ctime>
#include "AStar.hpp"
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

#define LOCALMAPSIZE 100

int direction_checker(AStar::Vec2i& prev, AStar::Vec2i& cur);
bool collision_checker(AStar::Generator& gen, AStar::Vec2i& point);

int main()
{
	srand((unsigned)time(0));
	Mat local_map(LOCALMAPSIZE, LOCALMAPSIZE, CV_8UC1, Scalar(255));
	Mat modified_map(LOCALMAPSIZE, LOCALMAPSIZE, CV_8UC1, Scalar(255));
	vector<AStar::Vec2i> initial_path;
	vector<AStar::Vec2i> modified_path;

	int evenCheck = 0;
	int numOfPoints = 0;

	int prev_direction = 0;
	int cur_direction = 0;
	bool is_collision = false;
	bool is_first = true;
	AStar::Vec2i start_point = { LOCALMAPSIZE / 2, LOCALMAPSIZE - 5 };
	AStar::Vec2i end_point = { LOCALMAPSIZE / 2, 5 };
	AStar::Vec2i prev_coord;
	AStar::Vec2i cur_coord;
	AStar::Vec2i prev_point;
	AStar::Vec2i cur_point;

	// Open window
	namedWindow("My Window", 0);
	namedWindow("modified_map", 0);

	// Put first point in the modified_path
	modified_path.push_back(start_point);

	AStar::Generator generator;
	// Set 2d map size.
	generator.setWorldSize({ LOCALMAPSIZE, LOCALMAPSIZE });
	// You can use a few heuristics : manhattan, euclidean or octagonal.
	generator.setHeuristic(AStar::Heuristic::euclidean);
	generator.setDiagonalMovement(false);


	// Random circle shape obstacle generator
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
		circle(modified_map, Point(rand_x, rand_y), 1, Scalar(0), 2);
		loop++;
	}

	/*
	// Rectangle shape obstacle generator
	rectangle(local_map, Point(LOCALMAPSIZE / 4, LOCALMAPSIZE / 2), Point(LOCALMAPSIZE * 3 / 4, LOCALMAPSIZE / 2 + 10) , Scalar(0));
	rectangle(modified_map, Point(LOCALMAPSIZE / 4, LOCALMAPSIZE / 2), Point(LOCALMAPSIZE * 3 / 4, LOCALMAPSIZE / 2 + 10) , Scalar(0));
	*/


	// Put coordinates in the wall vector
	// col : x, row : y
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
	// auto path = generator.findPath( { endingPoint.x, endingPoint.y }, { startingPoint.x, startingPoint.y });
	auto path = generator.findPath(end_point, start_point );

	is_first = true;
	// Checking last node is needed
	for (auto& coordinate : path)
	{
		if(is_first)
		{
			cur_coord = coordinate;
			is_first = false;
			continue;
		}

		prev_coord = cur_coord;
		cur_coord = coordinate;

		// direction checker
		prev_direction = cur_direction;


		cur_direction = direction_checker(prev_coord, cur_coord);

		// If direction is changed, save prev_point in the vector
		if(prev_direction != cur_direction)
		{
			// check prev point has collision to the wall
			// true means collision accrued
			is_collision = collision_checker(generator, prev_coord);
			if(!is_collision) modified_path.push_back(prev_coord);
		}

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
	// Put last point in the modified_path
	modified_path.push_back(end_point);

	is_first = true;
	for (auto& coordinate : modified_path)
	{
		if(is_first)
		{
			cur_point = coordinate;
			is_first = false;
			continue;
		}
		prev_point = cur_point;
		cur_point = coordinate;
		line(modified_map, Point(prev_point.x, prev_point.y), Point(cur_point.x, cur_point.y), Scalar(100));

		cout << coordinate.x << " " << coordinate.y << endl;
	}

	generator.clearCollisions();

	cv::imshow("My Window", local_map);
	cv::imshow("modified_map", modified_map);

	cv::waitKey(0);

	return 0;
}

int direction_checker(AStar::Vec2i& prev, AStar::Vec2i& cur)
{
	int x = 0;
	x = cur.x - prev.x;

	// up = 0, 1 = right, -1 = left
	if(x == 0) return 0;
	else if(x == 1) return 1;
	else return -1;
}

bool collision_checker(AStar::Generator& gen, AStar::Vec2i& point)
{
	// collision accrued = true
	bool left_check = false;
	bool right_check = false;
	bool up_check = false;
	bool down_check = false;

	AStar::Vec2i left = { -1, 0 };
	AStar::Vec2i right = { 1, 0 };
	AStar::Vec2i up = { 0, -1 };
	AStar::Vec2i down = { 0, 1 };

	left_check = gen.detectCollision(point + left);
	right_check = gen.detectCollision(point + right);
	up_check = gen.detectCollision(point + up);
	down_check = gen.detectCollision(point + down);

	if(left_check || right_check || up_check || down_check ) return true;
	else return false;
}
