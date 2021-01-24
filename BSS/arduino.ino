#include <Servo.h> 

int servoPin = 9;

Servo servo;  

int angle = 0;   // servo position in degrees 
 
void setup() 
{
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);

  servo.attach(servoPin); 
    // scan from 0 to 180 degrees
  Serial.begin(115200);
} 
 
 
void loop() {
  if(Serial.available() > 0) {
    servo.write(Serial.readString().toFloat()); 
    digitalWrite(7, HIGH);
  }

  if (digitalRead(8) == HIGH) {
      for(angle = 0; angle < 180; angle++)  
  {                    
    digitalWrite(2, LOW);
    delayMicroseconds(2);
   
    digitalWrite(2, HIGH);
    delayMicroseconds(10);
    digitalWrite(2, LOW);
   
    long echoTime = pulseIn(3, HIGH);
    float distance = echoTime / 58.0;              
    servo.write(angle);
        Serial.println(String(distance) + "," + String(angle-1));                
    delay(15);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 0; angle--)    
  {                                
    digitalWrite(2, LOW);
    delayMicroseconds(2);
   
    digitalWrite(2, HIGH);
    delayMicroseconds(10);
    digitalWrite(2, LOW);
   
    long echoTime = pulseIn(3, HIGH);
    float distance = echoTime / 58.0;              
    servo.write(angle);
    Serial.println(String(distance) + "," + String(angle-1));               
    delay(15);      
  }
  }
}
