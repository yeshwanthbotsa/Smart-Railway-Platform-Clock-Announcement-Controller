#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "rtc.h"
#include "rtc_defines.h"
#include "train_db.h"
#include "status.h"
#include "admin.h"
#include "delay.h"

static void RecalcDelay(u8 idx)
{
    s32 schedTotal;
    s32 updTotal;
    s32 diff;

    schedTotal = (TrainDB[idx].arrivalHour * 60) +
                 TrainDB[idx].arrivalMinute;

    updTotal = (TrainDB[idx].updatedArrivalHour * 60) +
               TrainDB[idx].updatedArrivalMinute;

    diff = updTotal - schedTotal;

    if (diff > 0)
        TrainDB[idx].delayMinutes = (u8)diff;
    else
        TrainDB[idx].delayMinutes = 0;
}

static void CopyString(s8 *dst, s8 *src)
{
    while (*src)
    {
        *dst++ = *src++;
    }

    *dst = '\0';
}

static u8 IsValidTrainTime(u8 arrH,
                           u8 arrM,
                           u8 depH,
                           u8 depM)
{
    s32 arrTotal;
    s32 depTotal;

    arrTotal = (arrH * 60) + arrM;
    depTotal = (depH * 60) + depM;

    if (depTotal < arrTotal)
        return 0;

    return 1;
}

static void SelectDestination(u8 idx)
{
    u8 k;

    while (1)
    {
        CmdLCD(CLEAR_LCD);

        StrLCD((s8 *)"1)New Delhi");

        CmdLCD(GOTO_LINE2_POS0);

        StrLCD((s8 *)"2)Chennai 3)Hyd");

        k = KeyScan();

        if (k == 1)
        {
            CopyString(TrainDB[idx].destination,
                       (s8 *)"New Delhi");

            return;
        }

        else if (k == 2)
        {
            CopyString(TrainDB[idx].destination,
                       (s8 *)"Chennai");

            return;
        }

        else if (k == 3)
        {
            CopyString(TrainDB[idx].destination,
                       (s8 *)"Hyderabad");

            return;
        }

        else
        {
            CmdLCD(CLEAR_LCD);

            StrLCD((s8 *)"Invalid Choice");

            delay_ms(700);
        }
    }
}

static void EditTrainMenu(u8 idx)
{
    u8 k;
    u8 h;
    u8 m;
    u8 changed = 0;

    while (1)
    {
        CmdLCD(CLEAR_LCD);

        StrLCD((s8 *)"1)Arr 2)Dep");

        CmdLCD(GOTO_LINE2_POS0);

        StrLCD((s8 *)"3)Plat 4)Dest");

        k = KeyScan();
         
        if (k == 1)
        {
            CmdLCD(CLEAR_LCD);

            StrLCD((s8 *)"Arr Hour(0-23):");

            h = (u8)ReadNumber(23, (s8 *)"ARRH");


            CmdLCD(CLEAR_LCD);

            StrLCD((s8 *)"Arr Min(0-59):");

            m = (u8)ReadNumber(59, (s8 *)"ARRM");


            if (IsValidTrainTime(
                    h,
                    m,
                    TrainDB[idx].updatedDepartureHour,
                    TrainDB[idx].updatedDepartureMinute))
            {
                TrainDB[idx].updatedArrivalHour   = h;
                TrainDB[idx].updatedArrivalMinute = m;

                RecalcDelay(idx);

                changed = 1;
            }

            else
            {
                CmdLCD(CLEAR_LCD);

                StrLCD((s8 *)"ARR > DEP");

                CmdLCD(GOTO_LINE2_POS0);

                StrLCD((s8 *)"Update rejected");

                delay_ms(1000);
            }
        }
       
        else if (k == 2)
        {
            CmdLCD(CLEAR_LCD);

            StrLCD((s8 *)"Dep Hour(0-23):");

            h = (u8)ReadNumber(23, (s8 *)"DEPH");


            CmdLCD(CLEAR_LCD);

            StrLCD((s8 *)"Dep Min(0-59):");

            m = (u8)ReadNumber(59, (s8 *)"DEPM");


            if (IsValidTrainTime(
                    TrainDB[idx].updatedArrivalHour,
                    TrainDB[idx].updatedArrivalMinute,
                    h,
                    m))
            {
                TrainDB[idx].updatedDepartureHour   = h;
                TrainDB[idx].updatedDepartureMinute = m;

                changed = 1;
            }

            else
            {
                CmdLCD(CLEAR_LCD);

                StrLCD((s8 *)"DEP < ARR");

                CmdLCD(GOTO_LINE2_POS0);

                StrLCD((s8 *)"Update rejected");

                delay_ms(1000);
            }
        }
         
        else if (k == 3)
        {
            CmdLCD(CLEAR_LCD);

            StrLCD((s8 *)"Platform(0-9):");

            TrainDB[idx].platform =
                (u8)ReadNumber(9, (s8 *)"PLAT");

            changed = 1;
        }
        else if (k == 4)
        {
            SelectDestination(idx);

            changed = 1;
        }
 
        else if (k == 5)
        {
            if (changed)
            {
                BuzzerBeep(300);
            }

            return;
        }
    }
}

static void EditRTCTime(void)
{
    u8 hh;
    u8 mm;
    u8 ss;

    CmdLCD(CLEAR_LCD);

    StrLCD((s8 *)"Hour(0-23):");

    hh = (u8)ReadNumber(23, (s8 *)"HOUR");


    CmdLCD(CLEAR_LCD);

    StrLCD((s8 *)"Min(0-59):");

    mm = (u8)ReadNumber(59, (s8 *)"MIN");


    CmdLCD(CLEAR_LCD);

    StrLCD((s8 *)"Sec(0-59):");

    ss = (u8)ReadNumber(59, (s8 *)"SEC");


    SetRTCTimeInfo(hh,
                   mm,
                   ss);
}

static void EditRTCDate(void)
{
    u8 dd;
    u8 mo;
    u32 yy;

    CmdLCD(CLEAR_LCD);

    StrLCD((s8 *)"Date(1-31):");

    dd = (u8)ReadNumber(31, (s8 *)"DATE");


    CmdLCD(CLEAR_LCD);

    StrLCD((s8 *)"Month(1-12):");

    mo = (u8)ReadNumber(12, (s8 *)"MON");


    CmdLCD(CLEAR_LCD);

    StrLCD((s8 *)"Year(1-9999):");

    yy = Read4Number(9999, (s8 *)"YEAR");


    SetRTCDateInfo(dd,
                   mo,
                   yy);
}

static void EditRTCDay(void)
{
    u8 dow;

    CmdLCD(CLEAR_LCD);

    StrLCD((s8 *)"Day(0-6):");

    dow = (u8)ReadNumber(6, (s8 *)"DAY");

    SetRTCDay(dow);
}

static void EditRTCMenu(void)
{
    u8 k;

    while (1)
    {
        CmdLCD(CLEAR_LCD);

        StrLCD((s8 *)"1)Time 2)Date");

        CmdLCD(GOTO_LINE2_POS0);

        StrLCD((s8 *)"3)Day 4)Back");

        k = KeyScan();


         // 1 = EDIT TIME
        if (k == 1)
        {
            EditRTCTime();
        }


         // 2 = EDIT DATE
        else if (k == 2)
        {
            EditRTCDate();
        }


         // 3 = EDIT DAY
        else if (k == 3)
        {
            EditRTCDay();
        }


         // 4 = BACK TO ADMIN MENU
        else if (k == 4)
        {
            return;
        }
    }
}
static u8 AdminAuthenticate(void)
{
    u8 attempt;
    u32 entered;

    for (attempt = 0; attempt < ADMIN_MAX_ATTEMPTS; attempt++)
    {
        CmdLCD(CLEAR_LCD);

        StrLCD((s8 *)"Enter Admin PIN:");

        entered = Read4Number(9999, (s8 *)"PIN");

        if (entered == (u32)ADMIN_PIN)
        {
            return 1;
        }

        CmdLCD(CLEAR_LCD);

        StrLCD((s8 *)"Wrong PIN");

        delay_ms(800);
    }

    return 0;
}

void AdminMode(void)
{
    u8 k;
    u8 trainIdx;
    CmdLCD(CLEAR_LCD);

    if (!AdminAuthenticate())
    {
        CmdLCD(CLEAR_LCD);

        StrLCD((s8 *)"Access Denied");

        delay_ms(1000);

        return;
    }


    while (1)
    {
        CmdLCD(CLEAR_LCD);

        StrLCD((s8 *)"1)Train 2)RTC");

        CmdLCD(GOTO_LINE2_POS0);

        StrLCD((s8 *)"3)Exit");


        k = KeyScan();

        if (k == 1)
        {
            CmdLCD(CLEAR_LCD);

            StrLCD((s8 *)"Train no(1-3):");

            trainIdx = KeyScan();


            if ((trainIdx >= 1) &&
                (trainIdx <= TOTAL_TRAINS))
            {
                EditTrainMenu(trainIdx - 1);
            }

            else
            {
                CmdLCD(CLEAR_LCD);

                StrLCD((s8 *)"Invalid Train");

                delay_ms(1000);
            }
        }
        else if (k == 2)
        {
            EditRTCMenu();
        }
        else if (k == 3)
        {
            CmdLCD(CLEAR_LCD);

            StrLCD((s8 *)"Admin Exiting");

            delay_ms(500);

            return;
        }
    }
}
