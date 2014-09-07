#include <Timer.h>

#define PIN_MOTOR_FWD 11
#define PIN_MOTOR_BWD 12
#define PIN_CURRENTCHECK_1 A13
#define PIN_CURRENTCHECK_2 A15

Timer t;

int powerOfTen(int);
int readPosNumberFromSerial();
void readSerialInput();

int currPower = 0;
int targetPower = 0;

int a1sum,a2sum,a2max,a2maxAvg;
int printItems = 0;

void readAnalogs() {
    int avgSize = 1;
    a2maxAvg = 0;
    for (int i=0; i < avgSize && currPower; ) {
        if (printItems++ < 100) {
            int a2 = analogRead(PIN_CURRENTCHECK_2);
            if (a2 > a2max) a2max = a2;
        }
        else {
            //if (a2max) Serial.println(a2max);
            //else Serial.print("-");
            if (a2max) {
                i++;
                a2maxAvg += a2max;
            }
            a2max=0;
            printItems=0;
        }
    }
    a2maxAvg/=avgSize;
    Serial.println(a2maxAvg);
}

void printSums() {
    //Serial.print(currPower);
    //Serial.print(" "); 
    //Serial.print(a1sum); 
    //Serial.print(" "); 
    //Serial.print(a2sum);
    //Serial.print("\n"); 
}

void updateMotors() {
    if (currPower < targetPower) analogWrite(PIN_MOTOR_FWD, ++currPower);
    else if (currPower > targetPower) analogWrite(PIN_MOTOR_FWD, --currPower);
}

void initMotors() {
    analogWrite(PIN_MOTOR_FWD, 10);
    delay(10);
    analogWrite(PIN_MOTOR_FWD, 0);
    delay(10);
    analogWrite(PIN_MOTOR_BWD, 10);
    delay(10);
    analogWrite(PIN_MOTOR_BWD, 0);
}

void setup() {
    Serial.begin(57600);
    pinMode(PIN_MOTOR_FWD, OUTPUT);
    pinMode(10, OUTPUT); // turns LCD backlight off
    
    t.every(1000, readSerialInput);
    t.every(100, updateMotors);
    t.every(1, readAnalogs);
    //t.every(1, printSums);
}
void loop() {t.update();}




/// SERIAL COMMS
void readSerialInput() {
    int serialInput = readPosNumberFromSerial();   
    if (serialInput != -1) {
        if (serialInput < 240) targetPower = serialInput;
        else if (serialInput == 1337) initMotors();
        else targetPower = 0;
        //Serial.println(targetPower);
    }
}
int powerOfTen(int a) {
    if (a>0) return 10*powerOfTen(a-1);
    else return 1;
}
int readPosNumberFromSerial() {
    if (Serial.available()) {
        int number = 0;
        while (Serial.available() > 0) {
            number += (Serial.read()-'0') * powerOfTen(Serial.available());
        }
        return number;
    }
    else return -1;
}

