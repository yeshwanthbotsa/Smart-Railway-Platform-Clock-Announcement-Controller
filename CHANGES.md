# CHANGES.md — Buzzer Fix

## What changed vs. your submitted `mini_proj22_corrected.zip`

Only **3 files touched**: `status.c`, `status.h` (fully replaced with the
working buzzer implementation from your other project), and `main.c`
(one call site updated, everything else — welcome screen, RTC screen,
display timing, state machine — untouched).

### Why not just copy `status.c` alone?

The working buzzer implementation changed `UpdateTrainStatus()`'s
signature:

```c
/* old (current project) */
void UpdateTrainStatus(s32 minsToArrival, u8 delayMinutes);

/* new (working buzzer project) */
void UpdateTrainStatus(s32 trainIndex, s32 curHour, s32 curMin, s32 curSec);
```

Dropping in `status.c` alone would leave `main.c` calling the old
signature and fail to build. So `main.c`'s **one call site** was updated
to match — nothing else in `main.c` was touched.

### What the new buzzer actually does (vs. the old one)

| | Old behavior | New behavior |
|---|---|---|
| Trigger | Beeps once when LED color transitions (Green→Yellow or Green→Red) | Beeps on specific time windows, tracked per-train |
| Approaching | 1 beep, on entering the approach window | 1 beep at exactly 5 min before arrival |
| Close approach | (not present) | 3 beeps at 1 min before arrival |
| Departure | (not present) | 1 beep right after departure |
| Scope | Only for the currently-displayed "next" train | Checks **all trains** every loop, independent of which one is on-screen |

This is a real behavior change, not just a bug fix — your buzzer will
now sound more often and at different moments than before. Make sure
this matches what you actually want before your demo; if you only
wanted the original "beep once on approach/delay" behavior but just
working correctly, let me know and I'll adjust it instead of using the
full 3-beep/5-min/1-min scheme.

### `main.c` — exact edit made

**Removed** (no longer needed — the new buzzer module recomputes this
internally from `curHour/curMin/curSec` and `TrainDB[]` directly):
```c
s32 minsToArr;
...
minsToArr = GetMinutesToArrival((u8)idx, hour, min);
```

**Changed** — the status/buzzer update now runs every loop iteration
unconditionally (previously it only ran when `idx >= 0`, calling
`Status_Reset()` otherwise). This is required because the new buzzer
checks *all three trains'* time windows every cycle, including the
after-departure beep for a train that just left — even if it's no
longer the "next" train being displayed:

```c
/* before */
if (idx >= 0)
{
    minsToArr = GetMinutesToArrival((u8)idx, hour, min);
    UpdateTrainStatus(minsToArr, TrainDB[idx].delayMinutes);
}
else
{
    Status_Reset();
}

/* after */
UpdateTrainStatus(idx, hour, min, sec);
```

`UpdateTrainStatus()` still turns the LEDs off internally when
`idx == -1` (no upcoming train), so the LED behavior for that case is
unchanged — only the buzzer's scope widened.

`GetMinutesToArrival()` itself was **left untouched** in `train_db.c`/
`train_db.h` — it's just no longer called from `main.c`. Harmless dead
code; I didn't remove it since that wasn't part of what you asked for.

## Verification done

- Every `.c` file syntax-checked against a stub LPC214x register header
  (covers all registers/symbols your code actually uses) — zero errors,
  zero warnings.
- Full link check across all 8 firmware files (with `eint.c`'s two
  exported symbols stubbed, since its Keil-specific `__asm{}` blocks
  can't be parsed by a host gcc) — links cleanly, confirming `main.c`'s
  new call correctly matches `status.c`'s new function signature.
- This is **not** a substitute for a real Keil build — please rebuild
  in µVision and flash before your actual demo/viva to confirm on real
  hardware.

## What to test on the kit

Since the buzzer logic changed meaningfully, retest it specifically:
1. Set RTC to exactly 5 minutes before a train's arrival → expect 1 beep
2. Let it reach exactly 1 minute before arrival → expect 3 beeps
   (250ms each, 150ms gaps)
3. Let the train's departure time pass → expect 1 beep after departure
4. Confirm each of these only fires **once** per train (not repeating
   every loop) — the `beep5Done[]`/`beep1Done[]`/`beepAfterDone[]` flags
   in `status.c` are what prevent repeats; they reset when
   `Status_Init()`/`Status_Reset()` runs (e.g. after leaving admin mode).

---

## Fix 2: Display no longer shows the first train immediately

### The problem

On power-on, the RTC boots to a placeholder `00:00:00`. Since
`FindNextTrainIndex()` correctly finds Train 1 as "the earliest train
that hasn't departed yet" (true at any time before 6:35), the LCD would
immediately show Train 1's full info — even though the real time isn't
actually close to 6:30 yet. It looked like the display wasn't comparing
against RTC time at all, when actually it was comparing correctly, just
with no lower bound on how far away is "too far to show."

### The fix

Added a new function, `FindDisplayTrain()`, in `main.c`. It only
considers a train "displayable" if the current RTC time falls between
**1 hour before its arrival** and its departure. Outside that window,
the LCD shows a waiting screen (`WAITING FOR NEXT TRAIN` / `STATUS:
WAITING`) instead of train info.

**Important — this is a display-only change.** `FindNextTrainIndex()`
(used for LED color and the buzzer via `UpdateTrainStatus()`) is
**untouched** — LEDs and the buzzer still track the true next train
regardless of the 1-hour display window, so status indication doesn't
lag behind reality. Only what appears on the *LCD text* is gated.

### Files touched
- `main.c` only. No other file changed for this fix.

### What changed inside `main.c`
1. Added `#define DISPLAY_LEAD_SECONDS 3600` (1 hour — adjust this
   single constant if you want a different lead time, e.g. `1800` for
   30 minutes).
2. Added `static s32 FindDisplayTrain(s32 curHour, s32 curMin, s32 curSec)`.
3. `main()` now computes both `idx` (true next train, unchanged) and
   a new `displayIdx` (gated) every loop.
4. `ShowLine1Normal()`, `ShowLine2ArrDep()`, and `ShowLine2Status()`
   each gained a second parameter (`nextIdx`) so they can tell the
   difference between "genuinely no more trains today" (`nextIdx < 0`
   → shows `NO MORE TRAINS`) and "there's a train coming, just not
   close enough yet" (`nextIdx >= 0` but `displayIdx < 0` → shows
   `WAITING...` / `STATUS: WAITING`).

### Verified before packaging
- Host-side test against your actual `TrainDB` data (8 cases): confirms
  at boot (`00:00:00`) the display shows "waiting," not Train 1, while
  the LED/buzzer-facing `idx` still correctly identifies Train 1 as
  next. Also confirmed the 1-hour boundary itself (just inside vs. just
  outside the window) and the mid-journey and end-of-day cases. All 8
  passed.
- Full project syntax-check + link-check (all 8 firmware files):
  zero errors, zero warnings, links cleanly.
- Not a substitute for a real Keil build/hardware test — please rebuild
  in µVision and retest on the kit, specifically test case 2.0 in
  `test_cases.md`.

---

## Fix 3: "TRY AGAIN" / "INVALID" now names the field

### The problem

`ReadNumber()` and `Read4Number()` in `kpm.c` showed a plain
`"TRY AGAIN"` / `"INVALID"` on bad input, with no indication of which
field (Hour, Minute, Sec, Platform, PIN, etc.) actually failed —
confusing during multi-field entry like the RTC time edit (Hour →
Minute → Sec back to back).

### The fix

Both functions gained a new `label` parameter. On `NO INPUT`,
`INVALID`, or `TRY AGAIN`, the field name now appears on line 2 in
parentheses — e.g. line 1 shows `TRY AGAIN`, line 2 shows `(Minute)`.

### Files touched
- `kpm.h` — `ReadNumber()`/`Read4Number()` signatures gained `s8 *label`.
- `kpm.c` — both functions print `(label)` under `NO INPUT`/`INVALID`/
  `TRY AGAIN`.
- `admin.c` — all 13 call sites updated to pass a label: `"Arr Hour"`,
  `"Arr Min"`, `"Dep Hour"`, `"Dep Min"`, `"Platform"`, `"Hour"`,
  `"Minute"`, `"Sec"`, `"Date"`, `"Month"`, `"Year"`, `"Day"`, `"PIN"`.

No other file needed changes — nothing else calls `ReadNumber()`/
`Read4Number()`.

### Verified before packaging
- Confirmed via diff against the pre-fix `admin.c` that these 13 lines
  are the *only* lines changed — the PIN-authentication logic from the
  earlier fix, `IsValidTrainTime()` checks, and everything else are
  byte-for-byte untouched.
- Full syntax-check + link-check across all 8 firmware files: zero
  errors, zero warnings, links cleanly.
- Not a substitute for a real Keil build — please rebuild in µVision
  and confirm the field name actually displays correctly on your
  16×2 LCD (line 2, in parentheses) before your demo.
