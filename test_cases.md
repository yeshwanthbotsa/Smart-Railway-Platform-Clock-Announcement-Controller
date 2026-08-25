# Test Cases — Smart Railway Platform Clock & Announcement Controller

## 1. RTC & Display

| # | Test Case | Steps | Expected Result |
|---|---|---|---|
| 1.1 | RTC initializes and runs | Power on the board | LCD shows `00:00:00` and increments once per second |
| 1.2 | Date/Day screen appears | Wait ~3 seconds without pressing anything | Display switches from time+train screen to date+day screen |
| 1.3 | Screen cycles back | Wait another ~3 seconds | Display returns to time+train screen |
| 1.4 | RTC keeps time across screens | Watch both screens over one full cycle | Seconds keep counting correctly on the time screen; no time is lost/skipped switching screens |

## 2. Train Schedule Logic

| # | Test Case | Steps | Expected Result |
|---|---|---|---|
| 2.0 | Display waits for real proximity | Power on (RTC boots to `00:00:00`, ~6.5 hrs before Train 1's 6:30 arrival) | LCD shows a waiting screen (`WAITING FOR NEXT TRAIN` / `STATUS: WAITING`), **not** Train 1's info directly — Train 1 only appears once RTC time is within 1 hour of its arrival |
| 2.1 | Correct next train shown | Set RTC time to within 1 hr of Train 1's arrival (e.g. 06:00) | LCD line 2 shows Train 12627, Platform 1, status `ONTIME` |
| 2.2 | Auto-advance to next train | Set RTC time to just after Train 1's updated departure (e.g. 06:40) | Display automatically switches to Train 2 (12028) once RTC enters its display window, without any key press |
| 2.3 | Last train departs | Set RTC time to after Train 3's updated departure (e.g. 08:30) | LCD line 2 shows `NO MORE TRAINS`, all LEDs off, buzzer off |
| 2.4 | Approaching window | Set RTC time to 5 minutes before a train's updated arrival | Status shows `STATUS: APPROACH`, Yellow LED on |
| 2.5 | Delayed train | Use Train 3 (pre-loaded with a 20 min delay) | Status shows `DELAY`, Red LED on |
| 2.6 | Buzzer timing (⚠️ see `CHANGES.md`) | See `docs/CHANGES.md` in the buzzer-fix zip | Buzzer is now time-window based (1 beep at 5 min before arrival, 3 beeps at 1 min before, 1 beep after departure) rather than "beep once on LED color change" — retest against the new behavior, not the old wording |

## 3. Admin-Edit Switch (EINT1 / P0.3)

| # | Test Case | Steps | Expected Result |
|---|---|---|---|
| 3.0a | PIN required | Press the admin-edit switch | Display shows `Enter Admin PIN:` before any menu appears |
| 3.0b | Correct PIN accepted | Enter `1234` then `=` | Admin main menu (`1)Train 2)RTC 4)Exit`) appears |
| 3.0c | Wrong PIN rejected | Enter an incorrect 4-digit value then `=` | LCD shows `Wrong PIN`, then re-prompts for the PIN |
| 3.0d | Max attempts enforced | Enter an incorrect PIN 3 times in a row | LCD shows `Access Denied`, returns directly to normal display without ever showing the admin menu |
| 3.1 | Interrupt fires | Press the admin-edit switch during normal display, then enter the correct PIN | Display shows the admin main menu (`1)Train 2)RTC 4)Exit`) |
| 3.2 | Normal loop pauses correctly | While in admin mode, wait several seconds | Display does NOT return to the time/train screen on its own — stays in the menu until admin exits |
| 3.3 | Exit returns cleanly | From the main admin menu, press `4` | LCD clears and returns to the normal time+train display |

## 4. Admin — Edit Train Schedule

| # | Test Case | Steps | Expected Result |
|---|---|---|---|
| 4.1 | Select valid train | From admin menu, press `1`, then enter `2` for train number | Enters the submenu for Train 2 (Shatabdi Express) |
| 4.2 | Reject invalid train number | Press `1`, then enter `9` (out of range 1-3) | Shows `Invalid Train` and returns to the admin main menu — note: train selection uses raw `KeyScan()`, not `ReadNumber()`, so it's unaffected by the field-label fix in section 5 below | |
| 4.3 | Edit updated arrival | In train submenu, press `1`, enter new hour/minute | Train's `updatedArrivalHour/Minute` update; `delayMinutes` recalculates automatically |
| 4.4 | Delay recalculation - delayed case | Set a train's updated arrival 15 min later than its original scheduled arrival | `delayMinutes` becomes 15; status shows `DELAY` and Red LED on next display refresh |
| 4.5 | Delay recalculation - on-time case | Set a train's updated arrival equal to or earlier than scheduled | `delayMinutes` becomes 0; status returns to `ONTIME`/`APPROACH` as appropriate |
| 4.6 | Edit platform | Press `3` in train submenu, enter a new platform number | Platform number updates and shows correctly on the next normal-mode display |
| 4.7 | Backspace during entry | While entering a number, press the backspace key (key value 15) before confirming | Last digit is erased from both the LCD and the running value |
| 4.8 | Exit submenu | Press `4` in train submenu | Returns to admin main menu (not straight to normal mode) |

## 5. Admin — Edit RTC

| # | Test Case | Steps | Expected Result |
|---|---|---|---|
| 5.1 | Set new time | From admin menu, press `2`, enter hour/min/sec | RTC time updates immediately; visible on next normal-mode display |
| 5.2 | Set new date | Continue through date/month/year prompts | RTC date updates; visible on the date+day screen |
| 5.3 | Set day of week | Enter day value 0-6 | Correct day abbreviation (SUN-SAT) shows on the date+day screen |
| 5.4 | Reject invalid hour | Enter `25` for hour | `ReadNumber(23, "Hour")` rejects it, shows `INVALID` then `(Hour)`, then `TRY AGAIN` `(Hour)`, re-prompts |
| 5.5 | Reject invalid month | Enter `13` for month | `ReadNumber(12, "Month")` rejects it, shows `INVALID` then `(Month)`, then `TRY AGAIN` `(Month)`, re-prompts |
| 5.6 | Field labels shown for every RTC field | Enter an out-of-range value for each of Hour/Minute/Sec/Date/Month/Year/Day in turn | Each shows its own correct label — `(Hour)`, `(Minute)`, `(Sec)`, `(Date)`, `(Month)`, `(Year)`, `(Day)` — not a generic message |

## 6. Known Limitations (document these, don't try to "fix" silently)

| # | Limitation | Notes |
|---|---|---|
| 6.1 | No midnight rollover handling in train-matching logic | `FindNextTrainIndex()`/`GetMinutesToArrival()` compare same-day minute totals only - fine for a single operating shift, but a train scheduled to depart just after midnight relative to a late-night current time isn't handled. Worth a line in your report rather than leaving it unmentioned. |
| 6.2 | 16x2 LCD can't show everything at once | Time+train and date+day are shown on a ~3 second cycle rather than simultaneously - a deliberate design choice given the hardware, worth explaining in your report/viva rather than letting it look like an oversight. |
| 6.3 | Startup time is a placeholder | Boots to `00:00:00, 01/01/2026` until corrected via admin mode - mention this is expected first-boot behavior, not a bug, if a reviewer notices it. |
| 6.4 | PIN stored in firmware, not EEPROM | `ADMIN_PIN` (admin.h) is a compile-time constant (`1234`) - changing it requires reflashing. A future version could store it in EEPROM so it's field-changeable without a rebuild. |
