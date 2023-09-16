#include "main.h"


u8 data_to_send[50];                                  //发送数据缓存
MotorData __Motordata={0,0,0,0,0,0,0,0};
SensorData __Sensordata={0,0,0,0,0,0,0,0,0};

/**
  * @brief 协议中所有发送数据功能使用到的发送函数
  */
static void Data_Send(u8 *DataToSend, u8 length)
{
#ifdef USART1_SEND
  Usart1_Send(DataToSend, length);
#else
  Usart2_Send(DataToSend, length);
#endif

}

/**
  * @brief 将数据传回地面站，以方便调试
  * @param 
  * @retval None
  * @details 传输电机数据，传感器数据
  */
void DatatransferTask(void)
{
  static u16 flag=0;
  flag++;
  if(flag%2==1)
  {
    ANO_DT_Send_MotoPWM(__Motordata.motor_right,
                        __Motordata.motor_left ,
                        __Motordata.motor_fan  ,
                        __Motordata.motor4     ,
                        __Motordata.motor5     ,
                        __Motordata.motor6     ,
                        __Motordata.motor7     ,
                        __Motordata.motor8
                        );
  }
  else if(flag%2==0)
  {
    ANO_DT_Send_Senser(__Sensordata.dis_front  ,
                      __Sensordata.dis_right  ,
                      __Sensordata.dis_left   ,
                      __Sensordata.fire_sensor,
                      __Sensordata.wheel_left ,
                      __Sensordata.wheel_right,
                      __Sensordata.POS_X      ,
                      __Sensordata.POS_Y      ,
                      __Sensordata.MAG_Z      
                    );
  }

}


#if 0
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Prepare函数是协议预解析，根据协议的格式，将收到的数据进行一次格式性解析，格式正确的话再进行数据解析
//移植时，此函数应由用户根据自身使用的通信方式自行调用，比如串口每收到一字节数据，则调用此函数一次
//此函数解析出符合格式的数据帧后，会自行调用数据解析函数
void ANO_DT_Data_Receive_Prepare(u8 data)
{
  static u8 RxBuffer[50];
  static u8 _data_len = 0,_data_cnt = 0;
  static u8 state = 0;
  
  if(state==0&&data==0xAA)
  {
    state=1;
    RxBuffer[0]=data;
  }
  else if(state==1&&data==0xAF)
  {
    state=2;
    RxBuffer[1]=data;
  }
  else if(state==2&&data<0XF1)
  {
    state=3;
    RxBuffer[2]=data;
  }
  else if(state==3&&data<50)
  {
    state = 4;
    RxBuffer[3]=data;
    _data_len = data;
    _data_cnt = 0;
  }
  else if(state==4&&_data_len>0)
  {
    _data_len--;
    RxBuffer[4+_data_cnt++]=data;
    if(_data_len==0)
      state = 5;
  }
  else if(state==5)
  {
    state = 0;
    RxBuffer[4+_data_cnt]=data;
    ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
  }
  else
    state = 0;
}
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl函数是协议数据解析函数，函数参数是符合协议格式的一个数据帧，该函数会首先对协议数据进行校验
//校验通过后对数据进行解析，实现相应功能
//此函数可以不用用户自行调用，由函数Data_Receive_Prepare自动调用
void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num)
{
  u8 sum = 0;
  for(u8 i=0;i<(num-1);i++)
    sum += *(data_buf+i);
  if(!(sum==*(data_buf+num-1)))		return;		//判断sum
  if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头
  
  if(*(data_buf+2)==0X01)
  {
    if(*(data_buf+4)==0X01)
      mpu6050.Acc_CALIBRATE = 1;
    if(*(data_buf+4)==0X02)
      mpu6050.Gyro_CALIBRATE = 1;
    if(*(data_buf+4)==0X03)
    {
      mpu6050.Acc_CALIBRATE = 1;		
      mpu6050.Gyro_CALIBRATE = 1;			
    }
  }
  
  if(*(data_buf+2)==0X02)
  {
    if(*(data_buf+4)==0X01)
    {
      f.send_pid1 = 1;
      f.send_pid2 = 1;
      f.send_pid3 = 1;
      f.send_pid4 = 1;
      f.send_pid5 = 1;
      f.send_pid6 = 1;
    }
    if(*(data_buf+4)==0X02)
    {
      
    }
    if(*(data_buf+4)==0XA0)		//读取版本信息
    {
      f.send_version = 1;
    }
    if(*(data_buf+4)==0XA1)		//恢复默认参数
    {
      Para_ResetToFactorySetup();
    }
  }

  if(*(data_buf+2)==0X10)								//PID1
    {
        ctrl_1.PID[PIDROLL].kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        ctrl_1.PID[PIDROLL].ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        ctrl_1.PID[PIDROLL].kd  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        ctrl_1.PID[PIDPITCH].kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        ctrl_1.PID[PIDPITCH].ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        ctrl_1.PID[PIDPITCH].kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        ctrl_1.PID[PIDYAW].kp 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        ctrl_1.PID[PIDYAW].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        ctrl_1.PID[PIDYAW].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);
        Param_SavePID();
    }
    if(*(data_buf+2)==0X11)								//PID2
    {
        ctrl_1.PID[PID4].kp 	= 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        ctrl_1.PID[PID4].ki 	= 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        ctrl_1.PID[PID4].kd 	= 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        ctrl_1.PID[PID5].kp 	= 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        ctrl_1.PID[PID5].ki 	= 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        ctrl_1.PID[PID5].kd 	= 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        ctrl_1.PID[PID6].kp	  = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        ctrl_1.PID[PID6].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        ctrl_1.PID[PID6].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);
        Param_SavePID();
    }
    if(*(data_buf+2)==0X12)								//PID3
    {	
        ctrl_2.PID[PIDROLL].kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
        ctrl_2.PID[PIDROLL].ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
        ctrl_2.PID[PIDROLL].kd  = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
        ctrl_2.PID[PIDPITCH].kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
        ctrl_2.PID[PIDPITCH].ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
        ctrl_2.PID[PIDPITCH].kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
        ctrl_2.PID[PIDYAW].kp 	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
        ctrl_2.PID[PIDYAW].ki 	= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
        ctrl_2.PID[PIDYAW].kd 	= 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);
        Param_SavePID();
    }
  if(*(data_buf+2)==0X13)								//PID4
  {
    ANO_DT_Send_Check(*(data_buf+2),sum);
  }
  if(*(data_buf+2)==0X14)								//PID5
  {
    ANO_DT_Send_Check(*(data_buf+2),sum);
  }
  if(*(data_buf+2)==0X15)								//PID6
  {
    ANO_DT_Send_Check(*(data_buf+2),sum);
  }
}
#endif

/**
  *
  */
void ANO_DT_Send_Check(u8 head, u8 check_sum)
{
  data_to_send[0]=0xAA;
  data_to_send[1]=0xAA;
  data_to_send[2]=0xEF;
  data_to_send[3]=2;
  data_to_send[4]=head;
  data_to_send[5]=check_sum;
  
  
  u8 sum = 0;
  for(u8 i=0;i<6;i++)
    sum += data_to_send[i];
  data_to_send[6]=sum;

  Data_Send(data_to_send, 7);
}

void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
  u8 _cnt=0;
  vs16 _temp;
  vs32 _temp2 = alt;
  
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x01;
  data_to_send[_cnt++]=0;
  
  _temp = (int)(angle_rol*100);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int)(angle_pit*100);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int)(angle_yaw*100);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  data_to_send[_cnt++]=BYTE3(_temp2);
  data_to_send[_cnt++]=BYTE2(_temp2);
  data_to_send[_cnt++]=BYTE1(_temp2);
  data_to_send[_cnt++]=BYTE0(_temp2);

  data_to_send[_cnt++] = fly_model;

  data_to_send[_cnt++] = armed;

  data_to_send[3] = _cnt-4;

  u8 sum = 0;
  for(u8 i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;

  Data_Send(data_to_send, _cnt);
}

void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
  u8 _cnt=0;
  vs16 _temp;
  
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x02;
  data_to_send[_cnt++]=0;
  
  _temp = a_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = a_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = a_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  _temp = g_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = g_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = g_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);

  _temp = m_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = m_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = m_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  data_to_send[3] = _cnt-4;
  
  u8 sum = 0;
  for(u8 i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++] = sum;

  Data_Send(data_to_send, _cnt);
}

void ANO_DT_Send_Power(u16 votage, u16 current)
{
  u8 _cnt=0;
  u16 temp;
  
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x05;
  data_to_send[_cnt++]=0;
  
  temp = votage;
  data_to_send[_cnt++]=BYTE1(temp);
  data_to_send[_cnt++]=BYTE0(temp);
  temp = current;
  data_to_send[_cnt++]=BYTE1(temp);
  data_to_send[_cnt++]=BYTE0(temp);
  
  data_to_send[3] = _cnt-4;
  
  u8 sum = 0;
  for(u8 i=0;i<_cnt;i++)
    sum += data_to_send[i];
  
  data_to_send[_cnt++]=sum;
  
  Data_Send(data_to_send, _cnt);
}

void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8)
{
  u8 _cnt=0;
  
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x06;
  data_to_send[_cnt++]=0;
  
  data_to_send[_cnt++]=BYTE1(m_1);
  data_to_send[_cnt++]=BYTE0(m_1);
  data_to_send[_cnt++]=BYTE1(m_2);
  data_to_send[_cnt++]=BYTE0(m_2);
  data_to_send[_cnt++]=BYTE1(m_3);
  data_to_send[_cnt++]=BYTE0(m_3);
  data_to_send[_cnt++]=BYTE1(m_4);
  data_to_send[_cnt++]=BYTE0(m_4);
  data_to_send[_cnt++]=BYTE1(m_5);
  data_to_send[_cnt++]=BYTE0(m_5);
  data_to_send[_cnt++]=BYTE1(m_6);
  data_to_send[_cnt++]=BYTE0(m_6);
  data_to_send[_cnt++]=BYTE1(m_7);
  data_to_send[_cnt++]=BYTE0(m_7);
  data_to_send[_cnt++]=BYTE1(m_8);
  data_to_send[_cnt++]=BYTE0(m_8);

  data_to_send[3] = _cnt-4;
  
  u8 sum = 0;
  for(u8 i=0;i<_cnt;i++)
    sum += data_to_send[i];
  
  data_to_send[_cnt++]=sum;
  
  Data_Send(data_to_send, _cnt);
}

void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
  u8 _cnt=0;
  vs16 _temp;
  
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x10+group-1;
  data_to_send[_cnt++]=0;
  
  
  _temp = p1_p * 1000;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = p1_i  * 1000;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = p1_d  * 1000;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = p2_p  * 1000;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = p2_i  * 1000;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = p2_d * 1000;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = p3_p  * 1000;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = p3_i  * 1000;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = p3_d * 1000;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  data_to_send[3] = _cnt-4;
  
  u8 sum = 0;
  for(u8 i=0;i<_cnt;i++)
    sum += data_to_send[i];
  
  data_to_send[_cnt++]=sum;

  Data_Send(data_to_send, _cnt);
}
