#include <SoftwareSerial.h>
#include <Servo.h>

#define ECHOpin1 10 // Echo pin for distance sensor 1
#define TRIGpin1 9  // Trigger pin for distance sensor 1
#define ECHOpin2 12 // Echo pin for distance sensor 2
#define TRIGpin2 11 // Trigger pin for distance sensor 2

int servoPin = 8;
int servoAngle = 0;
long duration1, distance1;   
long duration2, distance2; 

Servo servoMotor;
SoftwareSerial mySerial(3, 2);

void setup() {
  pinMode(TRIGpin1, OUTPUT);  
  pinMode(ECHOpin1, INPUT);  
  pinMode(TRIGpin2, OUTPUT);  
  pinMode(ECHOpin2, INPUT);  

  servoMotor.attach(servoPin);
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);
}

void loop() {
  // Measure distance using sensor 1
  digitalWrite(TRIGpin1, LOW);  
  delayMicroseconds(2);  
  digitalWrite(TRIGpin1, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(TRIGpin1, LOW);  
  duration1 = pulseIn(ECHOpin1, HIGH);  
  distance1 = duration1 * (0.034/2); 

  if (distance1 < 20) {
    servoMotor.write(145);
    servoAngle = 145;
    delay(30000);
  }

  // Measure distance using sensor 2
  digitalWrite(TRIGpin2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGpin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGpin2, LOW);
  duration2 = pulseIn(ECHOpin2, HIGH);
  distance2 = duration2 * (0.034/2);

  // Send SMS if distance2 is less than or equal to 10
  if (distance2 <= 10) {
    sendSMS("SIR/MAM DUSTBIN IS FULL");
    delay(2000);
    //sendSMS("Welcome");
    delay(1000);
  }

  //Move servo back to its initial position
  servoMotor.write(0);
  servoAngle = 0;
  delay(1000);
}

void sendSMS(String Text) {
  mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+CMGS=\"+917448373370\""); // enter your phone number here (prefix country code)
  updateSerial();
  mySerial.print(Text); // enter your message here
  updateSerial();
  mySerial.write(26);
}

void updateSerial() {
  delay(500);
  while(mySerial.available()) {
    Serial.write(mySerial.read()); // Forward what Software Serial received to Serial Port
  }
}
