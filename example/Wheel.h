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
    Motor(int,int,int,int,int,int,int,Timer*);
    void set(int);
    void updatePin();
    
    private:
    void pinWrite(int);

    //Necessary to pass to pass updatePin() function as Timer callback function
    static void updatePin_motor0();
    static Motor* ptr_motor0;

    static void updatePin_motor1();
    static Motor* ptr_motor1;
};

Motor* Motor::ptr_motor0;
Motor* Motor::ptr_motor1;

Motor::Motor (int motorID, int set_pinForward, int set_pinBackward,
              int set_lowerLimit, int set_upperLimit, int set_stepSize,
              int delayOfStepUpdate, Timer* t
              ) : currStep     (0),
                  targetStep   (0),
                  pinForward   (set_pinForward),
                  pinBackward  (set_pinBackward),
                  lowerLimit   (set_lowerLimit),
                  upperLimit   (set_upperLimit),
                  stepSize     (set_stepSize)
{
    if (motorID == 0) {
        ptr_motor0 = this;
        t -> every(delayOfStepUpdate, Motor::updatePin_motor0);
    }
    else if (motorID == 1) {
        ptr_motor1 = this;
        t -> every(delayOfStepUpdate, Motor::updatePin_motor1);
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

void Motor::updatePin_motor0() {
    Motor::ptr_motor0 -> updatePin();
}
void Motor::updatePin_motor1() {
    Motor::ptr_motor1 -> updatePin();
}
#endif

