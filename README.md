# Smart Railway Platform Clock & Announcement Controller

An embedded C-based railway platform information and announcement controller developed using the **ARM7-based LPC2148 microcontroller**. The system integrates an RTC, 16×2 LCD, 4×4 matrix keypad, LEDs, buzzer, and external interrupt to automate train schedule monitoring and passenger information display.

## 📌 Project Overview

The **Smart Railway Platform Clock & Announcement Controller** is designed to automate railway platform information management and reduce manual announcement effort.

The controller continuously monitors the real-time clock and compares the current time with stored train schedules. Based on the schedule, it automatically displays upcoming train information and provides visual and audible status indications.

The system also provides an administrator configuration mode through an external interrupt, allowing authorized users to modify train information and correct the RTC date and time.

The project uses modular Embedded C programming with separate modules for LCD, keypad, RTC, external interrupt, train database, administration, delay, and train status management.

## 🎯 Objectives

* Display the current date and time using the RTC.
* Enter train schedules using a 4×4 matrix keypad.
* Store multiple train records.
* Compare RTC time continuously with scheduled train timings.
* Automatically display upcoming train information.
* Indicate train delays using updated schedule information.
* Provide LED-based train status indication.
* Allow authorized editing of train schedules.
* Allow RTC date and time correction.
* Automatically update platform information based on the RTC.
* Reduce manual intervention in platform information management.

## ✨ Features

* **RTC-based real-time clock**
* **Train schedule management**
* **4×4 matrix keypad input**
* **16×2 LCD information display**
* **Automatic upcoming-train selection**
* **Train delay detection**
* **LED-based status indication**
* **Buzzer-based audible alerts**
* **External interrupt-based administrator mode**
* **Train database management**
* **Modular Embedded C implementation**

## 🧰 Hardware Requirements

| Component                       | Purpose                                |
| ------------------------------- | -------------------------------------- |
| LPC2148                         | Main ARM7 microcontroller              |
| 16×2 LCD                        | Displays time and train information    |
| 4×4 Matrix Keypad               | Train schedule and administrator input |
| RTC                             | Maintains current date and time        |
| LEDs                            | Indicates train status                 |
| Buzzer                          | Provides audible alerts                |
| USB-UART Converter / DB-9 Cable | Serial/programming interface           |

## 💻 Software Requirements

* Embedded C
* Keil development environment
* Flash Magic
* LPC2148 development hardware

## 🏗️ System Architecture

```text
                  +-------------------+
                  |      LPC2148      |
                  |    ARM7 MCU       |
                  +---------+---------+
                            |
          +-----------------+------------------+
          |          |          |       |      |
          v          v          v       v      v
        RTC        LCD       Keypad   LEDs   Buzzer
          |          |          |
          |          |          |
          +----------+----------+
                     |
                     v
              Train Schedule
                 Database
                     |
                     v
              Status / Delay
                Processing
```

## ⚙️ System Workflow

### Normal Operation

1. The RTC maintains the current date and time.
2. The LCD displays the current time and upcoming train information.
3. Train schedule data is stored in the train database.
4. The controller continuously compares the RTC time with stored train timings.
5. As the arrival time approaches, the corresponding train information is displayed.
6. After the scheduled departure time has elapsed, the controller automatically moves to the next scheduled train.
7. LEDs provide the current train status.
8. The buzzer provides an audible notification when a train is about to arrive or when important schedule changes occur.

### Administrator Mode

An administrator can enter configuration mode using the designated external interrupt switch.

After successful authorization, the administrator can use the 4×4 matrix keypad to:

* Select an existing train record.
* Modify arrival time.
* Modify departure time.
* Modify destination.
* Modify platform information.
* Modify other schedule details.
* Correct the RTC date.
* Correct the RTC time.

The interrupt-based configuration mode temporarily pauses normal display operation while the administrator performs configuration.

## 🚦 Train Status Indication

The system uses LEDs to provide visual train status information.

| LED       | Status      | Meaning                                  |
| --------- | ----------- | ---------------------------------------- |
| 🟢 Green  | On-Time     | Train is operating according to schedule |
| 🟡 Yellow | Approaching | Train is expected to arrive shortly      |
| 🔴 Red    | Delayed     | Train timing has been modified/delayed   |

The buzzer provides an additional audible notification for approaching trains and important schedule changes.

## 🗄️ Train Database

The project maintains train information using a structured data type.

Each train record contains information such as:

* Train number
* Train name
* Destination
* Scheduled arrival time
* Scheduled departure time
* Updated arrival time
* Updated departure time
* Platform number
* Delay duration

Example train records included in the project:

| Train No. | Train             | Destination | Platform |  Delay |
| --------- | ----------------- | ----------- | -------: | -----: |
| 12627     | Karnataka Express | New Delhi   |        1 |  0 min |
| 12028     | Shatabdi Express  | Chennai     |        2 |  0 min |
| 12785     | Kacheguda Express | Hyderabad   |        3 | 20 min |

## 📁 Project Structure

```text
Smart-Railway-Platform-Clock-Announcement-Controller/
│
├── .gitignore
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
└── test_cases.md
```

## 🧩 Software Modules

### `main.c`

Main application control and system workflow.

### `rtc.c / rtc.h`

Handles RTC-related operations and maintains the current date and time.

### `lcd.c / lcd.h`

Provides LCD initialization and display operations.

### `kpm.c / kpm.h`

Handles 4×4 matrix keypad interfacing and user input.

### `eint.c / eint.h`

Handles external interrupt functionality used to enter administrator configuration mode.

### `train_db.c / train_db.h`

Contains the train schedule database and train information structure.

### `status.c / status.h`

Handles train status-related processing and indications.

### `admin.c / admin.h`

Provides administrator configuration functionality for train schedule and RTC editing.

### `delay.c / delay.h`

Provides delay-related functionality used by the application.

## 🔄 Train Information Processing

The controller continuously evaluates the stored train schedule against the current RTC time.

```text
          RTC Current Time
                 |
                 v
       Compare with Train DB
                 |
        +--------+--------+
        |                 |
        v                 v
   Schedule Match?    Schedule Changed?
        |                 |
        v                 v
Upcoming Train       Delay Status
        |                 |
        +--------+--------+
                 |
                 v
          Update LCD
                 |
          +------+------+
          |             |
          v             v
        LEDs          Buzzer
```

## 🧪 Testing

The project includes a dedicated `test_cases.md` file containing project test cases.

Testing covers the major functional areas of the system, including:

* RTC operation
* LCD display
* Keypad input
* Train schedule handling
* Train status indication
* Delay handling
* Administrator configuration
* External interrupt functionality
* Buzzer notification
* Automatic train information updates

## 🔧 Build and Program

The project is developed for the **LPC2148** using Embedded C and the Keil development environment.

### Basic workflow

```text
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
```

The generated build files and compiler artifacts are intentionally excluded from this Git repository using `.gitignore`.

## 📋 Design Approach

The project follows a modular embedded software structure where individual peripherals and functional blocks are separated into dedicated source and header files.

This approach improves:

* Code organization
* Maintainability
* Reusability
* Debugging
* Peripheral-level testing
* Project scalability

## 🚀 Future Improvements

Possible future improvements include:

* Larger graphical display for passenger information.
* EEPROM/Flash-based persistent train schedule storage.
* UART-based schedule updates.
* Real-time communication with an external railway information system.
* Multiple platform support.
* Automatic voice announcement using an audio module.
* Improved user authentication for administrator access.
* More extensive automated test coverage.

## 📚 Technologies Used

```text
Microcontroller : LPC2148
Architecture    : ARM7
Language        : Embedded C
Display         : 16×2 LCD
Input           : 4×4 Matrix Keypad
Timekeeping     : RTC
Indicators      : LEDs
Alert           : Buzzer
Interrupt       : External Interrupt
IDE             : Keil
Programming     : Flash Magic
```

## 👨‍💻 Project Type

**Embedded Systems / Embedded C / ARM7 / Microcontroller Project**

This project demonstrates practical implementation of peripheral interfacing, RTC-based scheduling, interrupt handling, keypad input, LCD interfacing, status indication, and modular Embedded C programming on the LPC2148 platform.

## 📄 Documentation

Additional project documentation is available in:

* `CHANGES.md` — project changes and development information
* `test_cases.md` — functional test cases and validation
* Source and header files — individual peripheral and application modules

## ⭐ Project Highlights

* ARM7 LPC2148 microcontroller
* RTC-driven scheduling
* Automatic train information selection
* Delay monitoring
* LCD-based passenger information
* Keypad-based schedule management
* External interrupt-based administrator mode
* LED train-status indication
* Audible buzzer alerts
* Modular Embedded C architecture
