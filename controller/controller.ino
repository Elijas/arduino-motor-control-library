// (c) Elijas 2015 | github.com/Elijas
#include <Timer.h>
#include "Wheel.h"
#include "Options.h"

Timer timer;

Wheel leftWheel(PIN_MOTOR_LEFT_FWD, PIN_MOTOR_LEFT_BWD, MOTOR_LOWER_POWER_LIMIT, MOTOR_UPPER_POWER_LIMIT);
Wheel rightWheel(PIN_MOTOR_RIGHT_FWD, PIN_MOTOR_RIGHT_BWD, MOTOR_LOWER_POWER_LIMIT, MOTOR_UPPER_POWER_LIMIT);

//Helper functions to pass functions of objects as callback functions (which are required to be defined statically)
void ISR_leftEncoder()      {leftWheel.encoder.tickCount++;} //"interrupt service routine"
void ISR_rightEncoder()     {rightWheel.encoder.tickCount++;}
void TCB_updateLeftMotor()  {leftWheel.motor.update();} //"timer callback"
void TCB_updateRightMotor() {rightWheel.motor.update();}

#ifdef DEBUG
void dbg_out() {
    Serial.print(leftWheel.encoder.tickCount);
    Serial.print(" ");
    Serial.print(rightWheel.encoder.tickCount);
    Serial.print("\n");
}
void dbg_start() {
    leftWheel.motor.setPower(999);
    rightWheel.motor.setPower(0);
}
void dbg_start2() {
    leftWheel.motor.setPower(-999);
    rightWheel.motor.setPower(0);
}
void dbg_stop() {
    leftWheel.motor.setPower(17);
    rightWheel.motor.setPower(0);
}
#endif

void setup() {
    pinMode(10, OUTPUT); //irrelevant, turns off backlight of the LED screen

    Serial.begin(9600);
        
    attachInterrupt(INTERRUPT_NO_ENCODER_LEFT, ISR_leftEncoder, RISING);
    attachInterrupt(INTERRUPT_NO_ENCODER_RIGHT, ISR_rightEncoder, RISING);
    
    timer.every(MOTOR_POWER_DELAYSTEPUPDATE, TCB_updateLeftMotor);
    timer.every(MOTOR_POWER_DELAYSTEPUPDATE, TCB_updateRightMotor);
    
    #ifdef DEBUG
    //timer.every(600, dbg_out);
    timer.after(100, dbg_start);
    timer.after(3000, dbg_start2);
    timer.after(6000, dbg_stop);
    #endif
}

void loop() {timer.update();}

