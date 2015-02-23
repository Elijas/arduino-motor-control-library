// (c) Author: Elijas (2015) // github.com/Elijas //
#include <Timer.h>
#include "Motor.h"

#define LOWER_LIMIT         15   /STEP_SIZE // integers from 0 to 255 (default: 0)
#define UPPER_LIMIT         200  /STEP_SIZE // integers from 0 to 255 (default: 255)
#define STEP_SIZE           5               // integer
#define DELAY_STEP_UPDATE   30              // ms

#define MOTOR0_FWD_PIN      2
#define MOTOR0_BWD_PIN      3
#define MOTOR1_FWD_PIN      11
#define MOTOR1_BWD_PIN      12

Timer timer;

// NB: Initialize as a global object (because accidental destruction of Motor class object may cause unexpected behaviour, as Timer is now pointed to a member function of a non-existing object)
Motor motor0(0, MOTOR0_FWD_PIN, MOTOR0_BWD_PIN, LOWER_LIMIT, UPPER_LIMIT, STEP_SIZE, DELAY_STEP_UPDATE, &timer); //first argument - unique motor ID number
//Motor motor1(1, MOTOR1_FWD_PIN, MOTOR1_BWD_PIN, LOWER_LIMIT, UPPER_LIMIT, STEP_SIZE, DELAY_STEP_UPDATE, &timer);

void Example_step1() {
    motor0.set(30); // Sets power for motors. Integer in range [-255;255], sign means direction, number represents PWM duty cycle (255 is 100%)
}
void Example_step2() {
    motor0.set(-20);
}
void Example_step3() {
    motor0.set(0);
}

void setup() {
    //Serial.begin(9600);

    timer.after(1000, Example_step1);
    timer.after(2000, Example_step2);
    timer.after(3000, Example_step3);
}
void loop() {timer.update();}

