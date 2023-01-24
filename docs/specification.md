# Specification of data physicalization "Grow Active"

## Main components

The physicalization consists of a digital and a physical part.

### Digital

The digital part consists of:
- a REST endpoint on the FitBit server,
- a web server `(1)`, and
- an ESP32-based development board (e.g. *M5Stack Atom*) running the main program and controlling the stepper **(2)**.

### Physical

The physical part consists of:
- a *FitBit Sense* smartwatch,
- a stepper motor with a driver,
- a power supply for driving the motor, and
- a (yet not fully specified) dynamic physical object, whose appearance can be changed by the stepper.

## Data flow

**Quantity of interest**: *total minutes in custom heart rate zone* (e.g. 100-220 BPM).

The following data flow describes the process:

1. heart beats
2. → *FitBit Sense* tracks heart rate
3. → syncs captured time series with FitBit's server
4. → web server `(1)` performs OAuth2 authorization with FitBit's server, acquires *Access* and *Refresh Token*
5. → if *Access Token* is expired, *Refresh Token* is used to acquire new pair of tokens
6. → fetch JSON from endpoint `GET /1/user/-/activities/heart/date/[date]/[period].json`
7. → extract `activities-heart:value:customHeartRateZone:minutes`
8. → calculate number of steps `diff` from `current_position` and store it permanently
9. → ESP32 authorizes with web server
10. → fetch JSON from endpoint `[not yet specified]`
11. → move stepper `diff` steps from `current_position` to `new_position`
12. → store `new_position = current_position (in steps) + diff` permanently in ESP32 flash memory
13. → if 30 minutes of activity are reached, stop requesting data from server

On date change:

Web server:
1. store date and achieved minutes in e.g. a CSV file
2. set `current_position = -current_position` (so on next data fetch, stepper will go all the way back to initial position)

ESP32:
1. → return to `position0` by going `current_position` steps in opposite direction (alternative to fetching steps back, but then the ESP32 needs to be aware of the current time and date)
2. → reset `current_position` to 0 in flash memory (also not necessary if `-current_position` is fetched from server)

In case ESP32 turns off accidentally:

- read `current_position` from flash memory and assume it is correct (so it did not turn off between moving to new position and writing it to flash memory)
- (or fetch `current_position` from server and compare to value from memory)
- continue normally
