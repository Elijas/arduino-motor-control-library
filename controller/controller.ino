// (c) Author: Elijas (2015) github.com/Elijas
#include <Timer.h>
#include "Motor.h"

#define PIN_FORWARD         2
#define PIN_BACKWARD        3  
#define LOWER_LIMIT         80   /MOTOR_POWER_STEPSIZE //integers from 0 to 255 (255 means 100% PWM duty cycle)
#define UPPER_LIMIT         170  /MOTOR_POWER_STEPSIZE
#define STEP_SIZE           5       //integer
#define DELAY_STEP_UPDATE   30      //ms

Timer timer;

Motor motor(
    PIN_FORWARD,
    PIN_BACKWARD,
    LOWER_LIMIT,
    UPPER_LIMIT,
    STEP_SIZE,
    DELAY_STEP_UPDATE
);


void startExample() {
    leftMotor.set(100);
}

void stopExample() {
    rightMotor.set(100)
}

void setup() {
    #ifdef DEBUG
    Serial.begin(9600);
    //timer.every(600, dbg_out);
    timer.after(100, dbg_start);
    timer.after(3000, dbg_start2);
    timer.after(6000, dbg_stop);
    #endif

    pinMode(10, OUTPUT); //irrelevant, turns off backlight of the LED screen

    timer.every(MOTOR_POWER_DELAYSTEPUPDATE, TCB_updateLeftMotor);
    timer.every(MOTOR_POWER_DELAYSTEPUPDATE, TCB_updateRightMotor);
}

void loop() {timer.update();}

//Helper functions to pass functions of objects as callback functions (which are required to be defined statically)
//void TCB_updateLeftMotor()  {leftMotor.motor.update();} //"timer callback"
//void TCB_updateRightMotor() {rightMotor.motor.update();}
/*
#ifdef DEBUG
void dbg_out() {
    Serial.print(leftMotor.encoder.tickCount);
    Serial.print(" ");
    Serial.print(rightMotor.encoder.tickCount);
    Serial.print("\n");
}
void dbg_start() {
    leftMotor.motor.setPower(999);
    rightMotor.motor.setPower(0);
}
void dbg_start2() {
    leftMotor.motor.setPower(-999);
    rightMotor.motor.setPower(0);
}
void dbg_stop() {
    leftMotor.motor.setPower(17);
    rightMotor.motor.setPower(0);
}
#endif
*/
