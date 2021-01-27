#include <Servo.h> 

Servo servo;  
int angle = 0;
bool right = true;
bool start = false;
 
void setup() {
    pinMode(2, OUTPUT); // HC-SR04 trig
    pinMode(3, INPUT);  // HC-SR04 echo
    pinMode(8, INPUT);  // HC-05 state

    servo.attach(9); 
    Serial.begin(115200);
} 
 
void loop() {
    if(Serial.available() >= 4) {
        String data = Serial.readString();

        if (data == "s000") {
            start = false;
        } else if (data == "s001") {
            start = true;
        } else {
            servo.write(data.toFloat());
        }
    }

    if (!start || digitalRead(8) == LOW) {
        delay(50);
        return;
    }

    float distance = measureDistance();
    Serial.println(distance);                
    delay(50);    
}

float measureDistance() {
    digitalWrite(2, LOW);
    delayMicroseconds(2);
        
    digitalWrite(2, HIGH);
    delayMicroseconds(10);
    digitalWrite(2, LOW);
        
    long echoTime = pulseIn(3, HIGH);
    float distance = echoTime / 58.0;

    return distance;
}