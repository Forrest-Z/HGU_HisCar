

#ifndef ShortestPath
#define ShortestPath
#include<iostream>
#include<vector>

#define GLOBAL_PATH_LOCATION ".\\GlobalPath_coordinate.txt"
#define INF 10000
#define NIL -1
#define SIZE 14

std::string* strSplit(std::string strOrigin, std::string strTok);
int getlineNum(std::ifstream &infile);
void FloydWarshall(std::vector<char*>*input);
void str2ch(char* ch, int buffSize, std::string str);
void AddEdge(double **graph, int start, int end);
#endif