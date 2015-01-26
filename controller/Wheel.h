// (c) Elijas 2015 | github.com/Elijas
#ifndef WHEEL
#define WHEEL

#include <Arduino.h>
#include "Options.h"

struct Encoder {
    volatile int tickCount;
    Encoder() : tickCount(0){} //set variable default initialization values
};

struct Motor {
    int currStep, targetStep; //Power "steps". Actual power outputted is currStep*MOTOR_POWER_STEPSIZE
    int pinFwd, pinBwd;    
    int powerLimit[2]; //lower power limit indexed 0, and upper limit 1
    Motor() : currStep(0), targetStep(0){}
    
    void init(int pinMotorFwd, int pinMotorBwd, int motorLowerPowerLimit, int motorUpperPowerLimit) {
        pinFwd = pinMotorFwd;
        pinBwd = pinMotorBwd;
        powerLimit[0] = motorLowerPowerLimit;
        powerLimit[1] = motorUpperPowerLimit;
    }

    void setPower(int p) {
        p = p / MOTOR_POWER_STEPSIZE;

        if (p > 0) {
            if      (p < powerLimit[0]) targetStep = 0;
            else if (p > powerLimit[1]) targetStep = powerLimit[1];
            else targetStep = p;
        }
        else {
            if      (p > -powerLimit[0]) targetStep = 0;
            else if (p < -powerLimit[1]) targetStep = powerLimit[1];
            else targetStep = p;
        }      
    }

    void pinWrite(int power) {
        #ifndef DEBUG
        if (power >= 0) analogWrite(pinFwd, power);
        else            analogWrite(pinBwd, -power);
        #else
        if (power >= 0) Serial.println(power*MOTOR_POWER_STEPSIZE);
        else            Serial.println(power*MOTOR_POWER_STEPSIZE);
        #endif
    }

    void update() {
        //Jumps through gap of value between 0 and lower power limit or vice versa
        if      (currStep == 0              &&  currStep < targetStep) pinWrite(currStep = powerLimit[0]);
        else if (currStep == 0              &&  currStep > targetStep) pinWrite(currStep = -powerLimit[0]);
        else if (currStep == powerLimit[0]  &&  currStep > targetStep) pinWrite(currStep = 0);
        else if (currStep == -powerLimit[0] &&  currStep < targetStep) pinWrite(currStep = 0);
        else if (currStep < targetStep) pinWrite(++currStep);
        else if (currStep > targetStep) pinWrite(--currStep);
    }
};

struct Wheel {
    Encoder encoder;
    Motor motor;

    Wheel(int pinMotorFwd, int pinMotorBwd, int motorLowerPowerLimit, int motorUpperPowerLimit) {
        motor.init(pinMotorFwd, pinMotorBwd, motorLowerPowerLimit, motorUpperPowerLimit);
    }
};
#endif

