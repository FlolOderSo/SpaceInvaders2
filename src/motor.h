#include <Arduino.h>

#define maxLeftRotation -5
#define maxRightRotation 5

#define left 34
#define right 35
#define shot 22

#define ena 2
#define in1 0
#define in2 33

const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

int rotation = 0;

bool isMoving = false;
unsigned long moveStartTime = 0;
const unsigned long moveDuration = 1000;
int moveDirection = 0; // -1 = links, 1 = rechts


void startMove(int direction) {
    moveDirection = direction;
    moveStartTime = millis();
    isMoving = true;
    int pwmSpeed = 0;

    if(direction == -1) {
        Serial.println("Motor dreht nach rechts");
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        pwmSpeed = 203;//208
    }
    else {
        Serial.println("Motor dreht nach links");
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        pwmSpeed = 204;//208
    }

    ledcWrite(pwmChannel, pwmSpeed);
}

void stopMotor() {
    ledcWrite(pwmChannel, 0);
    Serial.println("Motor gestoppt");
}

void motor() {
    unsigned long currentTime = millis();

    if(!isMoving) {
        // Button1: Linksbewegung
        if(digitalRead(left) == HIGH && rotation > maxLeftRotation) {
            startMove(-1);
        }

        // Button2: Rechtsbewegung
        if(digitalRead(right) == HIGH && rotation < maxRightRotation) {
            startMove(1);
        }
    }
    else {
        if(currentTime - moveStartTime >= moveDuration) {
            stopMotor();
            rotation += moveDirection;
            isMoving = false;
        }
    }
}