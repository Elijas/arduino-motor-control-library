#define PIN_MOTOR_FWD 2
#define PIN_MOTOR_BWD 3
#define PIN_ANALOGREAD_1 A14
#define PIN_ANALOGREAD_2 A15

int powerOfTen(int);
int readPosNumberFromSerial();

void setup() {
    Serial.begin(9600);
    pinMode(PIN_MOTOR_FWD, OUTPUT);
    pinMode(PIN_MOTOR_BWD, OUTPUT);
}

void loop() {
    int targetPower = readPosNumberFromSerial();   
    if (targetPower != -1) {
        analogWrite(PIN_MOTOR_FWD, targetPower);
        Serial.println(targetPower);
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
            number += (Serial.read()-'0') * powerOfTen(Serial.available()-1);
        }
        return number;
    }
    else return -1;
}

