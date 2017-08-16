
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
/*
int main()
{
	Mat map = imread("example1.jpg");
	vector<char*> input;
	vector<string> check;
	char a[] = "1";
	char b[] = "10";
	char c[] = "12";
	char d[] = "3";
	input.push_back(a);
	input.push_back(b);
	input.push_back(c);
	input.push_back(d);

	//함수 실행
	FloydWarshall(&input);

	//출력 결과 확인
	for (int i = 0; i < input.size(); i++)
	{
		cout << input.at(i) << endl;
	}

	imshow("src", map);
	waitKey(0);
	return 0;
}*/

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
	stack<int> path;

	GPS_DD* point_arr;                     //좌표들 저장하는 배열

	double total_distance = 0.0;

	/*파일의 라인 수 파악*/
	lineNum = getlineNum(f_input1);

	//int n = lineNum; //vertex 의 수

	/*인접 행렬 동적 할당*/
	double **graph = new double*[lineNum];

	for (int i = 0; i < lineNum; i++)
	{
		graph[i] = new double[lineNum];
		memset(graph[i], 0, sizeof(double)*lineNum);
	}

	/*초기화*/
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
		AddEdge(graph, start, end);										// 이부분 함수 넘어가는 부분 잘 안됨
	}

	/*인접 행렬 확인*/
	for (int i = 0; i < lineNum; i++)
	{
		for (int j = 0; j < lineNum; j++)
			cout << graph[i][j] << " ";
		cout << endl;
	}


	/*FW(지점 사이의 거리 밑 경유지 정보 포함) 초기화*/
	FW.resize(lineNum + 1);
	for (int i = 0; i < lineNum; i++) {
		for (int j = 0; j < lineNum; j++) {
			FW[i].push_back(make_pair(INF, NIL));                        //FW를 모두 INF(무한), NIL(-1)로 초기화한다.
			if (i == j)                                       //단 i==j일 경우 FW[i][j]의 first에 0으로 초기화 한다.
				FW[i][j].first = 0;
		}
	}

	/*라인 수만큼 동적할당 */
	point_arr = new GPS_DD[lineNum];

	/*text의 값 GPS array에 저장*/
	while (getline(f_input, line)) {

		string *str = strSplit(line, " ");
		vertex = str[0];
		point_arr[index].lon = stod(str[1]);
		point_arr[index].lat = stod(str[2]);
		index++;
	}


	/*좌표 사이의 거리 입력*/
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

	/*graph(좌표간 거리) 출력*/
	for (int i = 0; i < lineNum; i++) {
		//printf("  %c    ", i + 65);
	}
	//cout << endl;
	for (int i = 0; i < lineNum; i++) {
		//printf("%c ", i + 65);
		for (int j = 0; j < lineNum; j++) {

			//cout << graph[i][j] << "m" << " ";

		}
		//cout << endl;
	}

	/*좌표 저장*/
	for (int i = 0; i < lineNum; i++) {
		for (int j = 0; j < lineNum; j++) {
			FW[i][j].first = graph[i][j];                        //FW의 first에 graph(i->j까지의 거리정보)를 입력
			if (FW[i][j].first != INF && FW[i][j].first != 0)            // i->j로 갈수 있고 i==j가 아닐때 
				FW[i][j].second = i;                        //FW[i][j]의 second에는 i로 저장(출발 정보)
		}
	}

	/*최단 거리 계산*/
	for (int i = 0; i < lineNum; i++) {
		for (int j = 0; j < lineNum; j++) {
			for (int k = 0; k < lineNum; k++) {
				if (FW[k][j].first > FW[k][i].first + FW[i][j].first) {      // i를 거쳤을때가 더 최단 경로일때
					FW[k][j].first = FW[k][i].first + FW[i][j].first;         // k->j의 거리를 새로운 값으로 수정해준다   
					FW[k][j].second = FW[i][j].second;            // i 값을 k에 넣어준다.(경로지를 표시해주는 듯)
				}
			}
		}
	}

	/*입력*/
	char* depart_string;                        //출발지 입력 받을 char 변수
	char* arrive_string;                        //도착지 입력 받을 char 변수
	int depart = -1, arrive = -1;                     //출발지, 도착지의 문자 -> 숫자 변수
	int count = 0;                           //중복된 경유지의 갯수
	int waypoint_num = 0;                        //경유지의 수
	int waypoints[SIZE];                        //경유지를 담을 배열
	int waypoint = -1;                           //경유지의 문자 -> 숫자 변수
	int i = 0;
	string conv_tostring;

	vector<string> output;


	/*Input vector 정보 추출*/
	depart_string = (*input).front();                     //처음 원소(출발지) 저장 char -> int형으로
	cout << "출발지 : " << depart_string << endl;
	depart = atoi(depart_string);

	arrive_string = (*input).back();                     //마지막 원소(도착지) 저장 char -> int형으로
	cout << "도착지 : " << arrive_string << endl;
	arrive = atoi(arrive_string);

	waypoint_num = (*input).size() - 2;                  //경유지 갯수 파악
	cout << "경유지 갯수 : " << waypoint_num << endl;

	for (i = 0; i < waypoint_num; i++)               //Input에서 경유지만 따로 빼서 waypoints에 저장 char -> int형으로
	{
		waypoints[i] = atoi((*input).at(i + 1));
	}

	for (i = 0; i < waypoint_num; i++) {
		//cout << "경유지 " << i + 1 << ": " << waypoints[i] << endl;
	}

	int temp = 0;


	/*입력한 경유지부터 arrive까지 사이의 경유지들을 path에 저장*/

	for (int a = waypoint_num - 1; a >= 0; a--) {               //경유지가 있을때만(waypoint_num !=0) 반복

		path.push(arrive);                                  //path의 마지막에 도착지의 값(arrive) push

		temp = waypoints[a];                              //waypoints 마지막 부터 하나씩 경유지 정보를 temp에 넣는다.
		//cout << temp << endl;
		while (temp != FW[temp][arrive].second) {                  //FW안에 있는 temp(입력한 경유지)->arrive 까지의 경유지가 더 있을때만(second != 1)이면 반복

			arrive = FW[temp][arrive].second;                  //arrive까지의 경유지(temp)의 second값 저장 
			path.push(arrive);                              //새로운 arrive를 path에 저장
		}
		arrive = temp;                                 //입력한 경유지(temp)를 arrive에 저장
	}
	path.push(arrive);                                    // 도착지를 path에 push


	/*depart->arrive 사이에 최단거리가 되는 경유지가 있는지 확인*/
	while (depart != FW[depart][arrive].second) {                  //depart 부터 arrive까지 경유지가 있을때 반복
		arrive = FW[depart][arrive].second;
		path.push(arrive);                                 //사이의 경유지들을 path에 저장
	}


	conv_tostring = to_string(depart);
	/*vector<char> writable(conv_tostring.begin(), conv_tostring.end());
	writable.push_back('\0');
	char *conv_tochar = &writable[0];*/
	output.push_back(conv_tostring);

	while (!path.empty()) {

		//writable.clear();

		/*Point의 좌표 이름*/
		//printf("%d- ", depart);

		/*Point의 좌표 주소*/
		//printf(" point = (%lf, %lf) \n", point_arr[depart].lon, point_arr[depart].lat);

		total_distance += graph[depart][path.top()];
		//printf("%lfm\n", total_distance);

		depart = path.top();
		//printf("-%d\n", path.top());

		conv_tostring = to_string(depart);
		/*std::copy(conv_tostring.begin(), conv_tostring.end(), back_inserter(writable));
		writable.push_back('\0');
		conv_tochar = &writable[0];*/
		output.push_back(conv_tostring);

		//cout << endl;

		path.pop();
		//waitKey(0);
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
	int cutAt;                                       // 자르는 위치
	int index = 0;                                    // 문자열 인덱스
	string* strResult = new string[25];                     // 결과 return 변수

	/*strTok을 찾을 때까지 반복*/
	while ((cutAt = strOrigin.find_first_of(strTok)) != strOrigin.npos)
	{
		/*자르는 위치가 0보다 크면*/
		if (cutAt > 0)
		{
			strResult[index++] = strOrigin.substr(0, cutAt);         // 결과 배열에 추가
		}
		strOrigin = strOrigin.substr(cutAt + 1);               // 원본은 자른 부분을 제외한 나머지
	}
	/*원본이 아직 남았으면*/
	if (strOrigin.length() > 0)
	{
		strResult[index++] = strOrigin.substr(0, cutAt);            // 나머지를 결과 배열에 추가
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


