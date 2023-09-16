#include "main.h"
u8 along_flag=0;

u8 map[6][6];

//��ǰλ����Ϣ������x����,y����
Car_info car_info={0,5,3};

/**
  * @brief ��ȡС����λ����Ϣ
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


/*function:  ʵ����ǽ��һ��step
direction:��ת����  left/front/right
wave�����ĸ�����������ת��  left/front/right
along_direction:���ı�ǽ  left/front/right			*/
void wave_wave(u8 turn_direction,u8 wave,u8 along_direction)
{
	//segment start:
	u8 temp;
	if(along_direction==RIGHT)
		along_flag=RIGHT;							//20ms�ж��ڿ�����ǽ��
	else
		along_flag=LEFT;
	
	// judge
	switch(wave)
	{
		case FRONT:
			Get_Distance_Front();
			while(distance.front>19);			//ȷ��ת����ǽ15cm
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
			while(abs(temp-distance.left)<30);		//�ɵ�ͼ��֪ ���ҳ���������һ��ͻ�䵽����30cm�Ϳ���ת��
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

/*��������ǽ�� һ������Ȼ��ת
num��encoder ������  ����=������*0.7327
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

//ûǽֱ��ֱ�ߣ���encoder��ת��ͣ��
/*
void encoder_encoder(u8 turn_direction,u8 encoder_num)
{
	along_flag=0;			//0��ʾ û����ǽ  �൱�ڹص�20ms���ж�
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

//ûǽֱ��ֱ�У�����������ֵ�仯����ת��ͣ��
//wave:ָʾĳ������ FRONT/LEFT/RIGHT
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
			while(distance.front>19);			//ȷ��ת����ǽ15cm
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
			while(abs((temp-distance.left))<30);		//�ɵ�ͼ��֪ ���ҳ���������һ��ͻ�䵽����30cm�Ϳ���ת��
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


