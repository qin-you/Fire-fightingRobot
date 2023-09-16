#include "main.h"

//  float P,I,D,FF,MaxError
static PIDParams pos_pidparam={0,0,0,0,80};

//  float Error,Target;
static PIDState  pos_pidstate={0,14};

//  int8_t ne_min,ne_two,ne_one,zero,one,two,max;比例参数
static FuzzyParam fuzzyparam={-35,-20,-15,0,15,20,35};

//  int8_t ne_min,ne_two,ne_one,zero,one,two,max;微分参数
//static FuzzyD fuzzyd={50,15,8,5,8,15,50};
static FuzzyD fuzzyd={30,15,15,40,15,15,30};

/*
now_dis 本次测量的距离    last_dis 上次测量的距离
now_diff 本次误差信号量    last_diff 上次误差信号量
*/
void Pos_ControlLeft(void)
{
	 float e=0,de=0,ctrl_signal=0;
    Get_Distance_Left();
    e = pos_pidstate.Target - distance.left;
    de = e - pos_pidstate.Error;
    pos_pidstate.Error = e;

    //模糊规则
    if(e>2)
    {
      ctrl_signal=fuzzyparam.max+fuzzyd.max*de;
    }
    else if(e==2)
    {
      ctrl_signal=fuzzyparam.two+fuzzyd.two*de;
    }
    else if(e==1)
    {
      ctrl_signal=fuzzyparam.one+fuzzyd.one*de;
    }
    else if(e==0)
    {
      ctrl_signal=fuzzyparam.zero+fuzzyd.zero*de;
    }
    else if(e==-1)
    {
      ctrl_signal=fuzzyparam.ne_one+fuzzyd.ne_one*de;
    }
    else if(e==-2)
    {
      ctrl_signal=fuzzyparam.ne_two+fuzzyd.ne_two*de;
    }
    else if(e<-2)
    {
      ctrl_signal=fuzzyparam.ne_min+fuzzyd.ne_min*de;
    }

    if(ctrl_signal>pos_pidparam.MaxError)
    {
      ctrl_signal=pos_pidparam.MaxError;
    }
    else if(ctrl_signal<-pos_pidparam.MaxError)
    {
      ctrl_signal=-pos_pidparam.MaxError;
    }

    if(e>0)                  //左偏
    {
      MotorLeft(100);
      MotorRight(100-ctrl_signal);
    }
    else if(e<0)             //右偏
    {
      MotorLeft(100+ctrl_signal);
      MotorRight(100);
    }
    else if(de>0)       //右边回来
    {
      MotorLeft(100);
      MotorRight(100-ctrl_signal);
    }
    else                //左边回来
    {
      MotorLeft(100+ctrl_signal);
      MotorRight(100);
    }

}
void Pos_ControlRight(void)
{
    float e=0,de=0,ctrl_signal=0;
    Get_Distance_Right();
    e = pos_pidstate.Target - distance.right;
    de = e - pos_pidstate.Error;
    pos_pidstate.Error = e;

    //模糊规则
    if(e>2)
    {
      ctrl_signal=fuzzyparam.max+fuzzyd.max*de;
    }
    else if(e==2)
    {
      ctrl_signal=fuzzyparam.two+fuzzyd.two*de;
    }
    else if(e==1)
    {
      ctrl_signal=fuzzyparam.one+fuzzyd.one*de;
    }
    else if(e==0)
    {
      ctrl_signal=fuzzyparam.zero+fuzzyd.zero*de;
    }
    else if(e==-1)
    {
      ctrl_signal=fuzzyparam.ne_one+fuzzyd.ne_one*de;
    }
    else if(e==-2)
    {
      ctrl_signal=fuzzyparam.ne_two+fuzzyd.ne_two*de;
    }
    else if(e<-2)
    {
      ctrl_signal=fuzzyparam.ne_min+fuzzyd.ne_min*de;
    }

    if(ctrl_signal>pos_pidparam.MaxError)
    {
      ctrl_signal=pos_pidparam.MaxError;
    }
    else if(ctrl_signal<-pos_pidparam.MaxError)
    {
      ctrl_signal=-pos_pidparam.MaxError;
    }

    if(e>0)                  //右偏
    {
      MotorLeft(100-ctrl_signal);
      MotorRight(100);
    }
    else if(e<0)             //左偏
    {
      MotorLeft(100);
      MotorRight(100+ctrl_signal);
    }
    else if(de>0)       //左边回来
    {
      MotorLeft(100-ctrl_signal);
      MotorRight(100);
    }
    else                //右边回来
    {
      MotorLeft(100);
      MotorRight(100+ctrl_signal);
    }

}

void Speed_Control(void)
{

}
