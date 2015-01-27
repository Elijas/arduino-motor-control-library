// (c) Author: Elijas (2015) github.com/Elijas
#ifndef WHEEL
#define WHEEL
#include <Arduino.h>

class Motor {
    int pinForward,
        pinBackward,
        lowerLimit,
        upperLimit,
        stepSize,
        delayStepUpdate;
    
    public:
    Motor(int,int,int,int,int,int);
    set(int);
    updatePin();
    
    private:
    pinWrite(int);
}

Motor::Motor (int arg_pinForward,int arg_pinBackward,int arg_lowerLimit,int arg_upperLimit,int arg_stepSize,int arg_delayStepUpdate) {
    pinForward      = arg_pinForward;
    pinBackward     = arg_pinBackward;
    lowerLimit      = arg_lowerLimit;
    upperLimit      = arg_upperLimit;
    stepSize        = arg_stepSize;
    delayStepUpdate = arg_delayStepUpdate;
}

void Motor::set(int power) {
    step = power / stepSize;
    
    if (step > 0) {
        if      (step < lowerLimit) 
            targetStep = 0;
        else if (step > upperLimit) 
            targetStep = upperLimit;
        else 
            targetStep = step;
    }
    else {
        if      (step > -lowerLimit)
            targetStep = 0;
        else if (step < -upperLimit)
            targetStep = upperLimit;
        else
            targetStep = step;
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
    #if 0
    if (power >= 0) analogWrite(pinFwd, power);
    else            analogWrite(pinBwd, -power);
    #else
    if (power >= 0) Serial.println(power*MOTOR_POWER_STEPSIZE);
    else            Serial.println(power*MOTOR_POWER_STEPSIZE);
    #endif
}
#endif

