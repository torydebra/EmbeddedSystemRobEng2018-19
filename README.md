# EmbeddedSystemRobEng2018-19
Laboratiories with MPLAB-X in C language for Embedded System course of Robotics Engineering 2018/19 

The board used is a Microchip dsPIC30F4011 with pickit3 programmer.  
Note: only the mains are uploaded on Github, all the other stuff built automatically by the IDE are omitted.

# Laboratories:

* LAB1: Use of leds and buttons
* LAB2: Timers
* LAB3: Interrupts
* LAB4: use of display: SPI
* LAB5: UART to conect the board with the outside (a pc in our case)
* LAB6: ADC : analog to digital converter 
* LAB7: PWM : generate a pulse width modulated signal to control the velocity of a servomotor.

# Final project:
## Board to control autonomous catamaran

### Tasks:
* Receive Reference [100ms]
* Refreshing PWM [100ms]
* Acquire Temperature [100ms]
* Send Temperature (MCTERM) [1s]
* Send Feedback Message MCFBK [200ms]
* Blink Led for status (Control mode or timeout mode) [1s]
* Check/Read message of type MLSAT [100ms]

### Other functionalities:
S5/S6 button linked to interrupt to enter Safe mode

Write on LCD :
+ first row: 
    + print status (each time status change)
    + print temperature (each time it send temp (4th task))
+ second row:
    + write references applied (each time pwm is refreshed (2nd task))

MCACK message: sent in various tasks

### Detail
Scheduler with heartbit of 100ms
