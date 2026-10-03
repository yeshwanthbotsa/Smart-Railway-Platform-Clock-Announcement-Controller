#include <lpc214x.h>

#include "types.h"
#include "status_defines.h"
#include "status.h"
#include "train_db.h"
#include "delay.h"


#define APPROACH_WINDOW_MIN 5


static u8 currentColor = COLOR_NONE;


static u8 beep5Done[TOTAL_TRAINS];

static u8 beep1Done[TOTAL_TRAINS];

static u8 beepAfterDone[TOTAL_TRAINS];


static s32 TimeToSeconds(s32 hour,
                         s32 min,
                         s32 sec)
{
    return ((hour * 3600) +
            (min * 60) +
            sec);
}


static void ResetAnnouncementFlags(void)
{
    u8 i;


    for (i = 0; i < TOTAL_TRAINS; i++)
    {
        beep5Done[i] = 0;

        beep1Done[i] = 0;

        beepAfterDone[i] = 0;
    }
}


void Status_Init(void)
{
    IODIR0 |=
        (1 << LED_GREEN) |
        (1 << LED_YELLOW) |
        (1 << LED_RED) |
        (1 << BUZZER);


    IOCLR0 =
        (1 << LED_GREEN) |
        (1 << LED_YELLOW) |
        (1 << LED_RED) |
        (1 << BUZZER);


    currentColor =
        COLOR_NONE;


    ResetAnnouncementFlags();
}


void SetLED(u8 color)
{
    IOCLR0 =
        (1 << LED_GREEN) |
        (1 << LED_YELLOW) |
        (1 << LED_RED);


    switch (color)
    {
        case COLOR_GREEN:

            IOSET0 =
                (1 << LED_GREEN);

            break;


        case COLOR_YELLOW:

            IOSET0 =
                (1 << LED_YELLOW);

            break;


        case COLOR_RED:

            IOSET0 =
                (1 << LED_RED);

            break;


        default:

            break;
    }
}

void BuzzerOn(void)
{
    IOSET0 =
        (1 << BUZZER);
}


void BuzzerOff(void)
{
    IOCLR0 =
        (1 << BUZZER);
}

void BuzzerBeep(u32 ms)
{
    BuzzerOn();

    delay_ms(ms);

    BuzzerOff();
}


void Status_Reset(void)
{
    currentColor =
        COLOR_NONE;


    SetLED(
        COLOR_NONE
    );


    BuzzerOff();


    ResetAnnouncementFlags();
}

u8 Status_GetColor(void)
{
    return currentColor;
}


static void UpdateBuzzerForTrain(u8 idx,
                                 s32 curHour,
                                 s32 curMin,
                                 s32 curSec)
{
    s32 currentSeconds;

    s32 arrivalSeconds;

    s32 departureSeconds;

    s32 difference;


    currentSeconds =
        TimeToSeconds(
            curHour,
            curMin,
            curSec
        );


    arrivalSeconds =
        TimeToSeconds(
            TrainDB[idx].updatedArrivalHour,
            TrainDB[idx].updatedArrivalMinute,
            0
        );


    departureSeconds =
        TimeToSeconds(
            TrainDB[idx].updatedDepartureHour,
            TrainDB[idx].updatedDepartureMinute,
            0
        );


    difference =
        arrivalSeconds -
        currentSeconds;


    if ((difference <= 300) &&
        (difference > 240) &&
        (beep5Done[idx] == 0))
    {
        BuzzerBeep(250);

        beep5Done[idx] = 1;
    }


    if ((difference <= 60) &&
        (difference > 0) &&
        (beep1Done[idx] == 0))
    {
        BuzzerBeep(250);

        delay_ms(150);

        BuzzerBeep(250);

        delay_ms(150);

        BuzzerBeep(250);


        beep1Done[idx] = 1;
    }


    if ((currentSeconds >= departureSeconds) &&
        (beepAfterDone[idx] == 0))
    {
        BuzzerBeep(250);

        beepAfterDone[idx] = 1;
    }
}

static void UpdateAllBuzzers(s32 curHour,
                             s32 curMin,
                             s32 curSec)
{
    u8 i;


    for (i = 0; i < TOTAL_TRAINS; i++)
    {
        UpdateBuzzerForTrain(
            i,
            curHour,
            curMin,
            curSec
        );
    }
}

void UpdateTrainStatus(s32 trainIndex,
                       s32 curHour,
                       s32 curMin,
                       s32 curSec)
{
    s32 currentSeconds;

    s32 arrivalSeconds;

    s32 minsToArrival;


    UpdateAllBuzzers(
        curHour,
        curMin,
        curSec
    );

    if ((trainIndex < 0) ||
        (trainIndex >= TOTAL_TRAINS))
    {
        SetLED(
            COLOR_NONE
        );

        currentColor =
            COLOR_NONE;

        return;
    }


    currentSeconds =
        TimeToSeconds(
            curHour,
            curMin,
            curSec
        );


    arrivalSeconds =
        TimeToSeconds(
            TrainDB[trainIndex].updatedArrivalHour,
            TrainDB[trainIndex].updatedArrivalMinute,
            0
        );


    minsToArrival =
        (arrivalSeconds -
         currentSeconds) / 60;

    if (TrainDB[trainIndex].delayMinutes > 0)
    {
        currentColor =
            COLOR_RED;
    }

    else if ((minsToArrival >= 0) &&
             (minsToArrival <= APPROACH_WINDOW_MIN))
    {
        currentColor =
            COLOR_YELLOW;
    }

    else
    {
        currentColor =
            COLOR_GREEN;
    }


    SetLED(
        currentColor
    );
}
