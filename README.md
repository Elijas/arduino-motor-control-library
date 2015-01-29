# Description:
This is an arduino library that allows the power given to motors increase/decrease gradually (and not instantaneously as with analogWrite() function which this library is meant to replace).
# Features:
 - Lower and upper power limits (e.g. for motors that are able spin only when PWM duty cycle is above some threshold, (i.e. when the value is big enough in analogWrite(pin,value)))
 - Adjustable rate of power change (by changing step size and/or update delay)
 - Any number of motors (currently two are supported, modify header to allow for more)

# Usage:
```
Timer timer;
Motor myMotor(0, 2, 3, 0, 255, 1, 30, &timer);

myMotor.set(255)
```
(see example code for details)

(NB: It is necessary for [Timer](http://playground.arduino.cc/Code/Timer) library to be [added](http://arduino.cc/en/Guide/Libraries) to arduino)

