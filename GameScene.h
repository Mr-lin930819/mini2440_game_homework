#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "sys.h"
#include "LCD.h"
#include "adc.h"
#include "gui.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <S3C2440.H>
#include <time.h>
#include <Canvas.h>

#define true 1
#define false 0

typedef struct _gamebox{
	int x;
        int y;
	unsigned char type;
	unsigned char is_empty; //--0:空；--1:有图形
}GameBox;


typedef struct _boxindex{
	int x_i;
	int y_i;
}BoxIndex;	//记录一个需要指向的方格的逻辑坐标

typedef struct __buttonbox{
    int left;
    int right;
    int top;
    int bottom;
}ButtonRect;

void PlayGame(int game_level);

#endif
