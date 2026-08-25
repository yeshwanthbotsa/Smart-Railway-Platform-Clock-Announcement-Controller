#include "delay.h"

#define LOOPS_PER_US   6

void delay_us(unsigned int us)
{
    volatile unsigned int i;
    while (us--)
    {
        for (i = 0; i < LOOPS_PER_US; i++);
    }
}

void delay_ms(unsigned int ms)
{
    while (ms--)
    {
        delay_us(1000);
    }
}
