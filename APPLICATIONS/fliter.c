#include "main.h"

/*
Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
R:测量噪声，R增大，动态响应变慢，收敛稳定性变好
x_last=x(k-1|k-1)上一时刻的距离    p_last=p(k-1|k-1)上一时刻的协方差，上个时刻的迭代值
x_mid=x(k|k-1)，这一时刻的预测距离    p_mid=p(k|k-1)预测协方差
z_measure 测量值    R测量噪声，可理解为测量协方差
x_now计算得出的本时刻距离，p_now，计算出的本时刻协方差，供下一时刻迭代
*/

void kalmanfilter_r(u16 *z_measure)
{
  static float x_mid,x_last=0,p_mid,p_last=0,p_now,x_now=0;
  static float Q=ProcessNiose_Q,kg,R=MeasureNoise_R;
  static int8_t init_flag=0;
  if(!init_flag)
  {
    init_flag=1;
    x_last=*z_measure;
  }
  x_mid=x_last;                       //预测与上一时刻一致
  p_mid=p_last+Q;                     //预测协方差为上一时刻协方差+Q_Noise
  kg=p_mid/(p_mid+R);                 //kg为kalman filter，R 为测量噪声
  x_now=x_mid+kg*(*z_measure-x_mid);  //计算出的最优值
  p_now=(1-kg)*p_mid;                   //最优值对应的covariance
  p_last=p_now;                         //更新covariance 值
  x_last=x_now;                            //更新系统状态值
  *z_measure=(u16)x_now;
}

void kalmanfilter_l(u16 *z_measure)
{
  static float x_mid2,x_last2=0,p_mid2,p_last2=0,p_now2,x_now2=0;
  static float Q2=ProcessNiose_Q,kg2,R2=MeasureNoise_R;
  static int8_t init_flag2=0;
  if(!init_flag2)
  {
    init_flag2=1;
    x_last2=*z_measure;
  }
  x_mid2=x_last2;                       //预测与上一时刻一致
  p_mid2=p_last2+Q2;                     //预测协方差为上一时刻协方差+Q_Noise
  kg2=p_mid2/(p_mid2+R2);                 //kg为kalman filter，R 为测量噪声
  x_now2=x_mid2+kg2*(*z_measure-x_mid2);  //计算出的最优值
  p_now2=(1-kg2)*p_mid2;                   //最优值对应的covariance
  p_last2=p_now2;                         //更新covariance 值
  x_last2=x_now2;                            //更新系统状态值
  *z_measure=(u16)x_now2;
}

void kalmanfilter_f(u16 *z_measure)
{
  static float x_mid3,x_last3=0,p_mid3,p_last3=0,p_now3,x_now3=0;
  static float Q3=ProcessNiose_Q,kg3,R3=MeasureNoise_R;
  static int8_t init_flag3=0;
  if(!init_flag3)
  {
    init_flag3=1;
    x_last3=*z_measure;
  }
  x_mid3=x_last3;                       //预测与上一时刻一致
  p_mid3=p_last3+Q3;                     //预测协方差为上一时刻协方差+Q_Noise
  kg3=p_mid3/(p_mid3+R3);                 //kg为kalman filter，R 为测量噪声
  x_now3=x_mid3+kg3*(*z_measure-x_mid3);  //计算出的最优值
  p_now3=(1-kg3)*p_mid3;                   //最优值对应的covariance
  p_last3=p_now3;                         //更新covariance 值
  x_last3=x_now3;                            //更新系统状态值
  *z_measure=(u16)x_now3;
}

//滤掉粗大误差
void limitfilter(u16 *measure,u16 refer)
{
  int16_t err=*measure-refer;
  if(err>20||err<20)
  {
    *measure=refer;
  }
}
