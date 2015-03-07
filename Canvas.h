#ifndef _CANVAS_H_
#define _CANVAS_H_
#include "LCD.h"
#include <stdlib.h>
#include <string.h>

typedef unsigned char * Bitmap;

/*
struct BitmapsNode{
	Bitmap bmp_data;
	uint16 x;
	uint16 y;
	uint16 width;
	uint16 height;
	int layer;
	char id[5];
	struct BitmapsNode *next;
};

typedef struct BitmapsNode *Bitmaps;

struct Canvas{
	Bitmaps bmps;
	struct BitmapsNode *bmps_tail;
	int max_layer;
};	

*/


struct BitmapsNode{
	Bitmap bmp_data;
	uint16 x;
	uint16 y;
	uint16 width;
	uint16 height;
	int layer;
	char id[5];
	struct BitmapsNode *next;
};

typedef struct BitmapsNode Canvas[30];
/*
 *
 * 绘图的接口函数
 *
 */
//struct Canvas* createEmptyCanvas(void);
void addBitmap(Canvas canvas,Bitmap bmp,uint16 x,uint16 y,uint16 width,uint16 height,char id[],int layer);
void deleteBitmapById(Canvas canvas,char id[]);
//void deleteBitmapByLocation(Canvas canvas,int x,int y);
void cleanCanvas(Canvas canvas);
void update(Canvas canvas);	

#endif
