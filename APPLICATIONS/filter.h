#ifndef FILTER_H
#define FILTER_H
#include "main.h"

#define UPPER_LIMIT 500
#define MeasureNoise_R 0.11
#define ProcessNiose_Q 0.03
void kalmanfilter_r(u16 *z_measure);
void limitfilter(u16 *measure,u16 refer);
void kalmanfilter_l(u16 *z_measure);
void kalmanfilter_f(u16 *z_measure);
#endif
