#ifndef DATATRANSFER_H
#define DATATRANSFER_H
#include "main.h"

/**
  * @brief 数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆成
  *        单独字节进行发送
  */
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

#define USART1_SEND

typedef struct{
  int16_t motor_right;
  int16_t motor_left;
  int16_t motor_fan;
  int16_t motor4;
  int16_t motor5;
  int16_t motor6;
  int16_t motor7;
  int16_t motor8;
}MotorData;

typedef struct{
  int16_t dis_front;                             //前超声波，对应地面站ACC_X
  int16_t dis_right;                             //右超声波，对应地面站ACC_Y
  int16_t dis_left;                              //左超声波，对应地面站ACC_Z
  int16_t fire_sensor;                           //火焰传感器，对应地面站GYRO_X
  int16_t wheel_right;                           //右编码器，对应地面站GYRO_Y
  int16_t wheel_left;                            //左编码器，对应地面站GYRO_Z
  int16_t POS_X;                                 //当前位置X坐标，对应地面站MAG_X
  int16_t POS_Y;                                 //当前位置Y坐标，对应地面站MAG_Y
  int16_t MAG_Z;
}SensorData;

extern MotorData __Motordata;
extern SensorData __Sensordata;

void DatatransferTask(void);
void Data_Send(u8 *DataToSend, u8 length);
void ANO_DT_Send_Check(u8 head, u8 check_sum);
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);
void ANO_DT_Send_Power(u16 votage, u16 current);
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8);
void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d);

#endif
