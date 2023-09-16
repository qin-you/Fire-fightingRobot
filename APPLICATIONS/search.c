#include "main.h"
u8 along_flag=0;

u8 map[6][6];

//当前位置信息：朝向，x坐标,y坐标
Car_info car_info={0,5,3};

/**
  * @brief 获取小车的位置信息
  */
void GetCarInfo(void)
{
  //
  float length=0;
  length=(left_encoder_count+right_encoder_count)/60*WHEEL_DIR;
  if(length>ONESTEP)
  {
    switch(car_info.direction)
    {
      case 0:car_info.pos_y++;break;
      case 1:car_info.pos_x++;break;
      case 2:car_info.pos_y--;break;
      case 3:car_info.pos_x--;break;
    }
    left_encoder_count-=30;
    right_encoder_count-=30;
    __Sensordata.POS_X=car_info.pos_x;
    __Sensordata.POS_Y=car_info.pos_y;
  }
}


/*function:  实现沿墙的一个step
direction:待转方向  left/front/right
wave：用哪个超声波发起转向  left/front/right
along_direction:沿哪边墙  left/front/right			*/
void wave_wave(u8 turn_direction,u8 wave,u8 along_direction)
{
	//segment start:
	u8 temp;
	if(along_direction==RIGHT)
		along_flag=RIGHT;							//20ms中断内控制沿墙走
	else
		along_flag=LEFT;
	
	// judge
	switch(wave)
	{
		case FRONT:
			Get_Distance_Front();
			while(distance.front>19);			//确保转后离墙15cm
			along_flag=0;
			if(turn_direction==FRONT)
				;															//step end
			if(turn_direction==LEFT)
				turn_left();									//step end
			if(turn_direction==RIGHT)
				turn_right();									//step end
			break;
		case LEFT:
			temp=distance.left;
			Get_Distance_Left();
			while(abs(temp-distance.left)<30);		//由地图可知 左右超声波都是一旦突变到超过30cm就可以转了
			if(turn_direction==FRONT)
				;															//step end
			if(turn_direction==LEFT)
				turn_left();									//step end
			if(turn_direction==RIGHT)
				turn_right();									//step end
			break;
		case RIGHT:
			temp=distance.right;
			Get_Distance_Right();
			while(abs(temp-distance.right)<30);
			if(turn_direction==FRONT)
				;															//step end
			if(turn_direction==LEFT)
				turn_left();									//step end
			if(turn_direction==RIGHT)
				turn_right();									//step end
			break;
		default:break;
	}
}

/*超声波沿墙走 一定距离然后转
num：encoder 脉冲数  距离=脉冲数*0.7327
*/

void wave_encoder(u8* flag,u8 turn_direction,u8 encoder_num,u8 along_direction)
{
	if(along_direction==RIGHT)
		along_flag=RIGHT;
	else
		along_flag=LEFT;
	left_encoder_count=0;
	right_encoder_count=0;
	if(left_encoder_count>=encoder_num){				//arrived
		if(turn_direction==FRONT)
			;														
		else if(turn_direction==LEFT)
			turn_left_withdelay();									
		else if(turn_direction==RIGHT)
			turn_right_withdelay();		
		(*flag)++;																//FLAG++
		along_flag=0;
	}
	else{
		return;
	}

}

//没墙直接直走，用encoder来转向停车
/*
void encoder_encoder(u8 turn_direction,u8 encoder_num)
{
	along_flag=0;			//0表示 没有沿墙  相当于关掉20ms的中断
	MotorLeft(100);
	MotorRight(100);
	while(left_encoder_count<encoder_num);
	if(turn_direction==FRONT)
			;															//step end
	if(turn_direction==LEFT)
			turn_left();									//step end
	if(turn_direction==RIGHT)
			turn_right();									//step end
}

//没墙直接直行，靠超声波数值变化触发转弯停车
//wave:指示某超声波 FRONT/LEFT/RIGHT
void encoder_wave(u8 turn_direction,u8 wave)
{
	u8 temp;
	along_flag=FRONT;
	MotorLeft(100);
	MotorRight(100);
	
	switch(wave)
	{
		case FRONT:
			Get_Distance_Front();
			while(distance.front>19);			//确保转后离墙15cm
			if(turn_direction==FRONT)
				;															//step end
			if(turn_direction==LEFT)
				turn_left();									//step end
			if(turn_direction==RIGHT)
				turn_right();									//step end
			break;
		case LEFT:
			temp=distance.left;
			Get_Distance_Left();
			while(abs((temp-distance.left))<30);		//由地图可知 左右超声波都是一旦突变到超过30cm就可以转了
			if(turn_direction==FRONT)
				;															//step end
			if(turn_direction==LEFT)
				turn_left();									//step end
			if(turn_direction==RIGHT)
				turn_right();									//step end
			break;
		case RIGHT:
			temp=distance.right;
			Get_Distance_Right();
			while(abs(temp-distance.right)<30);
			if(turn_direction==FRONT)
				;															//step end
			if(turn_direction==LEFT)
				turn_left();									//step end
			if(turn_direction==RIGHT)
				turn_right();									//step end
			break;
		default:break;
	}
}
*/


