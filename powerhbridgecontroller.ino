#include <Timer.h>

#define PIN_MOTOR_FWD 2
#define PIN_CURRENTCHECK_1 A3
#define PIN_CURRENTCHECK_2 A4

Timer t;
int powerOfTen(int);
int readPosNumberFromSerial();
void readSerialInput();


int currPower = 0;
int targetPower = 0;
int a1sum,a2sum; //for better visibility


void readAnalogs() {
    int a1 = analogRead(PIN_CURRENTCHECK_1), a2 = analogRead(PIN_CURRENTCHECK_2);
    a1sum+=a1;
    a2sum+=a2;
    
    /*
    Serial.print(currPower); 
    Serial.print(" "); 
    Serial.print(a1); 
    Serial.print(" "); 
    Serial.println(a2);
    */
}

void printSums() {
    Serial.print(currPower); 
    Serial.print(" "); 
    Serial.print(a1sum); 
    Serial.print(" "); 
    Serial.println(a2sum);
    
    a1sum=a2sum=0;
}

void updateMotors() {
    if (currPower < targetPower) analogWrite(PIN_MOTOR_FWD, ++currPower);
    else if (currPower > targetPower) analogWrite(PIN_MOTOR_FWD, --currPower);
}


void setup() {
    Serial.begin(57600);
    pinMode(PIN_MOTOR_FWD, OUTPUT);
    pinMode(10, OUTPUT); // turns LCD backlight off
    
    t.every(1000, readSerialInput);
    t.every(100, updateMotors);
    //t.every(1, readAnalogs);
    //t.every(100, printSums);
}
void loop() {t.update();}




/// SERIAL COMMS
void readSerialInput() {
    int serialInput = readPosNumberFromSerial();   
    if (serialInput != -1) {
        if (serialInput < 0 && serialInput > 240) targetPower = 0; //safeguard against unexpected values
        else targetPower = serialInput;
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

