#include <Timer.h>

#define PIN_MOTOR_FWD 13
#define PIN_MOTOR_BWD 12
#define PIN_CURRENTCHECK_1 A13
#define PIN_CURRENTCHECK_2 A14

// TIMER
Timer t;
int TEID_readSerialInput, TEID_hbupdate, TEID_hbreadAnalogs; //TimerEventIDs

// HBRIDGE
namespace hb {
    int lowerPowerLimit;
    int upperPowerLimit;
    int currPower;
    int targetPower;
    bool fuseBlown;
    void setup(int lower, int upper) {
        currPower = 0;
        targetPower = 0;
        lowerPowerLimit = lower;
        upperPowerLimit = upper;
        fuseBlown = false;
    }
    void fix() { //quickfix for some unresponsiveness HW bug
        analogWrite(PIN_MOTOR_FWD, 10);
        delay(10);
        analogWrite(PIN_MOTOR_FWD, 0);
        delay(10);
        analogWrite(PIN_MOTOR_BWD, 10);
        delay(10);
        analogWrite(PIN_MOTOR_BWD, 0);
    }
    void setTargetPower (int p) {
        if (p == 0 || p >= lowerPowerLimit && p <= upperPowerLimit)
            targetPower = p;
    }
    void update() {
        if (currPower == 0 && targetPower >= lowerPowerLimit) analogWrite(PIN_MOTOR_FWD, currPower=lowerPowerLimit);
        else if (currPower == lowerPowerLimit && targetPower == 0) analogWrite(PIN_MOTOR_FWD, currPower=0);
        else if (currPower < targetPower) analogWrite(PIN_MOTOR_FWD, ++currPower);
        else if (currPower > targetPower) analogWrite(PIN_MOTOR_FWD, --currPower);
    }
    void changeCurrPower (int diff) {
        if (-10 < diff && diff < 10 && 0 <= currPower+diff && currPower+diff <= upperPowerLimit)
            analogWrite(PIN_MOTOR_FWD, currPower+=diff);
    }
    
    void blowFuse() {
        if (!fuseBlown) {
            fuseBlown = true;
        
            targetPower = 0;
            changeCurrPower(-10);
        
            t.stop(TEID_readSerialInput);
            Serial.print("X");
        }
    }
    
    int outputNo = 0;
    void readAnalogs() {
        int analogVal = analogRead(PIN_CURRENTCHECK_1);
        //if (analogVal > 300) Serial.print("!"), blowFuse();
      
        if (++outputNo > 20) {
            outputNo = 0;
            Serial.print("\n");
        }
        Serial.print(analogVal);
        Serial.print(" ");
    }
}

// SERIAL
int powerOfTen(int a) {
    if (a>0) return 10*powerOfTen(a-1);
    else return 1;
}
void readSerialInput() {
    if (Serial.available()) {
        int serialInput = 0;
        while (Serial.available() > 0) {
            serialInput += (Serial.read()-'0') * powerOfTen(Serial.available());
        }

        if (serialInput == 1337) hb::fix();
        else if (serialInput == 9001) hb::blowFuse();
        else hb::setTargetPower(serialInput);
    }
}

void setup() {
    Serial.begin(57600);
    pinMode(PIN_MOTOR_FWD, OUTPUT);
    pinMode(PIN_MOTOR_BWD, OUTPUT);
    pinMode(10, OUTPUT);//for LCD display
    
    hb::setup(40,240);
    
    TEID_readSerialInput = t.every(1000, readSerialInput);
    TEID_hbupdate = t.every(100, hb::update);
    TEID_hbreadAnalogs = t.every(200, hb::readAnalogs);
}
void loop() {t.update();}
