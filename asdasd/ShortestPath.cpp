
#include <stack>
#include <fstream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include"DGPS.h"
#include"ShortestPath.h"

using namespace std;
using namespace cv;

string* strSplit(string strOrigin, string strTok);
int getlineNum(ifstream &infile);
void FloydWarshall(vector<char*>*input);
void str2ch(char* ch, int buffSize, string str);
void AddEdge(double **graph, int start, int end);


void FloydWarshall(vector<char*>*input)
{
	ifstream f_input(GLOBAL_PATH_LOCATION);
	ifstream f_input1(GLOBAL_PATH_LOCATION);
	ifstream f_graph("Graph.txt");
	string line;
	string point;
	string edge;
	string vertex;
	string longtitude;
	string latitude;
	int lineNum = 0;
	int index = 0;

	vector<vector<pair<double, int>>> FW;
	vector<int> path;

	GPS_DD* point_arr;                    

	double total_distance = 0.0;

	
	lineNum = getlineNum(f_input1);

	
	double **graph = new double*[lineNum];

	for (int i = 0; i < lineNum; i++)
	{
		graph[i] = new double[lineNum];
		memset(graph[i], 0, sizeof(double)*lineNum);
	}


	for (int i = 0; i < lineNum; i++)
	{
		for (int j = 0; j < lineNum; j++)
		{
			if (i == j)
			{
				graph[i][j] = 0;
			}
			else
			{
				graph[i][j] = INF;
			}
		}
	}


	while (getline(f_graph, edge))
	{
		string*str = strSplit(edge, " ");
		int start = stoi(str[0]);
		int end = stoi(str[1]);
		AddEdge(graph, start, end);									
	}

	
	for (int i = 0; i < lineNum; i++)
	{
		for (int j = 0; j < lineNum; j++)
			cout << graph[i][j] << " ";
		cout << endl;
	}


	
	FW.resize(lineNum + 1);
	for (int i = 0; i < lineNum; i++) {
		for (int j = 0; j < lineNum; j++) {
			FW[i].push_back(make_pair(INF, NIL));                    
			if (i == j)                                      
				FW[i][j].first = 0;
		}
	}

	
	point_arr = new GPS_DD[lineNum];

	while (getline(f_input, line)) {

		string *str = strSplit(line, " ");
		vertex = str[0];
		point_arr[index].lon = stod(str[1]);
		point_arr[index].lat = stod(str[2]);
		index++;
	}


	
	for (int i = 0; i < lineNum; i++) {
		for (int j = 0; j < lineNum; j++) {
			if (graph[i][j] != INF && graph[i][j] != 0.0)
			{
				GPS_DD A, B;
				double distance;
				A.lon = point_arr[i].lon;
				A.lat = point_arr[i].lat;

				B.lon = point_arr[j].lon;
				B.lat = point_arr[j].lat;

				distance = GPS_DD2Dist(A, B);

				graph[i][j] = distance;

			}
		}
	}

	
	for (int i = 0; i < lineNum; i++) {
		for (int j = 0; j < lineNum; j++) {
			FW[i][j].first = graph[i][j];                       
			if (FW[i][j].first != INF && FW[i][j].first != 0)          
				FW[i][j].second = i;                      
		}
	}


	for (int i = 0; i < lineNum; i++) {
		for (int j = 0; j < lineNum; j++) {
			for (int k = 0; k < lineNum; k++) {
				if (FW[k][j].first > FW[k][i].first + FW[i][j].first) {     
					FW[k][j].first = FW[k][i].first + FW[i][j].first;         
					FW[k][j].second = FW[i][j].second;          
				}
			}
		}
	}

	char* depart_string;								
	char* arrive_string;								
	int depart = -1, arrive = -1;							
	int count = 0;									
	int waypoint_num = 0;								
	int waypoints[SIZE];								
	int waypoint = -1;									
	int i = 0;
	int new_second = 0;
	string conv_tostring;
	path.clear();
	vector<string> output;



	depart_string = (*input).front();                    
	cout << "Starting Point : " << depart_string << endl;
	depart = atoi(depart_string);

	arrive_string = (*input).back();                    
	cout << "Finishing Point : " << arrive_string << endl;
	arrive = atoi(arrive_string);

	waypoint_num = (*input).size() - 2;                 
	cout << "The number of waypoints : " << waypoint_num << endl;

	for (i = 0; i < waypoint_num; i++)             
	{
		waypoints[i] = atoi((*input).at(i + 1));
	}

	for (i = 0; i < waypoint_num; i++) {
	
	}

	int temp = 0;

	int front = 0;	//
	int tail = 0;

	path.push_back(depart);
	path.push_back(depart);
	front = depart;
	tail = arrive;
	if (waypoint_num == 0)				
	{
		while (front != arrive) {
			while (front != FW[front][tail].second)
			{

				tail = FW[front][tail].second;
			}
			path.push_back(tail);
			front = tail;
			tail = arrive;
		}
	}
	else								
	{
		for (int i = 0; i < waypoint_num; i++)			
		{
			tail = waypoints[i];

			while (front != FW[front][tail].second)
			{

				tail = FW[front][tail].second;
			}
			if (path.at(path.size() - 2) == tail)
			{
				double min = FW[depart][0].first + FW[0][arrive].first;
				double secondMin = min;

				for (int i = 0; i < lineNum;i++)
				{
					if (FW[front][i].first + FW[i][tail].first < min)
					{
						secondMin = min;
						min = FW[front][i].first + FW[i][tail].first;
					}
					else if ((min < (FW[front][i].first + FW[i][tail].first) && (FW[front][i].first + FW[i][tail].first) < secondMin) || min == secondMin)
					{
						secondMin = FW[front][i].first + FW[i][tail].first;
						new_second = i;
					}
				}
				tail = new_second;

			}

			while (front != waypoints[i]) {
				while (front != FW[front][tail].second)
				{
					tail = FW[front][tail].second;
				}
				path.push_back(tail);
				front = tail;
				tail = waypoints[i];
			}

		}

		int temp3;
		front = tail;
		tail = arrive;										

		while (front != FW[front][tail].second)
		{

			tail = FW[front][tail].second;
		}
		if (path.at(path.size() - 2) == tail)
		{
			double min = FW[front][tail].first + FW[tail][arrive].first;
			double secondMin = min;

			for (int i = 0; i < lineNum;i++)
			{
				if (FW[front][i].first + FW[i][arrive].first < min)
				{
					secondMin = min;
					min = FW[front][i].first + FW[i][arrive].first;
				}
				else if ((min < (FW[front][i].first + FW[i][arrive].first) && (FW[front][i].first + FW[i][arrive].first) < secondMin) || min == secondMin)
				{
					secondMin = FW[front][i].first + FW[i][arrive].first;
					new_second = i;
				}
			}
			tail = new_second;

		}

		while (front != arrive) {							
			while (front != FW[front][tail].second)
			{

				tail = FW[front][tail].second;
			}
			path.push_back(tail);
			front = tail;
			tail = arrive;
		}
	}

	path.erase(path.begin());							


	while (!path.empty()) {
		int i = 0;
		
		printf("%d- ", depart);

	
		printf(" point = (%lf, %lf) \n", point_arr[depart].lon, point_arr[depart].lat);

		total_distance += graph[depart][path.back()];
		printf("%lfm\n", total_distance);

		//depart = path.top();
		depart = path.back();
		printf("-%d\n", path.back());

		conv_tostring = to_string(depart);
		//output.push_back(conv_tostring);
		output.insert(output.begin() + i, conv_tostring);
		cout << endl;

		//path.pop();
		path.pop_back();
		i++;
	}


	(*input).clear();

	for (int x = 0; x < output.size(); x++)
	{
		char* temp = 0;
		int buffSize = 32;
		temp = (char*)malloc(buffSize);
		str2ch(temp, buffSize, output[x]);
		(*input).push_back(temp);
	}

	f_input.close();
	f_input1.close();

}
void AddEdge(double **graph, int start, int end)
{
	graph[start][end] = 1;
	graph[end][start] = 1;
}

int getlineNum(ifstream &infile)
{
	int count = 0;
	char line[512];
	while (!infile.eof())
	{
		infile.getline(line, 512);
		count++;
	}
	return count;
}

string* strSplit(string strOrigin, string strTok)
{
	int cutAt;                                      
	int index = 0;                                   
	string* strResult = new string[25];                    

	
	while ((cutAt = strOrigin.find_first_of(strTok)) != strOrigin.npos)
	{
		
		if (cutAt > 0)
		{
			strResult[index++] = strOrigin.substr(0, cutAt);        
		}
		strOrigin = strOrigin.substr(cutAt + 1);              
	}
	
	if (strOrigin.length() > 0)
	{
		strResult[index++] = strOrigin.substr(0, cutAt);          
	}

	return strResult;
}


void str2ch(char* ch, int buffSize, string str)
{
	for (int x = 0; x < buffSize; x++)
	{

		if (isalnum(str[x]))
		{
			ch[x] = str[x];
		}
		else{
			ch[x] = 0;
			break;
		}
	}
}


