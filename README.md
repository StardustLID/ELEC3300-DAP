# ELEC3300-DAP
The repo for the Digital Audio Player (DAP) project for HKUST ELEC3300 (Fall 2021-22).

## Repo Management
- [`main`](https://github.com/StardustLID/ELEC3300-DAP/tree/main) branch should **ALWAYS** contain working and runnable code for demo.
- [`develop`](https://github.com/StardustLID/ELEC3300-DAP/tree/develop) branch should **ALWAYS** contain code ready for collaboration.
- Please create a separate branch from `develop` for each feature (e.g. [`lcd-menu`](https://github.com/StardustLID/ELEC3300-DAP/tree/lcd-menu), and raise a pull request (PR) before merging into `develop`.

## STM32 Models and Pins
- Currently this repo uses STM32F103VET6. Later when F4 arrives, the pinouts and dependencies will be changed accordingly.
- All pinouts follow those in labs and development board unless otherwise stated (e.g. same FSMC and GPIO config for LCD).

## Code Generation
- The paths in `.mxproject` seem to depend on the person who generated the code. Please tell me if it causes any difficulty.