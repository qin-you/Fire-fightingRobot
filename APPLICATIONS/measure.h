#ifndef MEASURE_H
#define MEASURE_H
#include "main.h"

typedef struct{
  int16_t left;
  int16_t right;
  int16_t front;
}Distance;

extern Distance distance;

void Get_Distance_Left(void);
void Get_Distance_Right(void);
void Get_Distance_Front(void);
#endif
