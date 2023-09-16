#ifndef PID_H
#define PID_H
#include "main.h"


typedef struct{
  float P,I,D,FF,MaxError;
}PIDParams;

typedef struct{
  float Error,Target;
}PIDState;

typedef struct{
  int8_t ne_min;
  int8_t ne_two;
  int8_t ne_one;
  int8_t zero;
  int8_t one;
  int8_t two;
  int8_t max;
}FuzzyParam;

typedef struct{
  int8_t ne_min;
  int8_t ne_two;
  int8_t ne_one;
  int8_t zero;
  int8_t one;
  int8_t two;
  int8_t max;
}FuzzyD;

void Speed_Control(void);
void Pos_ControlRight(void);
void Pos_ControlLeft(void);
#endif
