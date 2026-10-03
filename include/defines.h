
#ifndef DEFINES_H
#define DEFINES_H

#define STATUSBIT(reg, bit)     (((reg) >> (bit)) & 0x1UL)
#define WRITENIBBLE(reg, pos, val) \
        ((reg) = ((reg) & ~(0xFUL << (pos))) | (((val) & 0xFUL) << (pos)))
#define READNIBBLE(reg, pos)     (((reg) >> (pos)) & 0xFUL)
#define WRITEBYTE(reg, pos, val) \
        ((reg) = ((reg) & ~(0xFFUL << (pos))) | (((val) & 0xFFUL) << (pos)))

#endif 
