/*******************************************************************
 *  File:    rtc.h
 *  Module:  LPC2148 Internal RTC Driver
 *  Source:  Based on institute-supplied rtc_main.c, split into a
 *           reusable module (main.c owns the actual main()).
 *******************************************************************/

#ifndef RTC_H
#define RTC_H

#include "types.h"

/* Name-of-day lookup, defined in rtc.c - used by DisplayRTCDay()
 * and available to other modules (e.g. status.c) if needed. */
extern s8 week[7][4];

void RTC_Init(void);

void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second);
void DisplayRTCTime(u32 hour, u32 minute, u32 second);
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second);

void GetRTCDateInfo(s32 *date, s32 *month, s32 *year);
void DisplayRTCDate(u32 date, u32 month, u32 year);
void SetRTCDateInfo(u32 date, u32 month, u32 year);

void GetRTCDay(s32 *dow);
void DisplayRTCDay(u32 day);
void SetRTCDay(u32 dow);

#endif /* RTC_H */
