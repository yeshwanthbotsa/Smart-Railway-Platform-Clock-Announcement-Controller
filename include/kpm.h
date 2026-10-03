// kpm.h */

#ifndef KPM_H
#define KPM_H

#include "types.h"

#define KPM_BACKSPACE 15

void InitKPM(void);
u8   ColScan(void);
u8   RowCheck(void);
u8   ColCheck(void);
u8   KeyScan(void);
u32  Read4Number(int max, s8 *label);
u32  ReadNumber(int max, s8 *label);

#endif 
