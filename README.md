Smart Railway Platform Clock & Announcement Controller

An embedded C-based railway platform information and announcement controller developed using the ARM7-based LPC2148 microcontroller. The system integrates an RTC, 16×2 LCD, 4×4 matrix keypad, LEDs, buzzer control, and external interrupt to automate train schedule monitoring and passenger information display.

📌 Project Overview

The Smart Railway Platform Clock & Announcement Controller is designed to automate railway platform information management and reduce manual intervention.

The controller continuously monitors the real-time clock and compares the current time with stored train schedules. Based on the schedule, it automatically selects the next train and displays relevant train information on the LCD.

The system also provides an administrator configuration mode triggered through an external interrupt (EINT1). In this mode, train schedule parameters and RTC date/time settings can be modified using the 4×4 keypad.

The project uses modular Embedded C programming with separate modules for LCD, keypad, RTC, external interrupt, train database, administration, delay calculation, and train status management.

🎯 Objectives

Display the current date and time using the RTC.

Display upcoming train information.

Store multiple train records.

Compare RTC time with stored train timings.

Automatically select the next train that has not departed.

Calculate train delay from updated arrival time.

Display train information using the LCD.

Provide LED-based train status indication.

Provide buzzer control as part of train-status logic.

Allow administrator modification of train schedules.

Allow RTC date and time correction.

Automatically update platform information based on the RTC.

Implement the system using modular Embedded C programming.

✨ Features

LPC2148 ARM7 microcontroller

RTC-based train scheduling

16×2 LCD interfacing

4×4 matrix keypad interfacing

Train database management

Automatic next-train selection

Train delay calculation

LED-based train status indication

Buzzer control integrated with train-status logic

External interrupt-based administrator mode

RTC date and time configuration

Scrolling train information display

Modular Embedded C architecture

🧰 Hardware Requirements

Component

Purpose

LPC2148

Main ARM7 microcontroller

16×2 LCD

Displays time and train information

4×4 Matrix Keypad

Train schedule and administrator input

RTC

Maintains current date and time

LEDs

Indicates train status

Buzzer

Audible status indication

External Interrupt Switch

Enters administrator mode

💻 Software Requirements

Embedded C

Keil development environment

Flash Magic

LPC2148 development board

🏗️ System Architecture

                  +-------------------+
                  |      LPC2148      |
                  |     ARM7 MCU      |
                  +---------+---------+
                            |
          +-----------------+------------------+
          |          |          |       |      |
          v          v          v       v      v
        RTC        LCD       Keypad   LEDs   Buzzer
          |          |          |       |      |
          +----------+----------+-------+------+
                            |
                            v
                     Train Database
                            |
                            v
                    Train Status Logic
                            |
                   +--------+--------+
                   |                 |
                   v                 v
              Normal Mode       Admin Mode
                                      ^
                                      |
                                    EINT1

⚙️ System Workflow

Normal Operation

The RTC maintains the current date and time.

The LCD displays the current time and train information.

Train schedule data is stored in the train database.

The controller continuously compares the RTC time with stored train timings.

The controller identifies the next train that has not yet departed.

The corresponding train information is displayed on the LCD.

Train name and destination information can be displayed using a scrolling window.

The controller evaluates the train's time-to-arrival and calculated delay.

LED and buzzer control are updated according to the train status.

After a train has departed, the controller automatically evaluates the next scheduled train.

Administrator Mode

Administrator mode is triggered through EINT1.

After entering administrator mode, the administrator can use the 4×4 matrix keypad to:

Modify train arrival time.

Modify train departure time.

Modify platform number.

Select a predefined destination.

Recalculate train delay.

Modify RTC time.

Modify RTC date.

Modify day of the week.

The administrator menu operates through the LCD and keypad interface.

🚦 Train Status Indication

The controller determines the status of the next scheduled train using RTC time, time-to-arrival, and calculated delay information.

LED

Status

Meaning

Green

On-Time

Train is operating according to its schedule

Yellow

Approaching

Train is approaching its scheduled arrival

Red

Delayed

Train has a calculated delay

The software also includes buzzer control as part of the train-status logic. Actual audible operation depends on the connected buzzer circuit and hardware configuration.

🗄️ Train Database

The project maintains train information using a structured train database.

Each train record contains information such as:

Train number

Train name

Destination

Scheduled arrival time

Scheduled departure time

Updated arrival time

Updated departure time

Platform number

Calculated delay

Example train records used in the project:

Train No.

Train

Destination

Platform

Delay

12627

Karnataka Express

New Delhi

1

0 min

12028

Shatabdi Express

Chennai

2

0 min

12785

Kacheguda Express

Hyderabad

3

20 min

⏱️ Train Delay Calculation

Train delay is calculated by comparing the scheduled arrival time with the updated arrival time.

Scheduled Arrival
       ↓
Convert to minutes
       ↓
Updated Arrival
       ↓
Convert to minutes
       ↓
Updated - Scheduled
       ↓
Delay in minutes

If the updated arrival time is later than the scheduled arrival time, the difference is stored as the train delay. Otherwise, the delay is set to zero.

🖥️ LCD Display

The LCD uses a combination of fixed and scrolling information.

Line 1

12345:TRAIN NAME

The train number remains fixed while train name and destination information can scroll through the available display width.

Line 2

The display alternates between train schedule information and the current RTC time.

Example:

P1 A10:30 D10:40

and:

    10:25:32

This allows both train-specific information and live RTC information to be presented using the 16×2 LCD.

🔄 Display State Machine

The application uses different display states to manage the LCD interface:

WELCOME
   ↓
RTC DISPLAY
   ↓
WAITING FOR TRAIN
   ↓
TRAIN INFORMATION SCROLL
   ↓
STATUS / INFORMATION DISPLAY
   ↓
NEXT TRAIN

The main loop continuously processes RTC information, train selection, display updates, train-status processing, and administrator requests.

📁 Project Structure

Smart-Railway-Platform-Clock-Announcement-Controller/
│
├── .gitignore
├── main.c
│
├── admin.c
├── admin.h
│
├── delay.c
├── delay.h
│
├── eint.c
├── eint.h
├── eint_defines.h
│
├── kpm.c
├── kpm.h
├── kpm_defines.h
│
├── lcd.c
├── lcd.h
├── lcd_defines.h
│
├── rtc.c
├── rtc.h
├── rtc_defines.h
│
├── status.c
├── status.h
├── status_defines.h
│
├── train_db.c
├── train_db.h
│
├── defines.h
├── types.h
├── Startup.s
│
├── smart_railways.sct
├── smart_railways.uvproj
│
├── CHANGES.md
└── test_cases.md

🧩 Software Modules

main.c

Controls the main application flow, RTC processing, train selection, LCD display states, and administrator-mode handling.

rtc.c / rtc.h

Handles RTC initialization, time/date reading, and RTC configuration.

lcd.c / lcd.h

Provides LCD initialization, command handling, character display, and string display functions.

kpm.c / kpm.h

Handles 4×4 matrix keypad scanning and numeric/user input.

eint.c / eint.h

Handles external interrupt functionality used to enter administrator mode.

train_db.c / train_db.h

Contains train records and train database structures.

status.c / status.h

Handles train-status evaluation and LED/buzzer control.

admin.c / admin.h

Provides administrator functionality for modifying train information and RTC settings.

delay.c / delay.h

Provides delay-related functionality used by the application.

🔐 Administrator Configuration

The administrator mode uses the external interrupt to enter the configuration interface.

The train-edit menu provides options for:

1) Arrival
2) Departure
3) Platform
4) Done

Destination selection is performed using predefined station choices supported by the keypad interface.

The RTC configuration menu allows modification of:

Hour

Minute

Second

Date

Month

Year

Day of week

🧪 Testing

The project includes a dedicated test_cases.md file containing functional test cases.

Testing covers the major functional areas of the system, including:

RTC operation

LCD display

Keypad input

Train database handling

Train selection

Train delay calculation

Train status indication

Administrator configuration

External interrupt functionality

RTC configuration

Buzzer control

Automatic train information updates

🔧 Build and Program

The project is developed for the LPC2148 using Embedded C and the Keil development environment.

Basic Workflow

Source Code
    ↓
Keil Build
    ↓
Generate HEX
    ↓
Program LPC2148
    ↓
Connect Hardware
    ↓
Test System

The generated build files and compiler artifacts are intentionally excluded from this Git repository using .gitignore.

📋 Design Approach

The project follows a modular embedded software structure where individual peripherals and functional blocks are separated into dedicated source and header files.

This approach improves:

Code organization

Maintainability

Reusability

Debugging

Peripheral-level testing

Project scalability

The main application combines these modules to implement RTC-driven train scheduling and status management.

🚀 Future Improvements

Possible future improvements include:

EEPROM/Flash-based persistent train schedule storage.

UART-based schedule updates.

Real-time communication with an external railway information system.

Multiple platform support.

Automatic voice announcement using an audio module.

Improved administrator authentication.

Persistent configuration storage.

Expanded automated test coverage.

Improved buzzer hardware integration.

📚 Technologies Used

Microcontroller : LPC2148
Architecture    : ARM7
Language        : Embedded C
Display         : 16×2 LCD
Input           : 4×4 Matrix Keypad
Timekeeping     : RTC
Indicators      : LEDs
Alert Control   : Buzzer
Interrupt       : External Interrupt (EINT1)
IDE             : Keil
Programming     : Flash Magic

🎓 Skills Demonstrated

Embedded C programming

ARM7/LPC2148 microcontroller programming

GPIO and peripheral interfacing

LCD interfacing

Matrix keypad interfacing

RTC programming

External interrupt handling

Embedded state-machine design

Train schedule data management

Time-based event processing

Train delay calculation

LED and buzzer control

Modular driver development

Hardware/software integration

📄 Documentation

Additional project documentation is available in:

CHANGES.md — project changes and development information

test_cases.md — functional test cases and validation

Source and header files — individual peripheral and application modules

⭐ Project Highlights

This project demonstrates a practical embedded-system implementation using the LPC2148 ARM7 microcontroller, combining peripheral drivers with application-level scheduling logic.

Key implementation areas include:

RTC-driven train scheduling

Automatic next-train selection

LCD-based train information display

Scrolling train name and destination

Keypad-based configuration

External interrupt-based administrator mode

Train delay calculation

LED-based train-status indication

Integrated buzzer control

Modular Embedded C architecture

👨‍💻 Project Type

Embedded Systems | Embedded C | ARM7 | LPC2148 | Microcontroller | Real-Time Scheduling | Peripheral Interfacing
