// (c) Author: Elijas (2015) github.com/Elijas
#include <Timer.h>
#include "Motor.h"
Timer timer;


#define PIN_FORWARD         2
#define PIN_BACKWARD        3  
#define LOWER_LIMIT         80   /MOTOR_POWER_STEPSIZE //integers from 0 to 255 (255 means 100% PWM duty cycle)
#define UPPER_LIMIT         170  /MOTOR_POWER_STEPSIZE
#define STEP_SIZE           5       //integer
#define DELAY_STEP_UPDATE   30      //ms
Motor motor(
    PIN_FORWARD,
    PIN_BACKWARD,
    LOWER_LIMIT,
    UPPER_LIMIT,
    STEP_SIZE,
    DELAY_STEP_UPDATE
);


void Example1() {
    leftMotor.set(100);
}
void Example2() {
    leftMotor.set(-100);
}
void Example3() {
    leftMotor.set(0);
}


void setup() {
    //temp
    pinMode(10, OUTPUT);
    Serial.begin(9600);

    timer.after(0,    Example1);
    timer.after(2000, Example2);
    timer.after(4000, Example3);
}
void loop() {timer.update();}









//Helper functions to pass functions of objects as callback functions (which are required to be defined statically)
//void TCB_updateLeftMotor()  {leftMotor.motor.update();} //"timer callback"
//void TCB_updateRightMotor() {rightMotor.motor.update();}
/*

    timer.every(MOTOR_POWER_DELAYSTEPUPDATE, TCB_updateLeftMotor);
    timer.every(MOTOR_POWER_DELAYSTEPUPDATE, TCB_updateRightMotor);




class RotaryEncoder { public: RotaryEncoder(uint8_t PinA, uint8_t PinB); void updateEncoder();

private: static RotaryEncoder* sEncoder; static void updateEncoderISR(); };


RotaryEncoder* RotaryEncoder::sEncoder = 0;

RotaryEncoder::RotaryEncoder(uint8_t PinA, uint8_t PinB) { ... 
sEncoder = this; 
attachInterrupt(0, RotaryEncoder::updateEncoderISR, CHANGE); 
attachInterrupt(1, RotaryEncoder::updateEncoderISR, CHANGE); }



*/

