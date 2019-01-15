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
Specifications [here](https://github.com/fafux/EmbeddedSystemRobEng2018-19/blob/master/Progetto.pdf)
### Features:
* Receive Reference [100ms]
* Refreshing PWM [100ms]
* Acquire Temperature [100ms]
* Send Temperature (MCTERM) [1s]
* Send Feedback Message MCFBK [200ms]
* Blink Led for status (Control mode or timeout mode) [1s]
* Check/Read message of type MLSAT [100ms]
* S5/S6 button linked to interrupt to enter Safe mode
* Write on LCD :
    + Status (each time status change)
    + Temperature (each time it send temp (4th task))
    + Write references applied (each time pwm is refreshed (2nd task))

* Send MCACK message to pc through UART

### Details
* Scheduler with heartbit of 100ms to manage all the task
* Five Tasks:
    * Read/process message arrived from UART
        * If references arrives, update PWM signals to control the motors
    * Acquire Temperature from sensor
    * Average last 10 temperature readings and send the mean to the UART
    * Send a feedback FBK message
    * Blink D3 and D4 leds to indicate correct running and timeout state
    * Task to write correctly on the LCD display
* Three Interrupts:
    * To save incoming bytes from UART in a buffer (if registers are full)
    * On S5 and S6 buttons, to immediately enter in safe mode
    * On a timer, to check if 5 seconds passed without references, to enter in timeout mode
* Peripherals used:
    * D3 D4 leds
    * S4 S5 buttons
    * LCD display controlled throught SPI
    * UART to send and receive messages
    * PWM to generate signals (different duty cicle ratio) to control the motors
    * Temperature Sensor
    * Two timers: one for scheduler, one for check timeout mode

## Contributors
* [torydebra](https://github.com/torydebra/)
* [fafux](https://github.com/fafux/)
