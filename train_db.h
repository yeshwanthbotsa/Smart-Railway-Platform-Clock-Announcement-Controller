#ifndef TRAIN_DB_H
#define TRAIN_DB_H

#include "types.h"

#define TOTAL_TRAINS 3

typedef struct
{
    u32 trainNumber;
    s8  trainName[25];
    s8  destination[20];

    u8 arrivalHour;
    u8 arrivalMinute;
    u8 departureHour;
    u8 departureMinute;

    u8 updatedArrivalHour;
    u8 updatedArrivalMinute;
    u8 updatedDepartureHour;
    u8 updatedDepartureMinute;

    u8 platform;
    u8 delayMinutes;
} TrainInfo_t;

extern TrainInfo_t TrainDB[TOTAL_TRAINS];

s32 FindNextTrainIndex(s32 curHour, s32 curMin);

s32 GetMinutesToArrival(u8 idx, s32 curHour, s32 curMin);

#endif 
