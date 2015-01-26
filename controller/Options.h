// (c) Elijas 2015 | github.com/Elijas
#ifndef OPTIONS
#define OPTIONS

#define DEBUG

#define INTERRUPT_NO_ENCODER_LEFT 4 //quickref: interrupt No. 4 is pin 19 on Arduino Mega (No.5 is pin 18)
#define INTERRUPT_NO_ENCODER_RIGHT 5
#define PIN_MOTOR_LEFT_FWD 2
#define PIN_MOTOR_LEFT_BWD 3
#define PIN_MOTOR_RIGHT_FWD 11
#define PIN_MOTOR_RIGHT_BWD 12
#define MOTOR_LOWER_POWER_LIMIT 80  /MOTOR_POWER_STEPSIZE //integers from 0 to 255 (255 means 100% PWM duty cycle)
#define MOTOR_UPPER_POWER_LIMIT 170  /MOTOR_POWER_STEPSIZE
#define MOTOR_POWER_STEPSIZE 5 //integer
#define MOTOR_POWER_DELAYSTEPUPDATE 30 //ms

#endif

