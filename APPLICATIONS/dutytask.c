#include "main.h"


void taskloop(void)
{
  static u32 system_10ms=0;
  system_10ms++;

  Duty_10ms();
  if(system_10ms%2==0)
    Duty_20ms();
  if(system_10ms%5==0)
		Duty_50ms();
  if(system_10ms%10==0)
    Duty_100ms();
  if(system_10ms%100==0)
    Duty_1000ms();
}

void Duty_10ms(void)
{
	
	//DatatransferTask();
  //judge_fire();
	//MotorLeft(0);	//left63 right80 can go straight
	//MotorRight(0);

	//MotorRight(80);
	//delay_ms(4000);
	
}

void Duty_20ms(void)
{
	
  
	
	
	if(along_flag==RIGHT)
		Pos_ControlRight();
	else if(along_flag==LEFT)
		Pos_ControlLeft();
	else
		;
}





void Duty_50ms(void)
{
	static u8 FirePosition[4]={1,0,1,1};  //标号是火的标号(火的标号是按照车轨迹的先后） 值0表示无火 值1表示有火
	static u8 candle=9;
	static u8 FLAG=1,CLR=1;			//flag:选择step   clr：标志第一次进入step，完成step时退出
	
	if(FLAG==1){								//沿墙 step1  FLAG=0,1
		along_flag=RIGHT;				
		
		Get_Distance_Front();
		if(distance.front<=18){
		if(FirePosition[0]){
		MotorLeft(0);
		MotorRight(0);
		FanMotor_Open();

		turn_right45();
		delay_ms(1800);
    turn_right45_return();
    FanMotor_Close();
		candle--;
		}
			turn_left_withdelay();
			along_flag=0;
			FLAG++;													//along_flag FLAG CLR 
			
	
		}
}
	

	
else if(FLAG==2){
		along_flag=RIGHT;
		if(CLR){
			left_encoder_count=right_encoder_count=0;
			CLR=0;
		}
	if(left_encoder_count>=114||right_encoder_count>=114){			//yuan 110
			turn_left_withdelay();
			along_flag=0;
			FLAG=3;
			CLR=1;
		}
	}
	
else if(FLAG==3){
	along_flag=0;
	MotorLeft(80);
	MotorRight(80);
	Get_Distance_Right();
	if(distance.right<=25){
		//MotorLeft(5);
		//MotorRight(5);
		//delay_ms(50);
		//MotorRight(0);
		//MotorLeft(0);
		FLAG++;
	}
}

else if(FLAG==4){
	along_flag=RIGHT;
	Get_Distance_Front();
	if(distance.front<=18){
		turn_left_withdelay();
		along_flag=0;
		FLAG++;
	}
}

else if(FLAG==5){
	along_flag=RIGHT;
	Get_Distance_Front();
	if(distance.front<=18){
		if(FirePosition[1]){
		MotorLeft(0);
		MotorRight(0);
		FanMotor_Open();
		turn_right45();
		delay_ms(1800);
    turn_right45_return();
    FanMotor_Close();
		candle--;
		}
		turn_left_withdelay();
		if(candle){
			along_flag=0;
			FLAG++;
		}
		else{
			along_flag=0;
			FLAG=100;
		}
		
	}
}

else if(FLAG==6){
	along_flag=RIGHT;
	if(CLR){
		Get_Distance_Front();     //灭火后第一次测得的超声波数据不稳会直接触发转弯
		Get_Distance_Front();
		Get_Distance_Front();
		Get_Distance_Front();
		CLR=0;
	}
	Get_Distance_Front();
	if(distance.front<=18){
		turn_left_withdelay();
		FLAG++;
		along_flag=0;
		CLR=1;
	}
}

else if(FLAG==7){
	along_flag=RIGHT;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	if(left_encoder_count>=15||right_encoder_count>=15){		//11cm
		along_flag=0;
		FLAG++;
		CLR=1;
	}
}

else if(FLAG==8){
	along_flag=0;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	MotorLeft(70);
	MotorRight(80);			//为区分段，后期改大  右轮大于左轮因为沿墙调节不及时
	Get_Distance_Front();
	if(distance.front<=18){
		turn_right_withdelay();
		CLR=1;
		FLAG++;
	}
}

else if(FLAG==9){
	along_flag=0;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	MotorLeft(74);
	MotorRight(80);
	Get_Distance_Front();
	if(distance.front<=18){
		turn_left_withdelay();
		FLAG++;
		CLR=1;
		along_flag=0;
	}
}

else if(FLAG==10){
	along_flag=RIGHT;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	Get_Distance_Front();
	//if(left_encoder_count>=53||right_encoder_count>=53){
	if(distance.front<=18){
		turn_left_withdelay();
		FLAG++;
		CLR=1;
		along_flag=0;
	}
}

else if(FLAG==11){
	along_flag=RIGHT;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	Get_Distance_Left();
	//if(left_encoder_count>=12||right_encoder_count>=12){
	if(distance.left<=20){
		FLAG++;
		CLR=1;
		along_flag=0;
	}
}

else if(FLAG==12){
	along_flag=LEFT;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	if(left_encoder_count>=80||right_encoder_count>=80){
		turn_right_withdelay();
		FLAG++;
		along_flag=0;
		CLR=1;
	}
}

else if(FLAG==13){
	along_flag=0;
	MotorLeft(80);
	MotorRight(80);
	Get_Distance_Left();
	if(distance.left<=35){
		
		FLAG++;
	}
}

else if(FLAG==14){
	along_flag=LEFT;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	Get_Distance_Front();
	//if(left_encoder_count>=85||right_encoder_count>=85){
	if(distance.front<=18){
		if(FirePosition[2]){
		MotorLeft(0);
		MotorRight(0);
		FanMotor_Open();
		turn_left45();
		delay_ms(2100);
    turn_left45_return();
    FanMotor_Close();
		candle--;
		}
		if(candle){
		
			turn_right_withdelay();
			FLAG++;
			CLR=1;
			along_flag=0;
		}
		else{
			turn_right_withdelay();
			
			turn_right_withdelay();
			
			FLAG=21;
			CLR=1;
			along_flag=0;
		}
	}
}

else if(FLAG==15){
	along_flag=LEFT;
	if(CLR){
		Get_Distance_Front();     //灭火后第一次测得的超声波数据不稳会直接触发转弯
		Get_Distance_Front();
		Get_Distance_Front();
		Get_Distance_Front();
		CLR=0;
	}
	Get_Distance_Front();
	//if(left_encoder_count>=110||right_encoder_count>=110){
	if(distance.front<=18){
		turn_right_withdelay();
		FLAG++;
		CLR=1;
		along_flag=0;
	}
}

else if(FLAG==16){
	along_flag=LEFT;
	
	Get_Distance_Front();
	//if(left_encoder_count>=51||right_encoder_count>=51){
	if(distance.front<=18){
		if(FirePosition[3]){
		MotorLeft(0);
		MotorRight(0);
		FanMotor_Open();
		turn_left45();
		delay_ms(1800);
    turn_left45_return();
    FanMotor_Close();
		}
		turn_right_withdelay();
		FLAG++;
		
		along_flag=0;
	}
}

else if(FLAG==17){
	along_flag=LEFT;
	if(CLR){
		Get_Distance_Front();     //灭火后第一次测得的超声波数据不稳会直接触发转弯
		Get_Distance_Front();
		Get_Distance_Front();
		Get_Distance_Front();
		CLR=0;
	}
	Get_Distance_Front();
	//if(left_encoder_count>=51||right_encoder_count>=51){
	if(distance.front<=18){
		turn_right_withdelay();
		FLAG++;
		CLR=1;
		along_flag=0;
	}
}

else if(FLAG==18){
	along_flag=LEFT;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	if(left_encoder_count>=13||right_encoder_count>=13){
		along_flag=0;
		CLR=1;
		FLAG++;
	}
}

else if(FLAG==19){
	along_flag=0;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	MotorLeft(90);
	MotorRight(90);
	Get_Distance_Front();
	if(distance.front<=18){
		turn_left_withdelay();
		CLR=1;
		FLAG++;
	}
}

else if(FLAG==20){
	along_flag=RIGHT;
	Get_Distance_Front();
	if(distance.front<=18){
		turn_left_withdelay();
		FLAG++;
		along_flag=0;
	}
}

else if(FLAG==21){
	along_flag=RIGHT;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	if(left_encoder_count>=70||right_encoder_count>=70){
		along_flag=0;
		FLAG++;
		CLR=1;
	}
}

else if(FLAG==22){
	along_flag=0;
	MotorLeft(74);
	MotorRight(80);
	Get_Distance_Front();
	if(distance.front<=18){
		turn_left_withdelay();
		FLAG++;
		along_flag=0;
	}
}

else if(FLAG==23){
	along_flag=RIGHT;
	Get_Distance_Right();     //用右侧超声波突变 会使车身向右倾
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	//if(left_encoder_count>=50||right_encoder_count>=50){
	if(distance.right>=25){
		along_flag=0;
		FLAG++;
		CLR=1;
	}
}

else if(FLAG==24){
	along_flag=0;
	MotorLeft(80);
	MotorRight(80);
	Get_Distance_Front();
	if(distance.front<=18){
		turn_right_withdelay();
		FLAG++;
	}
}

else if(FLAG==25){
	along_flag=LEFT;
	Get_Distance_Front();
	if(distance.front<=18){
		turn_right_withdelay();
		FLAG++;
		along_flag=0;
	}
}

else if(FLAG==26){
	along_flag=LEFT;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	if(left_encoder_count>=138||right_encoder_count>=138){
		along_flag=0;
		CLR=1;
		FLAG++;
		
		MotorLeft(0);
		MotorRight(0);
	}
}

else if(FLAG==100){
	along_flag=RIGHT;
	if(CLR){
		Get_Distance_Front();     //灭火后第一次测得的超声波数据不稳会直接触发转弯
		Get_Distance_Front();
		Get_Distance_Front();
		Get_Distance_Front();
		CLR=0;
	}
	Get_Distance_Front();
	if(distance.front<=18){
		turn_left_withdelay();
		FLAG++;
		along_flag=0;
		CLR=1;
	}
}

else if(FLAG==101){
	along_flag=RIGHT;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	if(left_encoder_count>=15||right_encoder_count>=15){		//11cm
		along_flag=0;
		FLAG++;
		CLR=1;
	}
}

else if(FLAG==102){
	along_flag=0;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	MotorLeft(59);
	MotorRight(80);			//为区分段，后期改大  右轮大于左轮因为沿墙调节不及时
	Get_Distance_Front();
	if(distance.front<=18){
		turn_right_withdelay();
		CLR=1;
		FLAG++;
	}
}

else if(FLAG==103){
	along_flag=0;
	if(CLR){
		left_encoder_count=right_encoder_count=0;
		CLR=0;
	}
	MotorLeft(100);
	MotorRight(88);
	Get_Distance_Front();
	if(distance.front<=18){
		turn_right_withdelay();
		FLAG=25;
		CLR=1;
		along_flag=0;
	}
}


}









void Duty_100ms(void)
{

}

void Duty_1000ms(void)
{
	
}
