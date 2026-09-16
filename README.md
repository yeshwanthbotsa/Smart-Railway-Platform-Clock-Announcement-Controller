Smart Railway Platform Clock & Announcement Controller

An Embedded C based railway platform information system developed using the LPC2148 ARM7 microcontroller.

The project is designed to automatically monitor train schedules using a Real-Time Clock (RTC), identify the upcoming train, and display useful train information such as train number, destination, arrival time, departure time, and platform information on a 16×2 LCD.

The system also provides LED-based train status indication, buzzer alerts, and an administrator mode for updating train schedules and correcting the RTC through a 4×4 matrix keypad.

📌 Project Overview

At a railway platform, passengers need accurate information about:

Which train is arriving next?

What is the train's destination?

When will it arrive?

When will it depart?

Which platform is it using?

Is the train on time or delayed?

This project implements a small embedded system to automate these tasks.

The RTC continuously maintains the current date and time. The controller compares the current RTC time with the stored train schedule and determines which train should currently be displayed.

The selected train information is then shown on the 16×2 LCD. The system can also indicate train status using LEDs and provide an audible indication through a buzzer.

An administrator can enter a configuration mode using an external interrupt and the 4×4 keypad. This allows train schedule information and RTC settings to be modified without changing the source code.

![Smart Railway Platform Clock and Announcement Controller](Smart%20Railway%20Platform%20Clock%20and%20Announcement%20Controller.png)

🎯 Project Aim

To develop a Smart Railway Platform Clock & Announcement Controller that automatically manages:

Train schedule monitoring

Real-time clock synchronization

Train delay indication

Passenger information display

Train status indication

Administrator schedule updates

The overall goal is to reduce manual intervention and provide passengers with continuously updated railway information.

✨ Main Features

LPC2148 ARM7 microcontroller based system

RTC-based real-time date and time display

16×2 LCD interface

4×4 matrix keypad interface

Multiple train schedule records

Automatic upcoming-train selection

Arrival and departure time display

Platform information display

Train delay calculation

Green, Yellow, and Red LED status indication

Buzzer-based audible notification

External interrupt based administrator mode

Train schedule modification through keypad

RTC date and time modification

Scrolling train name and destination display

Modular Embedded C software design

🧰 Hardware Requirements

Hardware

Purpose

LPC2148

Main ARM7 microcontroller

16×2 LCD

Displays RTC and train information

4×4 Matrix Keypad

User and administrator input

RTC

Maintains current date and time

LEDs

Indicates train status

Buzzer

Provides audible notification

Admin Edit Switch

Generates external interrupt

USB-UART Converter / DB-9 Cable

Communication/programming support

💻 Software Requirements

Embedded C

Keil development environment

Flash Magic

LPC2148 development board


🔄 How the System Works

The complete operation can be understood in the following sequence:

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
   Compare With Train Database
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
       +-----+-----+
       |     |     |
       v     v     v
    On-Time Near  Delayed
       |     |     |
       +-----+-----+
             |
             v
       Update Display
             |
             v
     Check Admin Request
             |
       +-----+-----+
       |           |
      No          Yes
       |           |
       |           v
       |      Admin Mode
       |           |
       |           v
       |     Modify Settings
       |           |
       +-----------+
             |
             v
        Continue Loop

The controller continuously repeats this process during normal operation.

🕒 RTC and Time Management

The RTC is responsible for maintaining the current date and time.

The system uses the RTC to obtain:

Hour

Minute

Second

Date

Month

Year

Day of the week

The current RTC information is displayed during the RTC display stage.

Example:

09:24:55 MON
10/08/2026

The RTC is also used for comparing the current time with train arrival and departure timings.

This is important because the system does not depend on a manually entered current time during normal operation.

🚆 Train Schedule Management

Train schedule information is maintained in a train database.

A train record contains information such as:

Train number

Train name

Destination

Scheduled arrival time

Scheduled departure time

Updated arrival time

Updated departure time

Platform number

Delay

Example records include:

Train No.

Train Name

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

Delayed

The controller compares the RTC time with the stored train schedule and selects the appropriate upcoming train.

After a train's scheduled departure time has elapsed, the controller automatically moves to the next scheduled train.

📺 LCD Display

The 16×2 LCD is the main passenger information display.

The software uses different display stages.

1. Welcome Screen

When the system starts, it displays:

Welcome To
Smart Railway...

The complete project title is displayed using scrolling text.

2. RTC Screen

The RTC screen displays the current time and date.

Example:

09:24:55 MON
10/08/2026

3. Normal Railway Display

The normal display provides information about the upcoming train.

Example:

12345:TRAIN NAME
P1 A10:30 D10:40

Where:

12345 = Train number

TRAIN NAME = Train name/destination information

P1 = Platform number

A10:30 = Arrival time

D10:40 = Departure time

The train name and destination can be displayed using scrolling text because a 16×2 LCD has limited character capacity.

The second LCD line can also display the current RTC time.

🚦 Train Status Indication

The system uses LEDs to provide a quick visual indication of train status.

LED

Status

Meaning

🟢 Green

On-Time

Train is operating according to schedule

🟡 Yellow

Approaching

Train is expected to arrive shortly

🔴 Red

Delayed

Train timing has been modified/delayed

This allows passengers to understand the train condition without continuously reading the LCD.

🔊 Buzzer Notification

A buzzer is included to provide an audible notification.

The project documentation specifies buzzer notification when:

A train is about to arrive

Important schedule changes occur

This is useful because passengers may not continuously watch the LCD.

The actual audible behavior depends on the buzzer circuit and hardware connection used with the LPC2148 board.

⏱️ Train Delay Calculation

The system can compare the scheduled arrival time with the updated arrival time.

Conceptually:

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

For example:

Scheduled Arrival = 08:00
Updated Arrival   = 08:20

Delay = 20 minutes

If the updated arrival time is later than the scheduled arrival time, the difference represents the train delay.

🔐 Administrator Mode

The administrator can modify train and RTC information without changing the source code.

Administrator mode is entered using an external interrupt.

The basic flow is:

Normal Operation
       |
       v
Admin Edit Switch
       |
       v
External Interrupt
       |
       v
Administrator Mode
       |
       v
LCD + Keypad Menu
       |
       v
Modify Required Information
       |
       v
Return to Normal Operation

The administrator can select an existing train record and modify information such as:

Arrival time

Departure time

Destination

Platform number

Other schedule information

The same configuration mode can also be used to correct:

RTC time

RTC date

Day of the week

⌨️ 4×4 Matrix Keypad

The keypad provides user input for the administrator configuration interface.

It is used to:

Select menu options

Enter numeric values

Modify train timings

Modify platform information

Select destination information

Configure RTC values

This eliminates the need to modify train data directly inside the source code for every schedule change.

⚡ External Interrupt

The administrator edit switch is connected to an external interrupt input.

When the administrator activates the switch:

Admin Switch
     |
     v
External Interrupt
     |
     v
Admin Request Flag
     |
     v
Main Application Detects Request
     |
     v
AdminMode()

The interrupt mechanism allows the normal railway display operation to be interrupted so that the administrator configuration interface can be entered.

🧱 Software Architecture

The project is divided into multiple modules instead of putting the entire application into one source file.

                    +------------------+
                    |      main.c      |
                    | Main Application |
                    +--------+---------+
                             |
        +--------------------+--------------------+
        |          |         |        |            |
        v          v         v        v            v
       RTC        LCD      Keypad    EINT        Status
        |          |         |        |            |
        +----------+---------+--------+------------+
                             |
                             v
                      Train Database
                             |
                             v
                       Admin Module

This modular structure makes the project easier to understand, debug, maintain, and extend.

📁 Project Structure

Smart-Railway-Platform-Clock-Announcement-Controller/
│
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
├── test_cases.md
└── .gitignore

🧩 Software Modules

main.c

The main application module.

It controls:

Startup sequence

RTC display

Train display

Train selection

Display scrolling

Train status processing

Administrator request handling

The main loop continuously processes the system state.

rtc.c / rtc.h

Responsible for RTC functionality.

It handles:

RTC initialization

Reading current time

Reading current date

RTC configuration

Time/date updates

lcd.c / lcd.h

Responsible for 16×2 LCD interfacing.

It provides functions for:

LCD initialization

Sending commands

Displaying characters

Displaying strings

Moving the LCD cursor

Updating display positions

kpm.c / kpm.h

Responsible for 4×4 matrix keypad interfacing.

It handles:

Key scanning

Numeric input

Menu selection

Administrator input

eint.c / eint.h

Responsible for external interrupt functionality.

It detects the administrator edit request and allows the main application to enter administrator mode.

train_db.c / train_db.h

Contains the train database and train-related structures.

It stores information such as:

Train number

Train name

Destination

Arrival time

Departure time

Platform

Delay

status.c / status.h

Responsible for train status processing and output indication.

It handles the logic associated with:

Train status

LED indication

Buzzer control

admin.c / admin.h

Responsible for administrator configuration.

It provides functionality for modifying:

Train schedule information

Platform information

Destination information

RTC settings

delay.c / delay.h

Contains delay-related timing functionality used by the application.

🧪 Testing

The project contains a test_cases.md file for functional testing.

Important areas tested include:

RTC operation

LCD operation

Keypad input

Train database handling

Train selection

Train arrival/departure display

Train delay calculation

LED status indication

Buzzer control

Administrator mode

External interrupt operation

RTC configuration

Automatic transition to the next train

Testing the modules individually before testing the complete system helps identify hardware and software integration problems.

🔧 Development and Programming

The project was developed using:

Microcontroller : LPC2148
CPU Architecture: ARM7
Programming      : Embedded C
IDE              : Keil
Programming Tool : Flash Magic
Display          : 16×2 LCD
Input            : 4×4 Matrix Keypad
Timekeeping      : RTC
Status           : LEDs
Audio Alert      : Buzzer
Interrupt        : External Interrupt

The basic development workflow is:

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

🧠 Embedded Concepts Demonstrated

This project demonstrates practical implementation of several embedded-system concepts:

Microcontroller Programming

Programming the LPC2148 ARM7 microcontroller using Embedded C.

Peripheral Interfacing

Interfacing multiple peripherals with the microcontroller:

LCD

Matrix keypad

RTC

LEDs

Buzzer

Interrupt Handling

Using an external interrupt to trigger administrator configuration mode.

Real-Time Processing

Continuously comparing RTC time against train schedule information.

State-Based Application Design

Managing different application stages such as:

Welcome

RTC display

Train display

Status indication

Administrator mode

Data Management

Maintaining multiple train records using structured data.

Time Calculations

Comparing scheduled and updated timings to determine train delay.

Hardware and Software Integration

Combining peripheral drivers, application logic, and physical hardware into a single embedded system.

📈 Why This Project Is Useful

A conventional platform display may require manual updating whenever train schedules change.

This project attempts to automate that process by continuously comparing the current RTC time with stored train schedules.

Instead of manually deciding which train should be displayed, the controller can:

Read Current Time
       ↓
Compare Train Schedules
       ↓
Identify Upcoming Train
       ↓
Display Train Information
       ↓
Indicate Train Status
       ↓
Move to Next Train

This reduces manual intervention and provides continuously updated passenger information.

🚀 Future Improvements

The current project can be extended in several ways:

Store train schedules permanently in EEPROM or Flash.

Add UART communication for updating schedules from a PC.

Connect the controller to an external railway information system.

Support multiple railway platforms simultaneously.

Add a dedicated voice/audio announcement module.

Add stronger administrator authentication.

Add persistent configuration storage.

Add more automated test cases.

Improve buzzer hardware integration.

Add a larger graphical display for more train information.

📚 Documentation

The repository contains additional project documentation:

CHANGES.md — project development changes and information

test_cases.md — functional test cases

Source files (.c) — implementation of individual modules

Header files (.h) — module interfaces and definitions

Keil project files — project configuration and build information

⭐ Project Highlights

The main engineering aspects of this project are:

LPC2148 ARM7 Embedded C development

RTC-based real-time scheduling

Automatic upcoming-train selection

16×2 LCD passenger information display

Scrolling train name and destination

4×4 matrix keypad input

External interrupt-based administrator mode

Train schedule modification

Train delay calculation

LED-based train status indication

Buzzer-based notification

Modular embedded software architecture

Hardware and software integration

👨‍💻 Skills Demonstrated

Embedded C

ARM7 architecture

LPC2148 microcontroller

GPIO interfacing

LCD interfacing

Matrix keypad interfacing

RTC programming

External interrupt handling

Embedded state-machine design

Time-based event processing

Train schedule management

Delay calculation

LED and buzzer control

Modular driver development

Hardware debugging

Embedded system integration

📌 Project Type

Embedded Systems | Embedded C | ARM7 | LPC2148 | RTC | LCD | Matrix Keypad | External Interrupt | Real-Time Scheduling | Peripheral Interfacing
