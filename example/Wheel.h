// (c) Author: Elijas (2015) // github.com/Elijas //
#ifndef WHEEL
#define WHEEL
#include <Arduino.h>
#include <Timer.h>

class Motor {
    int currStep, targetStep;
    const int pinForward,
              pinBackward,
              lowerLimit,
              upperLimit,
              stepSize;

    public:
    Motor(int,int,int,int,int,int,Timer*,int);
    void set(int);
    void updatePin();
    
    private:
    void pinWrite(int);

    //Necessary to pass to pass updatePin() function as Timer callback function
    static Motor* static_ptr_motor;
    static Motor* static_ptr_motor2;
    static void static_updatePin();
    static void static_updatePin2();
};

Motor* Motor::static_ptr_motor;
Motor* Motor::static_ptr_motor2;

Motor::Motor (int arg_pinForward, int arg_pinBackward, int arg_lowerLimit, 
              int arg_upperLimit, int arg_stepSize, 
              int delayStepUpdate, Timer* ptr_timer, int motorID)
  : currStep     (0),
    targetStep   (0),
    pinForward   (arg_pinForward),
    pinBackward  (arg_pinBackward),
    lowerLimit   (arg_lowerLimit),
    upperLimit   (arg_upperLimit),
    stepSize     (arg_stepSize) 
{
    if (motorID==0) {
        static_ptr_motor = this;
        ptr_timer -> every(delayStepUpdate, Motor::static_updatePin);
    }
    else if (motorID==1) {
        static_ptr_motor2 = this;
        ptr_timer -> every(delayStepUpdate, Motor::static_updatePin2);
    }
}

void Motor::set(int power) {
    int s = power / stepSize;
    
    if (s > 0) {
        if      (s < lowerLimit) targetStep = 0;
        else if (s > upperLimit) targetStep = upperLimit;
        else                     targetStep = s;
    }
    else {
        if      (s > -lowerLimit) targetStep = 0;
        else if (s < -upperLimit) targetStep = upperLimit;
        else                      targetStep = s;
    }      
}

void Motor::updatePin() {
    if      (currStep == 0           &&  currStep < targetStep) pinWrite(currStep = lowerLimit);
    else if (currStep == 0           &&  currStep > targetStep) pinWrite(currStep = -lowerLimit);
    else if (currStep == lowerLimit  &&  currStep > targetStep) pinWrite(currStep = 0);
    else if (currStep == -lowerLimit &&  currStep < targetStep) pinWrite(currStep = 0);
    else if (currStep < targetStep) pinWrite(++currStep);
    else if (currStep > targetStep) pinWrite(--currStep);
}

void Motor::pinWrite(int power) {
    //if (power >= 0) analogWrite(pinFwd, power*stepSize);
    //else            analogWrite(pinBwd, -power*stepSize);
    //<TEMPORARY>
    if (power >= 0) Serial.println(power*stepSize);
    else            Serial.println(power*stepSize);
    //</TEMPORARY>
}

void Motor::static_updatePin() {
    Motor::static_ptr_motor -> updatePin();
}
void Motor::static_updatePin2() {
    Motor::static_ptr_motor2 -> updatePin();
}
#endif

