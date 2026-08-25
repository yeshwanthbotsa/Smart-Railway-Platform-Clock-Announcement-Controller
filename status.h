// status.h 

#ifndef STATUS_H
#define STATUS_H

#include "types.h"

#define COLOR_NONE    0
#define COLOR_GREEN   1
#define COLOR_YELLOW  2
#define COLOR_RED     3

void Status_Init(void);
void Status_Reset(void);

void SetLED(u8 color);
u8   Status_GetColor(void);

void BuzzerOn(void);
void BuzzerOff(void);
void BuzzerBeep(u32 ms);

void UpdateTrainStatus(s32 trainIndex,
                       s32 curHour,
                       s32 curMin,
                       s32 curSec);

#endif 
