#ifndef __OPENMV_H
#define __OPENMV_H

extern int rx,ry,gx,gy,bx,by;//物块或圆环位置

void openmv_Init(void);

void scan_stop(void);
void scan_begin(void);

void scan(int *first,int *second);
//扫描函数，a为0扫二维码，a为1扫条形码

void colour(int *rx,int *ry,int *gx,int *gy,int *bx,int *by,int a);
void chose(int rx,int ry,int gx,int gy,int bx,int by,int *get_x,int *get_y);
//物块、圆环的函数，a为0找物块，a为1红2绿3蓝找圆环
#endif
