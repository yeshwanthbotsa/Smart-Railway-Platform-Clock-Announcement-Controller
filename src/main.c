/* FILE: main.c */

#include <lpc214x.h>

#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"
#include "kpm.h"
#include "rtc.h"
#include "rtc_defines.h"
#include "status.h"
#include "eint.h"
#include "admin.h"
#include "train_db.h"

#define LOOP_DELAY_MS       250
#define WELCOME_LOOPS       (15000 / LOOP_DELAY_MS)
#define RTC_SCREEN_LOOPS    (3000 / LOOP_DELAY_MS)
#define LINE2_PHASE_LOOPS   (5000 / LOOP_DELAY_MS)

#define LINE1_SCROLL_W      10
#define SCROLL_BUF_MAX      64

#define DISPLAY_LEAD_SECONDS 3600

typedef enum
{
    DISP_WELCOME,
    DISP_RTC,
    DISP_NORMAL

} DisplayState_t;

static u8 StrLenLocal(s8 *str)
{
    u8 len = 0;

    while (str[len] != '\0')
        len++;

    return len;
}

static void AppendChar(s8 *buf, u8 *idx, s8 c)
{
    buf[*idx] = c;
    (*idx)++;
}

static void AppendStr(s8 *buf, u8 *idx, s8 *src)
{
    while (*src)
    {
        AppendChar(buf, idx, *src);
        src++;
    }
}

static void ScrollWindow(s8 *buf,
                         u8 offset,
                         u8 width)
{
    u8 len = StrLenLocal(buf);
    u8 i;

    if (len == 0)
        return;

    for (i = 0; i < width; i++)
    {
        CharLCD((u8)buf[(offset + i) % len]);
    }
}

static void ShowWelcome(u8 scrollOffset)
{
    s8 scrollBuf[SCROLL_BUF_MAX];
    u8 i = 0;

    CmdLCD(GOTO_LINE1_POS0);
    StrLCD((s8 *)"   Welcome To   ");

    AppendStr(scrollBuf,
              &i,
              (s8 *)"Smart Railway Platform Clock Announcement Controller");

    AppendStr(scrollBuf,
              &i,
              (s8 *)"      ");

    scrollBuf[i] = '\0';

    CmdLCD(GOTO_LINE2_POS0);

    ScrollWindow(scrollBuf,
                 scrollOffset,
                 16);
}

static void ShowRTCScreen(s32 hour,
                          s32 min,
                          s32 sec,
                          s32 date,
                          s32 month,
                          s32 year,
                          s32 dow)
{
    CmdLCD(GOTO_LINE1_POS0);

    CharLCD((u8)((hour / 10) + '0'));
    CharLCD((u8)((hour % 10) + '0'));

    CharLCD(':');

    CharLCD((u8)((min / 10) + '0'));
    CharLCD((u8)((min % 10) + '0'));

    CharLCD(':');

    CharLCD((u8)((sec / 10) + '0'));
    CharLCD((u8)((sec % 10) + '0'));

    CharLCD(' ');

    if ((dow >= SUN) &&
        (dow <= SAT))
    {
        StrLCD(week[dow]);
    }
    else
    {
        StrLCD((s8 *)"---");
    }

    StrLCD((s8 *)"    ");

    CmdLCD(GOTO_LINE2_POS0);

    CharLCD((u8)((date / 10) + '0'));
    CharLCD((u8)((date % 10) + '0'));

    CharLCD('/');

    CharLCD((u8)((month / 10) + '0'));
    CharLCD((u8)((month % 10) + '0'));

    CharLCD('/');

    CharLCD((u8)(((year / 1000) % 10) + '0'));
    CharLCD((u8)(((year / 100)  % 10) + '0'));
    CharLCD((u8)(((year / 10)   % 10) + '0'));
    CharLCD((u8)((year % 10) + '0'));

    StrLCD((s8 *)"      ");
}
static s32 FindDisplayTrain(s32 curHour,
                            s32 curMin,
                            s32 curSec)
{
    s32 currentSeconds;
    s32 arrivalSeconds;
    s32 departureSeconds;
    s32 displayStart;

    s32 bestArrival = 2147483647L;
    s32 bestIndex   = -1;

    u8 i;

    currentSeconds = (curHour * 3600) +
                     (curMin * 60) +
                     curSec;

    for (i = 0; i < TOTAL_TRAINS; i++)
    {
        arrivalSeconds =
            (TrainDB[i].updatedArrivalHour * 3600) +
            (TrainDB[i].updatedArrivalMinute * 60);

        departureSeconds =
            (TrainDB[i].updatedDepartureHour * 3600) +
            (TrainDB[i].updatedDepartureMinute * 60);

        displayStart = arrivalSeconds - DISPLAY_LEAD_SECONDS;

        if (displayStart < 0)
        {
            displayStart += 86400;
        }

        if ((currentSeconds >= displayStart) &&
            (currentSeconds <= departureSeconds))
        {
            if (arrivalSeconds < bestArrival)
            {
                bestArrival = arrivalSeconds;
                bestIndex   = (s32)i;
            }
        }
    }

    return bestIndex;
}

static void ShowLine1Normal(s32 displayIdx,
                            s32 nextIdx,
                            u8 scrollOffset)
{
    s8 scrollBuf[SCROLL_BUF_MAX];
    u8 i = 0;

    CmdLCD(GOTO_LINE1_POS0);

    if (displayIdx >= 0)
    {
        u32 num = TrainDB[displayIdx].trainNumber;
        u32 div = 10000;

        while (div >= 1)
        {
            CharLCD((u8)(((num / div) % 10) + '0'));
            div /= 10;
        }

        CharLCD(':');

        AppendStr(scrollBuf,
                  &i,
                  TrainDB[displayIdx].trainName);

        AppendStr(scrollBuf,
                  &i,
                  (s8 *)" - ");

        AppendStr(scrollBuf,
                  &i,
                  TrainDB[displayIdx].destination);

        AppendStr(scrollBuf,
                  &i,
                  (s8 *)"      ");
    }
    else if (nextIdx < 0)
    {
        StrLCD((s8 *)"-----:");

        AppendStr(scrollBuf,
                  &i,
                  (s8 *)"NO MORE TRAINS TODAY      ");
    }
    else
    {
        StrLCD((s8 *)"OOPS!:");

        AppendStr(scrollBuf,
                  &i,
                  (s8 *)"WAITING FOR NEXT TRAIN ");
    }

    scrollBuf[i] = '\0';

    ScrollWindow(scrollBuf,
                 scrollOffset,
                 LINE1_SCROLL_W);
}

static void ShowLine2ArrDep(s32 displayIdx,
                            s32 nextIdx)
{
    CmdLCD(GOTO_LINE2_POS0);

    if (displayIdx < 0)
    {
        if (nextIdx < 0)
        {
            StrLCD((s8 *)"NO MORE TRAINS  ");
        }
        else
        {
            StrLCD((s8 *)"WAITING...      ");
        }

        return;
    }

    CharLCD('P');
    U32LCD(TrainDB[displayIdx].platform);

    CharLCD(' ');

    CharLCD('A');

    CharLCD((u8)((TrainDB[displayIdx].updatedArrivalHour / 10) + '0'));
    CharLCD((u8)((TrainDB[displayIdx].updatedArrivalHour % 10) + '0'));

    CharLCD(':');

    CharLCD((u8)((TrainDB[displayIdx].updatedArrivalMinute / 10) + '0'));
    CharLCD((u8)((TrainDB[displayIdx].updatedArrivalMinute % 10) + '0'));

    CharLCD(' ');

    CharLCD('D');

    CharLCD((u8)((TrainDB[displayIdx].updatedDepartureHour / 10) + '0'));
    CharLCD((u8)((TrainDB[displayIdx].updatedDepartureHour % 10) + '0'));

    CharLCD(':');

    CharLCD((u8)((TrainDB[displayIdx].updatedDepartureMinute / 10) + '0'));
    CharLCD((u8)((TrainDB[displayIdx].updatedDepartureMinute % 10) + '0'));
}

static void ShowLine2TimeCentered(s32 hour,
                                  s32 min,
                                  s32 sec)
{
    CmdLCD(GOTO_LINE2_POS0);

    StrLCD((s8 *)"    ");

    CharLCD((u8)((hour / 10) + '0'));
    CharLCD((u8)((hour % 10) + '0'));

    CharLCD(':');

    CharLCD((u8)((min / 10) + '0'));
    CharLCD((u8)((min % 10) + '0'));

    CharLCD(':');

    CharLCD((u8)((sec / 10) + '0'));
    CharLCD((u8)((sec % 10) + '0'));

    StrLCD((s8 *)"    ");
}

static void ShowLine2Status(s32 displayIdx,
                            s32 nextIdx)
{
    u8 color;

    CmdLCD(GOTO_LINE2_POS0);

    if (displayIdx < 0)
    {
        if (nextIdx < 0)
        {
            StrLCD((s8 *)"NO MORE TRAINS  ");
        }
        else
        {
            StrLCD((s8 *)"STATUS: WAITING ");
        }

        return;
    }

    color = Status_GetColor();

    switch (color)
    {
        case COLOR_GREEN:
            StrLCD((s8 *)"STATUS: ONTIME  ");
            break;

        case COLOR_YELLOW:
            StrLCD((s8 *)"STATUS: APPROACH");
            break;

        case COLOR_RED:
        {
            u32 delayVal = (u32)TrainDB[displayIdx].delayMinutes;
            u32 tmp      = delayVal;
            u8  digits   = 1;
            u8  written;

            StrLCD((s8 *)"STATUS:DELAY ");

            U32LCD(delayVal);

            while (tmp >= 10)
            {
                tmp /= 10;
                digits++;
            }

            CharLCD('m');
            written = (u8)(13 + digits + 1);

            while (written < 16)
            {
                CharLCD(' ');
                written++;
            }

            break;
        }

        default:
            StrLCD((s8 *)"STATUS: UNKNOWN ");
            break;
    }
}

int main(void)
{
    s32 hour;
    s32 min;
    s32 sec;

    s32 date;
    s32 month;
    s32 year;
    s32 dow;

    s32 idx;
    s32 displayIdx;

    DisplayState_t dispState = DISP_WELCOME;

    u32 stateLoopCounter = 0;

    u8 scrollOffsetWelcome = 0;
    u8 scrollOffset1 = 0;

    u8 line2Counter = 0;

    RTC_Init();
    InitLCD();
    InitKPM();
    Status_Init();
    EINT_Init();

    SetRTCTimeInfo(0, 0, 0);
    SetRTCDateInfo(0, 9, 2026);
    SetRTCDay(TUE);

    CmdLCD(CLEAR_LCD);

    while (1)
    {
        if (AdminEditRequested)
        {
            AdminEditRequested = 0;

            AdminMode();

            CmdLCD(CLEAR_LCD);

            Status_Reset();

            dispState = DISP_NORMAL;

            stateLoopCounter = 0;
            scrollOffset1 = 0;
            line2Counter = 0;
        }

        GetRTCTimeInfo(&hour,
                       &min,
                       &sec);

        GetRTCDateInfo(&date,
                       &month,
                       &year);

        GetRTCDay(&dow);

        idx = FindNextTrainIndex(hour,
                                 min);

        
        displayIdx = FindDisplayTrain(hour,
                                      min,
                                      sec);

        UpdateTrainStatus(idx,
                          hour,
                          min,
                          sec);

        switch (dispState)
        {
            case DISP_WELCOME:

                ShowWelcome(scrollOffsetWelcome);

                scrollOffsetWelcome++;

                stateLoopCounter++;

                if (stateLoopCounter >= WELCOME_LOOPS)
                {
                    dispState = DISP_RTC;
                    stateLoopCounter = 0;
                }

                break;

            case DISP_RTC:

                ShowRTCScreen(hour,
                              min,
                              sec,
                              date,
                              month,
                              year,
                              dow);

                stateLoopCounter++;

                if (stateLoopCounter >= RTC_SCREEN_LOOPS)
                {
                    dispState = DISP_NORMAL;

                    stateLoopCounter = 0;
                    scrollOffset1 = 0;
                    line2Counter = 0;
                }

                break;

            case DISP_NORMAL:

            default:

                ShowLine1Normal(displayIdx,
                                idx,
                                scrollOffset1);

                scrollOffset1++;
                
                if (line2Counter < LINE2_PHASE_LOOPS)
                {
                    ShowLine2ArrDep(displayIdx,
                                    idx);
                }
                else if (line2Counter <
                         (LINE2_PHASE_LOOPS * 2))
                {
                    ShowLine2TimeCentered(hour,
                                          min,
                                          sec);
                }
                else
                {
                    ShowLine2Status(displayIdx,
                                    idx);
                }

                line2Counter++;

                if (line2Counter >=
                    (LINE2_PHASE_LOOPS * 3))
                {
                    line2Counter = 0;
                }

                break;
        }

        delay_ms(LOOP_DELAY_MS);
    }
}
