#include <Servo.h> 

Servo servo;  
int angle = 0;
 
void setup() {
    pinMode(2, OUTPUT); // HC-SR04 trig
    pinMode(3, INPUT);  // HC-SR04 echo
    pinMode(8, INPUT);  // HC-05 state

    servo.attach(9); 
    Serial.begin(115200);
} 
 
void loop() {
    if(Serial.available() > 0) {
        //TODO: komendy
        servo.write(Serial.readString().toFloat()); 
    }

    if (digitalRead(8) == HIGH) {
        for(int angle = 0; angle <= 180; angle++) {                    
            digitalWrite(2, LOW);
            delayMicroseconds(2);
        
            digitalWrite(2, HIGH);
            delayMicroseconds(10);
            digitalWrite(2, LOW);
        
            long echoTime = pulseIn(3, HIGH);
            float distance = echoTime / 58.0;    

            servo.write(angle);
            Serial.println(distance);                
            delay(50);                   
        }
        for(int angle = 180; angle >= 0; angle--) {                    
            digitalWrite(2, LOW);
            delayMicroseconds(2);
        
            digitalWrite(2, HIGH);
            delayMicroseconds(10);
            digitalWrite(2, LOW);
        
            long echoTime = pulseIn(3, HIGH);
            float distance = echoTime / 58.0; 

            servo.write(angle);
            Serial.println(distance);                
            delay(50);                   
        } 
    }
}