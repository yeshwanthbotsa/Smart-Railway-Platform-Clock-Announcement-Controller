# 🚆 Smart Railway Platform Clock & Announcement Controller

> **An Embedded C based railway platform information and announcement controller developed using the LPC2148 ARM7 microcontroller.**

The **Smart Railway Platform Clock & Announcement Controller** is an embedded system designed to automate railway platform information display and train-status indication.

The system uses an **RTC** to maintain the current date and time, compares the current time with stored train schedules, identifies the appropriate upcoming train, and displays train information such as **train number, destination, arrival time, departure time, and platform number** on a **16×2 LCD**.

The system also provides **LED-based train status indication, buzzer alerts, and an administrator mode** through an external interrupt and 4×4 matrix keypad.

---

## 📌 Project Overview

At a railway platform, passengers need continuously updated information about:

- Which train is arriving next?
- What is the destination?
- When will it arrive?
- When will it depart?
- Which platform will it use?
- Is the train on time, approaching, or delayed?

This project implements these functions using an **LPC2148 ARM7 microcontroller** and a modular Embedded C software architecture.

The RTC continuously maintains the current time. The controller compares the RTC time against the stored train database and determines which train information should currently be displayed.

An administrator can enter configuration mode using an **external interrupt** and modify train schedule information or RTC settings through the **4×4 matrix keypad**.

---

## 🎯 Project Aim

To develop a Smart Railway Platform Clock & Announcement Controller that can automatically manage:

- Train schedule monitoring
- Real-time clock display
- Upcoming-train selection
- Passenger information display
- Train arrival and departure information
- Train delay indication
- LED-based train status
- Buzzer notifications
- Administrator schedule updates
- RTC date and time correction

The overall objective is to reduce manual intervention and provide continuously updated railway information.

---

## 🖼️ Architecture

![Smart Railway Platform Clock and Announcement Controller](Smart%20Railway%20Platform%20Clock%20and%20Announcement%20Controller.png)

---

## ✨ Key Features

- LPC2148 ARM7 microcontroller based system
- Embedded C firmware
- RTC-based real-time scheduling
- 16×2 LCD passenger information display
- 4×4 matrix keypad interface
- Multiple train schedule records
- Automatic upcoming-train selection
- Arrival and departure time display
- Platform information display
- Train delay calculation
- Green, Yellow and Red LED status indication
- Buzzer-based audible notification
- External interrupt based administrator mode
- Train schedule modification through keypad
- RTC date and time modification
- Scrolling train name and destination display
- Modular driver-based software architecture

---

# 🧰 Hardware Requirements

| Hardware | Purpose |
|---|---|
| **LPC2148** | Main ARM7 microcontroller |
| **16×2 LCD** | Displays RTC and train information |
| **4×4 Matrix Keypad** | Administrator/user input |
| **RTC** | Maintains current date and time |
| **LEDs** | Train-status indication |
| **Buzzer** | Audible notification |
| **Admin Edit Switch** | Generates external interrupt |
| **USB-UART / DB-9 Cable** | Programming/communication support |

---

# 💻 Software Requirements

| Software / Tool | Purpose |
|---|---|
| **Embedded C** | Firmware development |
| **Keil** | Compilation and project development |
| **Flash Magic** | LPC2148 programming |
| **LPC2148 Development Board** | Hardware platform |

---

# 🏗️ System Architecture

```text
                    +----------------------+
                    |      LPC2148         |
                    |       ARM7           |
                    +----------+-----------+
                               |
          +--------------------+--------------------+
          |          |          |         |         |
          v          v          v         v         v
        RTC        LCD       Keypad     EINT      Status
          |          |          |         |       LEDs
          |          |          |         |       Buzzer
          +----------+----------+---------+---------+
                               |
                               v
                     +-------------------+
                     |  Train Database   |
                     +-------------------+
                               |
                               v
                     +-------------------+
                     | Admin Configuration|
                     +-------------------+
```

---

# 🔄 How the System Works

The complete operation follows this sequence:

```text
                  Power ON
                     |
                     v
              Welcome Display
                     |
                     v
                RTC Display
                     |
                     v
              Read Current Time
                     |
                     v
          Compare Train Database
                     |
                     v
           Find Upcoming Train
                     |
                     v
          Display Train Information
                     |
                     v
            Check Train Status
                     |
              +------+------+
              |             |
            On-Time      Delayed/
              |          Approaching
              +------+------+
                     |
                     v
              Update Display
                     |
                     v
          Check Admin Request
              +------+------+
              |             |
             No            Yes
              |             |
              |             v
              |        Administrator
              |             Mode
              |             |
              |             v
              |      Modify Settings
              |             |
              +-------------+
                     |
                     v
              Continue Loop
```

The controller continuously repeats this process during normal operation.

---

# 🕒 RTC and Time Management

The RTC maintains the current date and time used by the application.

The system obtains:

- Hour
- Minute
- Second
- Date
- Month
- Year
- Day of the week

Example:

```text
09:24:55 MON
10/08/2026
```

The RTC is also used to compare the current time against train arrival and departure schedules.

This allows the controller to automatically determine which train should currently be displayed.

---

# 🚆 Train Schedule Management

Train schedule information is maintained in a train database.

A train record contains information such as:

- Train number
- Train name
- Destination
- Scheduled arrival time
- Scheduled departure time
- Updated arrival time
- Updated departure time
- Platform number
- Delay information

### Example Train Records

| Train No. | Train Name | Destination | Platform | Delay |
|---|---|---|---:|---:|
| 12627 | Karnataka Express | New Delhi | 1 | 0 min |
| 12028 | Shatabdi Express | Chennai | 2 | 0 min |
| 12785 | Kacheguda Express | Hyderabad | 3 | Delayed |

The controller compares the RTC time with the stored schedule and selects the appropriate upcoming train.

After a train's departure time has elapsed, the controller can move to the next scheduled train.

---

# 📺 LCD Display

The **16×2 LCD** is the primary passenger information display.

## 1. Welcome Screen

At startup:

```text
Welcome To
Smart Railway...
```

The project title is displayed using scrolling text.

## 2. RTC Screen

Example:

```text
09:24:55 MON
10/08/2026
```

## 3. Normal Railway Display

Example:

```text
12345:TRAIN NAME
P1 A10:30 D10:40
```

Where:

- `12345` → Train number
- `TRAIN NAME` → Train name/destination information
- `P1` → Platform number
- `A10:30` → Arrival time
- `D10:40` → Departure time

Scrolling text is used because a 16×2 LCD has limited display capacity.

---

# 🚦 Train Status Indication

LEDs provide a quick visual indication of train status.

| LED | Status | Meaning |
|---|---|---|
| 🟢 Green | On-Time | Train is operating according to schedule |
| 🟡 Yellow | Approaching | Train is expected to arrive shortly |
| 🔴 Red | Delayed | Train timing has been modified/delayed |

This provides passengers with an immediate visual status without requiring them to continuously read the LCD.

---

# 🔊 Buzzer Notification

A buzzer provides audible notifications.

The system documentation specifies buzzer notification for events such as:

- Train approaching/near arrival
- Important schedule changes

The exact audible behavior depends on the buzzer circuit and hardware connection used with the LPC2148 development board.

---

# ⏱️ Train Delay Calculation

The system can compare scheduled arrival time with updated arrival time.

```text
Scheduled Arrival
        |
        v
Convert Time to Minutes
        |
        v
Updated Arrival
        |
        v
Convert Time to Minutes
        |
        v
Calculate Difference
        |
        v
Delay in Minutes
```

Example:

```text
Scheduled Arrival = 08:00
Updated Arrival   = 08:20

Delay = 20 minutes
```

If the updated arrival time is later than the scheduled arrival time, the difference represents the train delay.

---

# 🔐 Administrator Mode

Administrator mode allows train and RTC information to be modified without changing the source code.

The administrator enters the mode using an **external interrupt**.

```text
Normal Operation
       |
       v
Admin Edit Switch
       |
       v
External Interrupt
       |
       v
Admin Request
       |
       v
Administrator Mode
       |
       v
LCD + Keypad Menu
       |
       v
Modify Information
       |
       v
Return to Normal Operation
```

The administrator can modify:

- Arrival time
- Departure time
- Destination
- Platform number
- Other schedule information
- RTC time
- RTC date
- Day of the week

---

# ⌨️ 4×4 Matrix Keypad

The keypad provides administrator input.

It is used for:

- Menu selection
- Numeric input
- Train schedule modification
- Platform selection
- Destination selection
- RTC configuration

This allows schedule information to be modified without directly editing the firmware source code for every change.

---

# ⚡ External Interrupt

The administrator edit switch is connected to an external interrupt input.

The basic sequence is:

```text
Admin Switch
     |
     v
External Interrupt
     |
     v
Admin Request Flag
     |
     v
Main Application
     |
     v
AdminMode()
```

The interrupt mechanism allows the normal railway display operation to be interrupted and the administrator configuration interface to be entered.

---

# 📁 Project Structure

The repository can be organized as:

```text
Smart-Railway-Platform-Clock-Announcement-Controller/
│
├── README.md
├── CHANGES.md
│
├── src/
│   ├── main.c
│   ├── admin.c
│   ├── delay.c
│   ├── eint.c
│   ├── kpm.c
│   ├── lcd.c
│   ├── rtc.c
│   ├── train_db.c
│   └── status.c
│
├── include/
│   ├── admin.h
│   ├── defines.h
│   ├── delay.h
│   ├── eint.h
│   ├── eint_defines.h
│   ├── kpm.h
│   ├── kpm_defines.h
│   ├── lcd.h
│   ├── rtc.h
│   ├── status.h
│   └── train_db.h
│
├── startup/
│   └── Startup.s
│
├── proteus/
│   └── Smart-Railway-Platform.dsn
│
└── docs/
    ├── project-image.png
    ├── block-diagram.png
    └── circuit-diagram.png
```

> Keep the actual repository structure synchronized with the files committed to GitHub.

---

# 🧩 Software Modules

## `main.c`

Main application module responsible for:

- Startup sequence
- RTC display
- Train display
- Train selection
- Display scrolling
- Train status processing
- Administrator request handling
- Main application loop

---

## `rtc.c / rtc.h`

Responsible for RTC functionality:

- RTC initialization
- Reading current time
- Reading current date
- RTC configuration
- Time/date updates

---

## `lcd.c / lcd.h`

Responsible for 16×2 LCD interfacing:

- LCD initialization
- Sending commands
- Displaying characters
- Displaying strings
- Cursor positioning
- Display updates

---

## `kpm.c / kpm.h`

Responsible for 4×4 matrix keypad interfacing:

- Key scanning
- Numeric input
- Menu selection
- Administrator input

---

## `eint.c / eint.h`

Responsible for external interrupt functionality.

It detects the administrator edit request and allows the main application to enter administrator mode.

---

## `train_db.c / train_db.h`

Contains train-related data structures and schedule information.

It manages:

- Train number
- Train name
- Destination
- Arrival time
- Departure time
- Platform
- Delay information

---

## `status.c / status.h`

Responsible for train-status processing and output indication:

- Train status
- LED indication
- Buzzer control

---

## `admin.c / admin.h`

Responsible for administrator configuration:

- Train schedule modification
- Platform modification
- Destination modification
- RTC settings

---

## `delay.c / delay.h`

Contains timing-related functionality used by the application.

---

# 🧪 Testing

Functional testing covers the major system modules and their integration.

Important test areas include:

- RTC operation
- LCD operation
- Keypad input
- Train database handling
- Train selection
- Arrival/departure display
- Train delay calculation
- LED status indication
- Buzzer control
- Administrator mode
- External interrupt operation
- RTC configuration
- Automatic transition to the next train

Testing individual modules before complete system integration helps identify hardware and software integration problems.

---

# 🔧 Development and Programming

| Parameter | Details |
|---|---|
| **Microcontroller** | LPC2148 |
| **Architecture** | ARM7 |
| **Programming** | Embedded C |
| **IDE** | Keil |
| **Programming Tool** | Flash Magic |
| **Display** | 16×2 LCD |
| **Input** | 4×4 Matrix Keypad |
| **Timekeeping** | RTC |
| **Status Indication** | LEDs |
| **Audio Alert** | Buzzer |
| **Interrupt** | External Interrupt |

### Development Workflow

```text
Write Embedded C Code
        ↓
Compile in Keil
        ↓
Build Project
        ↓
Generate HEX File
        ↓
Program LPC2148
        ↓
Connect Hardware
        ↓
Test Functionality
```

---



# 📌 Project Type

**Embedded Systems | Embedded C | ARM7 | LPC2148 | RTC | LCD | Matrix Keypad | External Interrupt | Real-Time Scheduling | Peripheral Interfacing**

---

## 👨‍💻 Author

**Yeshwanth Botsa**

Embedded Systems Engineer | Embedded C | ARM7 | LPC2148
