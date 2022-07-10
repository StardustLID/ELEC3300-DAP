# ELEC3300-DAP
The repo for the Digital Audio Player (DAP) project for HKUST ELEC3300 (Fall 2021-22).

## Project Setup
Prerequisite: all STM32 setup and installation should be done in ELEC3300.

### PCB Design
The PCB is designed with [Altium Designer](https://www.altium.com/).

### STM32 Models and Pins
- We used STM32F103VET6 with the same layout and config as specified by the course at first
- When F4 arrived, the pinouts and dependencies were changed accordingly

### Code Generation
The paths in `.mxproject` seem to depend on the person who generated the code. Please tell me if it causes any difficulty.

## Looking Back
There are a lot of things we could've done better:
1. We started too late. The PCB we designed arrived in November, which didn't leave us with much development time.
2. We planned too much. FFT and interactive UI were planned initially, but since the PCB arrived too late (we were both busy at other courses), and migrating the hardware from F1 to F4 was troublesome, we gave up on these ideas.
3. The project is solely graded on the "embedded system" part, i.e. how your software interfaces with various hardware components. Making a fancy PCB **DOES NOT** contribute to the grade.

### Reflection by Stardust
*(Below is my very subjective, opinionated comments. Please read with care and discretion.)*

I'm not so gifted in embedded system programming. It took me a lot of time to pick up the fundamentals, not to mention googling as I work on the project. The overwhelming documentation scared me back then and I didn't see where I could start from. If you don't have a good track record in robotics or embedded systems programming, I suggest you to consider again whether to take the course, and how much effort you expect to devote to it.

For me, I took it for the Robotics minor, hoping that I could at least play with embedded system as a programmer for once, not as a hardware engineer. I don't regret taking it, but it was a solid evidence that I shouldn't go any further into embedded system :p
