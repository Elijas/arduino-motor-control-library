// (c) Author: Elijas (2015) // github.com/Elijas //
#include <Timer.h>
#include "Wheel.h"

#define PIN_FORWARD         2
#define PIN_BACKWARD        3  
#define LOWER_LIMIT         80   /STEP_SIZE //integers from 0 to 255 (255 means 100% PWM duty cycle)
#define UPPER_LIMIT         170  /STEP_SIZE
#define STEP_SIZE           5       //integer
#define DELAY_STEP_UPDATE   30      //ms
Timer timer;

Motor motor(PIN_FORWARD,
            PIN_BACKWARD,
            LOWER_LIMIT,
            UPPER_LIMIT,
            STEP_SIZE,
            DELAY_STEP_UPDATE,
            &timer
            );

void Example1() {
    motor.set(100);
}
void Example2() {
    motor.set(-100);
}
void Example3() {
    motor.set(0);
}

void setup() {
    //<TEMPORARY>
    pinMode(10, OUTPUT);
    Serial.begin(9600);
    //</TEMPORARY>

    timer.after(1000, Example1);
    timer.after(2000, Example2);
    timer.after(3000, Example3);
}
void loop() {timer.update();}

