#include<vector>


#ifndef Shortest_Path_Interface
#define Shortest_Path_Interface

#define IMAGE_WIDHT 1151
#define IMAGE_HEIGHT 831

#define IMAGE_LOCATION ".\\GlobalImage.jpg"
#define IMAGE_POINT_LOCATION ".\\ImagePoint.txt"


#define DISTANCE(A,B) (((((A).x)-((B).x))*(((A).x)-((B).x)))+((((A).y)-((B).y))*(((A).y)-((B).y))))
#define MOUSE_DISTANCE 25
#define POINT_DISTANCE 25

#define ESC 27


void SelectInterface(void*);
void MouseSelect(int, int, int , int, void*);
void PointRevision(void*, void*);
int PointMapping(void*, void*, void*);
void clearBuffer(char*, int);
void drawPath(void*, void*);

#endif