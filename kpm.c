// kpm.c 

#include "defines.h"
#include "lcd_defines.h"
#include "delay.h"
#include <lpc21xx.h>
#include "types.h"
#include "lcd.h"
#include "kpm.h"
#include "kpm_defines.h"

u8 KpmLUT[4][4] =
{
    {1,  2,  3,  '/'},
    {4,  5,  6,  '*'},
    {7,  8,  9,  '-'},
    {15, 0,  '=', '+'}
};

void InitKPM(void)
{
    WRITENIBBLE(IODIR1, ROW0, 15);
}

u8 ColScan(void)
{
    if ((READNIBBLE(IOPIN1, COL0)) < 15)
        return 0;
    else
        return 1;
}

u8 RowCheck(void)
{
    u8 rno;

    for (rno = 0; rno <= 3; rno++)
    {
        WRITENIBBLE(IOPIN1, ROW0, ~(1 << rno));

        if (ColScan() == 0)
            break;
    }

    WRITENIBBLE(IOPIN1, ROW0, 0x0);

    return rno;
}

u8 ColCheck(void)
{
    u8 cno;

    for (cno = 0; cno <= 3; cno++)
    {
        if (STATUSBIT(IOPIN1, (COL0 + cno)) == 0)
            break;
    }

    return cno;
}

u8 KeyScan(void)
{
    u8 keyv;
    u8 rno;
    u8 cno;

    while (ColScan())
    {
        ;
    }

    delay_ms(10);

    rno = RowCheck();
    cno = ColCheck();

    keyv = KpmLUT[rno][cno];

    while (!ColScan())
    {
        ;
    }

    delay_ms(10);

    return keyv;
}

static void DisplayInputValue(u32 num, u8 cnt)
{
    u8 digits[4];
    s32 i;

    CmdLCD(GOTO_LINE2_POS0);

    StrLCD((s8 *)"                ");

    CmdLCD(GOTO_LINE2_POS0);

    if (cnt == 0)
        return;

    for (i = (s32)cnt - 1; i >= 0; i--)
    {
        digits[i] = (u8)((num % 10) + '0');
        num /= 10;
    }

    for (i = 0; i < cnt; i++)
        CharLCD(digits[i]);
}

u32 ReadNumber(int max, s8 *label)
{
    s8  k;
    u32 num = 0;
    u8  cnt = 0;

    CmdLCD(GOTO_LINE2_POS0);

    while (1)
    {
        k = KeyScan();

        if (k >= 0 && k <= 9)
        {
            if (cnt < 2)
            {
                num = (num * 10) + k;
                cnt++;

                CharLCD((u8)(k + '0'));
            }
        }
        else if (k == KPM_BACKSPACE)
        {
            if (cnt > 0)
            {
                num /= 10;
                cnt--;

                DisplayInputValue(num, cnt);
            }
        }
        else if (k == '=')
        {
            if (cnt == 0)
            {
                CmdLCD(CLEAR_LCD);
                StrLCD((s8 *)"NO INPUT");

                delay_ms(700);

                CmdLCD(CLEAR_LCD);
                StrLCD((s8 *)"ENTER AGAIN");

                CmdLCD(GOTO_LINE2_POS0);
                continue;
            }

            if (num <= (u32)max)
                return num;

            CmdLCD(CLEAR_LCD);
            StrLCD((s8 *)"INVALID");

            delay_ms(700);

            CmdLCD(CLEAR_LCD);
            StrLCD((s8 *)"TRY AGAIN (");
            StrLCD(label);
            StrLCD((s8 *)")");

            num = 0;
            cnt = 0;

            CmdLCD(GOTO_LINE2_POS0);
        }
    }
}

u32 Read4Number(int max, s8 *label)
{
    s8  k;
    u32 num = 0;
    u8  cnt = 0;

    CmdLCD(GOTO_LINE2_POS0);

    while (1)
    {
        k = KeyScan();

        if (k >= 0 && k <= 9)
        {
            if (cnt < 4)
            {
                num = (num * 10) + k;
                cnt++;

                CharLCD((u8)(k + '0'));
            }
        }
        else if (k == KPM_BACKSPACE)
        {
            if (cnt > 0)
            {
                num /= 10;
                cnt--;

                DisplayInputValue(num, cnt);
            }
        }
        else if (k == '=')
        {
            if ((num <= (u32)max) && (cnt > 0))
                return num;

            CmdLCD(CLEAR_LCD);
            StrLCD((s8 *)"INVALID");

            delay_ms(700);

            CmdLCD(CLEAR_LCD);
            StrLCD((s8 *)"TRY AGAIN (");
            StrLCD(label);
            StrLCD((s8 *)")");

            num = 0;
            cnt = 0;

            CmdLCD(GOTO_LINE2_POS0);
        }
    }
}
