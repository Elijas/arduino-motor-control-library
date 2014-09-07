#include <Timer.h>

#define PIN_MOTOR_FWD 11
#define PIN_MOTOR_BWD 12
#define PIN_CURRENTCHECK_1 A3
#define PIN_CURRENTCHECK_2 A4
#define SERIAL_PRINT(a) Serial.print(#a); Serial.print(" "); Serial.println(a)

Timer t;
int TEID_readSerialInput, TEID_readAnalogs;

int powerOfTen(int);
int readPosNumberFromSerial();
void readSerialInput();

int currPower = 0;
int targetPower = 0;
void initMotors();
void updateMotors();

int a1sum,a2sum,a2max,a2maxAvg;
int printItems = 0;

bool digitalFuseBlown = false;
void blowDigitalFuse();

void readAnalogs() {
    /*
    int avgSize = 1;
    a2maxAvg = 0;
    for (int i=0; i < avgSize && currPower; ) {
        if (printItems++ < 40) {
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
    
    //if (a2maxAvg > 1000 && !digitalFuseBlown) blowDigitalFuse();
    */
    
    const int iCount=10, jCount=5;
    int A1valAvg=0, A2valAvg=0, A1valMax=0, A2valMax=0;
    
    for (int i=0; i<iCount; i++) { //for (int j=0; j<jCount; j++) {
        int A1read = analogRead(PIN_CURRENTCHECK_1);
        int A2read = analogRead(PIN_CURRENTCHECK_2);
        
        A1valAvg+=A1read;
        A2valAvg+=A2read;
        if (A1valMax<A1read) A1valMax=A1read;
        if (A2valMax<A2read) A2valMax=A2read;
    }
    
    A1valAvg/=iCount;
    A2valAvg/=iCount;
    
    SERIAL_PRINT(A1valAvg);
    SERIAL_PRINT(A2valAvg);
    SERIAL_PRINT(A1valMax);
    SERIAL_PRINT(A2valMax);
}


void setup() {
    Serial.begin(57600);
    pinMode(PIN_MOTOR_FWD, OUTPUT);
    pinMode(10, OUTPUT);
    
    t.every(1000, readSerialInput);
    t.every(100, updateMotors);
    t.every(500, readAnalogs);
}
void loop() {t.update();}


void initMotors() {
    analogWrite(PIN_MOTOR_FWD, 10);
    delay(10);
    analogWrite(PIN_MOTOR_FWD, 0);
    delay(10);
    analogWrite(PIN_MOTOR_BWD, 10);
    delay(10);
    analogWrite(PIN_MOTOR_BWD, 0);
}
void updateMotors() {
    if (currPower < targetPower) analogWrite(PIN_MOTOR_FWD, ++currPower);
    else if (currPower > targetPower) analogWrite(PIN_MOTOR_FWD, --currPower);
}

void readSerialInput() {
    int serialInput = readPosNumberFromSerial();   
    if (serialInput != -1) {
        if (serialInput < 240) targetPower = serialInput;
        else if (serialInput == 1337) initMotors();
        else targetPower = 0;
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

void blowDigitalFuse() {
    targetPower = 0;
    currPower -= 10;
    updateMotors();
    
    t.stop(TEID_readSerialInput);
    
    Serial.println("Digital Fuse is Blown!");
    digitalFuseBlown = true;
}
