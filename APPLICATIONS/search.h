#ifndef SEARCH_H
#define SEARCH_H

#define WHEEL_DIR 7
#define ONESTEP 41.5       //一格迷宫
#define RIGHTWALL_FLAG 0
#define LEFTWALL_FLAG 1
#define NOWALL_FLAG 2

#define abs(x) ((x)>=0?x:-x)
#define FRONT  0	
#define LEFT   1
#define RIGHT  2
//当前位置信息：朝向，x坐标,y坐标
typedef struct{
  u8 direction;
  u8 pos_x;
  u8 pos_y;
}Car_info;

extern u8 along_flag;
extern Car_info car_info;

void GetCarInfo(void);
/*void encoder_wave(u8 turn_direction,u8 wave);
void encoder_encoder(u8 turn_direction,u8 encoder_num);

void wave_wave(u8 turn_direction,u8 wave,u8 along_direction);
void search_path(void);
*/
void wave_encoder(u8* flag,u8 turn_direction,u8 encoder_num,u8 along_direction);
#endif
