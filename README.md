
**Title:** Digital control algorithms of the solar panel position with a sun position tracking

**Description:**
  Diploma thesis written in 2022. I am not fully satisfied with the results of the work. The repository is intended to provide a reference point for my development as a software developer and electronics engineer.

Contains 3 programs:

**sun_position_calculator** - pyhton script -
 Determines the position of the sun and sends the data to the steering program via the serial bus.
  
  **steering_program** - arduino - Program controlling the stepper motor and servo. It receives data from a program that calculates the sun's position, performs calibration and moves a set of photovoltaic panels.
  
  **measuring_device** - arduino -  Program that allows you to measure the amount of energy generated by a photovoltaic panel (or other devices). Supports up to two devices at the same time.
