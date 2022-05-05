// Libraries
#include <Wire.h>
#include <ZumoShield.h>
#include <HCSR04.h>

// border_detect_try
#define QTR_THRESHOLD  1500 // microseconds
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);


// hcsr04_martinsos
const byte triggerPin = A4;
const byte echoPin = A5;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);


// HC-SR04_UltrasonicSensorExample
const int TRIG_PIN = A4;
const int ECHO_PIN = A5;
const unsigned int MAX_DIST = 580;


int c=0;

void setup() {
  // put your setup code here, to run once:
  
  button.waitForButton();
//  pinMode(TRIG_PIN, OUTPUT);
//  digitalWrite(TRIG_PIN, LOW);
//  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  while (c == 0){
    motors.setSpeeds(200,-200);
    delay(5000);
    motors.setSpeeds(0,0);
    c=1;
  }
  while (c == 1){
    borderDetect();
}
  while (c == 2){
    motors.setSpeeds(-200,200);
//  buzzer.play(">g32>>c32");
}
}
void borderDetect() {
//  superSonicSensor();
//  hcsr04Ultrasonic();  

  sensors.read(sensor_values);
  
  if (sensor_values[0] < QTR_THRESHOLD)
  {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (sensor_values[5] < QTR_THRESHOLD)
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else
  {
    // otherwise, go straight
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
}

void superSonicSensor() {
  float distance = distanceSensor.measureDistanceCm();
  Serial.println(distance);
  if (distance < 10) {
    buzzer.play(">g32>>c32");
    delay(2000);
    buzzer.stopPlaying();  
    c=2; 
}
}

void hcsr04Ultrasonic(){
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  while ( digitalRead(ECHO_PIN) == 0 );
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;
  Serial.print(pulse_width);
  if(pulse_width < MAX_DIST) {
    motors.setSpeeds(0,0);
    buzzer.play(">g32>>c32");
    delay(2000);
    buzzer.stopPlaying(); 
    c=2;
  }
  delay(1000);  
}
