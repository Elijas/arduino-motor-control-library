#include <Timer.h>

#define PIN_MOTOR_FWD 5
#define PIN_MOTOR_BWD 6
#define PIN_CURRENTCHECK_1 A4
#define PIN_CURRENTCHECK_2 A3
#define ARRSIZE 20

static __inline__ int sort(int *d){
        int i, j;
        for (i = 1; i < ARRSIZE; i++) {
                int tmp = d[i];
                for (j = i; j >= 1 && tmp < d[j-1]; j--)
                        d[j] = d[j-1];
                d[j] = tmp;
        }
}

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
    int readNo = 0;
    int ARR[ARRSIZE];
    void readAnalogs() {
        ARR[readNo] = analogRead(PIN_CURRENTCHECK_1) - analogRead(PIN_CURRENTCHECK_2);
        //if (analogVal > 300) Serial.print("!"), blowFuse();
        if (++readNo > ARRSIZE) {
            readNo = 0;
            
            
            sort(ARR);
            
            int ARRavg = 0;
            for (int i=2; i<ARRSIZE-2; i++) {
                ARRavg += ARR[i];
            }
            ARRavg /= ARRSIZE-4;
            

            if (++outputNo > 45) {
                outputNo = 0;
                Serial.print("\n");
            }
            Serial.print(ARRavg);
            Serial.print(" ");
        }
      
        
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
    Serial.begin(115200);
    pinMode(PIN_MOTOR_FWD, OUTPUT);
    pinMode(PIN_MOTOR_BWD, OUTPUT);
    pinMode(10, OUTPUT);//for LCD display
    
    hb::setup(25,240);
    
    TEID_readSerialInput = t.every(1000, readSerialInput);
    TEID_hbupdate = t.every(100, hb::update);
    TEID_hbreadAnalogs = t.every(1, hb::readAnalogs);
}
void loop() {t.update();}
