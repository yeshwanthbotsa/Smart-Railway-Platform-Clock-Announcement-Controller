#include <lpc214x.h>
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "rtc_defines.h"
#include "rtc.h"


s8 week[7][4] =
{
    "SUN",
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT"
};

void RTC_Init(void)
{
    PCONP |= (1 << 9);
    CCR = RTC_CLKSRC;

    CCR = RTC_ENABLE | RTC_CLKSRC;
    CIIR = 0;
    AMR = 0xFF;
}

void GetRTCTimeInfo(s32 *hour,
                    s32 *minute,
                    s32 *second)
{
    *hour   = HOUR & 0x1F;
    *minute = MIN & 0x3F;
    *second = SEC & 0x3F;
}

void DisplayRTCTime(u32 hour,
                    u32 minute,
                    u32 second)
{
    CmdLCD(GOTO_LINE1_POS0);

    CharLCD((u8)(hour / 10) + '0');
    CharLCD((u8)(hour % 10) + '0');

    CharLCD(':');

    CharLCD((u8)(minute / 10) + '0');
    CharLCD((u8)(minute % 10) + '0');

    CharLCD(':');

    CharLCD((u8)(second / 10) + '0');
    CharLCD((u8)(second % 10) + '0');
}

void SetRTCTimeInfo(u32 hour,
                    u32 minute,
                    u32 second)
{
    
    CCR &= ~RTC_ENABLE;

    HOUR = hour & 0x1F;
    MIN  = minute & 0x3F;
    SEC  = second & 0x3F;

    
    CCR = RTC_ENABLE | RTC_CLKSRC;
}

void GetRTCDateInfo(s32 *date,
                    s32 *month,
                    s32 *year)
{
    *date  = DOM;
    *month = MONTH;
    *year  = YEAR;
}


void DisplayRTCDate(u32 date,
                    u32 month,
                    u32 year)
{
    CmdLCD(GOTO_LINE2_POS0);

    CharLCD((u8)(date / 10) + '0');
    CharLCD((u8)(date % 10) + '0');

    CharLCD('/');

    CharLCD((u8)(month / 10) + '0');
    CharLCD((u8)(month % 10) + '0');

    CharLCD('/');

    U32LCD(year);
}

void SetRTCDateInfo(u32 date,
                    u32 month,
                    u32 year)
{
    CCR &= ~RTC_ENABLE;

    DOM   = date;
    MONTH = month;
    YEAR  = year;

    CCR = RTC_ENABLE | RTC_CLKSRC;
}

void GetRTCDay(s32 *dow)
{
    *dow = DOW;
}

void DisplayRTCDay(u32 day)
{
    CmdLCD(GOTO_LINE1_POS0 + 10);

    StrLCD(week[day]);
}


void SetRTCDay(u32 dow)
{
    CCR &= ~RTC_ENABLE;

    DOW = dow;

    CCR = RTC_ENABLE | RTC_CLKSRC;
}
