#ifndef __STEP_H
#define __STEP_H
#include "sys.h"

extern int first[3];//顺序
extern int second[3];

void aim_blobs(void);
void aim_circle(int color);
void move_or_not(void);
void move_or_not_2(void);
void keystart(void);
void Mission(void);
void arm_moving(int order,int ci);
uint8_t ViewColor(void);
void OperationRunning(int *array);//识别、抓取


#endif

