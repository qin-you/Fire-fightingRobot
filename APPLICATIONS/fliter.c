#include "main.h"

/*
Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��
x_last=x(k-1|k-1)��һʱ�̵ľ���    p_last=p(k-1|k-1)��һʱ�̵�Э����ϸ�ʱ�̵ĵ���ֵ
x_mid=x(k|k-1)����һʱ�̵�Ԥ�����    p_mid=p(k|k-1)Ԥ��Э����
z_measure ����ֵ    R���������������Ϊ����Э����
x_now����ó��ı�ʱ�̾��룬p_now��������ı�ʱ��Э�������һʱ�̵���
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
  x_mid=x_last;                       //Ԥ������һʱ��һ��
  p_mid=p_last+Q;                     //Ԥ��Э����Ϊ��һʱ��Э����+Q_Noise
  kg=p_mid/(p_mid+R);                 //kgΪkalman filter��R Ϊ��������
  x_now=x_mid+kg*(*z_measure-x_mid);  //�����������ֵ
  p_now=(1-kg)*p_mid;                   //����ֵ��Ӧ��covariance
  p_last=p_now;                         //����covariance ֵ
  x_last=x_now;                            //����ϵͳ״ֵ̬
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
  x_mid2=x_last2;                       //Ԥ������һʱ��һ��
  p_mid2=p_last2+Q2;                     //Ԥ��Э����Ϊ��һʱ��Э����+Q_Noise
  kg2=p_mid2/(p_mid2+R2);                 //kgΪkalman filter��R Ϊ��������
  x_now2=x_mid2+kg2*(*z_measure-x_mid2);  //�����������ֵ
  p_now2=(1-kg2)*p_mid2;                   //����ֵ��Ӧ��covariance
  p_last2=p_now2;                         //����covariance ֵ
  x_last2=x_now2;                            //����ϵͳ״ֵ̬
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
  x_mid3=x_last3;                       //Ԥ������һʱ��һ��
  p_mid3=p_last3+Q3;                     //Ԥ��Э����Ϊ��һʱ��Э����+Q_Noise
  kg3=p_mid3/(p_mid3+R3);                 //kgΪkalman filter��R Ϊ��������
  x_now3=x_mid3+kg3*(*z_measure-x_mid3);  //�����������ֵ
  p_now3=(1-kg3)*p_mid3;                   //����ֵ��Ӧ��covariance
  p_last3=p_now3;                         //����covariance ֵ
  x_last3=x_now3;                            //����ϵͳ״ֵ̬
  *z_measure=(u16)x_now3;
}

//�˵��ִ����
void limitfilter(u16 *measure,u16 refer)
{
  int16_t err=*measure-refer;
  if(err>20||err<20)
  {
    *measure=refer;
  }
}
