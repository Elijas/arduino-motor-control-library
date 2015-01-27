// (c) Elijas 2015 | github.com/Elijas
#include <Timer.h>
#include "Motor.h"
#include "Options.h"

Timer timer;

Motor leftMotor(PIN_MOTOR_LEFT_FWD, PIN_MOTOR_LEFT_BWD, MOTOR_LOWER_POWER_LIMIT, MOTOR_UPPER_POWER_LIMIT);
Motor rightMotor(PIN_MOTOR_RIGHT_FWD, PIN_MOTOR_RIGHT_BWD, MOTOR_LOWER_POWER_LIMIT, MOTOR_UPPER_POWER_LIMIT);

//Helper functions to pass functions of objects as callback functions (which are required to be defined statically)
void TCB_updateLeftMotor()  {leftMotor.motor.update();} //"timer callback"
void TCB_updateRightMotor() {rightMotor.motor.update();}

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

void setup() {
    #ifdef DEBUG
    Serial.begin(9600);
    //timer.every(600, dbg_out);
    timer.after(100, dbg_start);
    timer.after(3000, dbg_start2);
    timer.after(6000, dbg_stop);
    #endif

    pinMode(10, OUTPUT); //irrelevant, turns off backlight of the LED screen

    attachInterrupt(INTERRUPT_NO_ENCODER_LEFT, ISR_leftEncoder, RISING);
    attachInterrupt(INTERRUPT_NO_ENCODER_RIGHT, ISR_rightEncoder, RISING);
    timer.every(MOTOR_POWER_DELAYSTEPUPDATE, TCB_updateLeftMotor);
    timer.every(MOTOR_POWER_DELAYSTEPUPDATE, TCB_updateRightMotor);
}

void loop() {timer.update();}

