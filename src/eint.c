#include <lpc214x.h>

#include "types.h"
#include "eint_defines.h"
#include "eint.h"

volatile u8 AdminEditRequested = 0;
static void __irq EINT1_ISR(void)
{
    AdminEditRequested = 1;
    EXTINT |= (1 << 1);
    VICVectAddr = 0;
}

static void EnableGlobalIRQ(void)
{
    unsigned int reg;
    __asm
    {
        MRS reg, CPSR
        BIC reg, reg, #0x80
        MSR CPSR_c, reg
    }
}
void EINT_Init(void)
{
    PINSEL0 &= ~(0x3 << 6);
    PINSEL0 |=  (0x3 << 6);

    EXTMODE |= (1 << 1);

    EXTPOLAR &= ~(1 << 1);

    EXTINT |= (1 << 1);
	
    VICIntSelect &= ~(1 << EINT1_VIC_CHANNEL);

    VICVectCntl1 =
        0x20 | EINT1_VIC_CHANNEL;

    VICVectAddr1 =
        (unsigned long)EINT1_ISR;

    VICIntEnable |=
        (1 << EINT1_VIC_CHANNEL);


    EnableGlobalIRQ();
}
