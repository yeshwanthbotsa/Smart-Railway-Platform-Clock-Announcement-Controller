// train_db.c */

#include "types.h"
#include "train_db.h"

TrainInfo_t TrainDB[TOTAL_TRAINS] =
{
    {
        12627,
        "Karnataka Express",
        "New Delhi",

        6, 30,
        6, 35,

        6, 30,
        6, 35,

        1,
        0
    },

    {
        12028,
        "Shatabdi Express",
        "Chennai",

        7, 15,
        7, 20,

        7, 15,
        7, 20,

        2,
        0
    },

    {
        12785,
        "Kacheguda Express",
        "Hyderabad",

        8, 00,
        8, 05,

        8, 20,
        8, 25,

        3,
        20
    }
};

s32 GetMinutesToArrival(u8 idx, s32 curHour, s32 curMin)
{
    s32 curTotal;
    s32 arrTotal;

    curTotal = (curHour * 60) + curMin;

    arrTotal = (TrainDB[idx].updatedArrivalHour * 60) +
               TrainDB[idx].updatedArrivalMinute;

    return arrTotal - curTotal;
}

s32 FindNextTrainIndex(s32 curHour, s32 curMin)
{
    s32 curTotal;
    s32 depTotal;
    s32 bestDepTotal = 1441;
    s32 bestIndex = -1;

    u8 i;

    curTotal = (curHour * 60) + curMin;

    for (i = 0; i < TOTAL_TRAINS; i++)
    {
        depTotal = (TrainDB[i].updatedDepartureHour * 60) +
                   TrainDB[i].updatedDepartureMinute;

        if ((depTotal >= curTotal) &&
            (depTotal < bestDepTotal))
        {
            bestDepTotal = depTotal;
            bestIndex = (s32)i;
        }
    }

    return bestIndex;
}
