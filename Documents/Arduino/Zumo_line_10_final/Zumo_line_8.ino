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

// LineFollower_try
ZumoReflectanceSensorArray reflectanceSensors;
int lastError = 0;
const int MAX_SPEED = 200;

// hcsr04_martinsos
const byte triggerPin = A4;
const byte echoPin = A5;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);


// HC-SR04_UltrasonicSensorExample
const int TRIG_PIN = A4;
const int ECHO_PIN = A5;
const unsigned int MAX_DIST = 800;


int c=0;

const char march[] PROGMEM = 
"! O2 T100 MS"
"a8. r16 a8. r16 a8. r16 f8 r16 >c16" 
"ML"
"a8. r16 f8 r16" "MS" ">c16 a. r8"
"O3"
"e8. r16 e8. r16 e8. r16 f8 r16 <c16"
"O2 ML"
"a-8. r16" "MS" "f8 r16 >c16 a r"

"O3 ML"
"a8. r16 <a8 r16 <a16" "MS" "a8. r16 a-8 r16 g16"
"ML V10"
"g-16 f16 g-16 r16 r8 <b-16 r16" "MS" "e-8. r16 d8." "ML" "d-16"
"c16 <c-16 c16 r16 r8" "MS O2" "f16 r16 a-8. r16 f8. a-16"
"O3"
"c8. r16 <a8 r16 c16 e2. r8"

"O3 ML"
"a8. r16 <a8 r16 <a16" "MS" "a8. r16 a-8 r16 g16"
"ML V10"
"g-16 f16 g-16 r16 r8 <b-16 r16" "MS" "e-8. r16 d8." "ML" "d-16"
"c16 <c-16 c16 r16 r8" "MS O2" "f16 r16 a-8. r16 f8. >c16"
"ML"
"a8. r16 f8 r16 >c16 a2. r8"
;

void borderDetect() {
  
  sensors.read(sensor_values); 
  
  if (sensor_values[0] < QTR_THRESHOLD )
  {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (sensor_values[5] < QTR_THRESHOLD )
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

//void superSonicSensor() {
//  float distance = distanceSensor.measureDistanceCm();
//  if (distance < 10) {
//    buzzer.play(">g32>>c32");
//    delay(2000);
//    buzzer.stopPlaying();  
//    c=2;
//  delay(500); 
//}
//}

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
  if(pulse_width > MAX_DIST) {
  }
  else if(pulse_width < MAX_DIST) {
    motors.setSpeeds(0,0);
    buzzer.play(">g32>>c32");
    delay(2000);
    buzzer.stopPlaying(); 
    c=2;
  }
}

void hcsr04UltrasonicTwo(){
  unsigned long t3;
  unsigned long t4;
  unsigned long pulse_width_2;
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  while ( digitalRead(ECHO_PIN) == 0 );
  t3 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t4 = micros();
  pulse_width_2 = t4 - t3;
  if(pulse_width_2 > MAX_DIST) {
    borderDetect();
  }
  else if(pulse_width_2 < MAX_DIST) {
    motors.setSpeeds(0,0);
    delay(1000);
    buzzer.playFromProgramSpace(march);
    c=1;
  }
}

void lineFollower(){
  unsigned int sensors[6];

  
  // Get the position of the line.  Note that we *must* provide the "sensors"
  // argument to readLine() here, even though we are not interested in the
  // individual sensor readings
  int position = reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, 1);

  // Our "error" is how far we are away from the center of the line, which
  // corresponds to position 2500.
  int error = position - 2500;

  // Get motor speed difference using proportional and derivative PID terms
  // (the integral term is generally not very useful for line following).
  // Here we are using a proportional constant of 1/4 and a derivative
  // constant of 6, which should work decently for many Zumo motor choices.
  // You probably want to use trial and error to tune these constants for
  // your particular Zumo and line course.
  int speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;

  


  // Get individual motor speeds.  The sign of speedDifference
  // determines if the robot turns left or right.
  int m1Speed = MAX_SPEED + speedDifference;
  int m2Speed = MAX_SPEED - speedDifference;

  // Here we constrain our motor speeds to be between 0 and MAX_SPEED.
  // Generally speaking, one motor will always be turning at MAX_SPEED
  // and the other will be at MAX_SPEED-|speedDifference| if that is positive,
  // else it will be stationary.  For some applications, you might want to
  // allow the motor speed to go negative so that it can spin in reverse.
  //  distanceFunc();
  if (m1Speed < 0)
    m1Speed = 0;
  if (m2Speed < 0)
    m2Speed = 0;
  if (m1Speed > MAX_SPEED)
    m1Speed = MAX_SPEED;
  if (m2Speed > MAX_SPEED)
    m2Speed = MAX_SPEED; 
   motors.setSpeeds(m1Speed, m2Speed);      
}


void reverseRobot(){
    motors.setSpeeds(0,0);
    buzzer.play(">g32>>c32");
    delay(1000);
    buzzer.stopPlaying();     
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(100);
    motors.setSpeeds(0,0);
    buzzer.play(">g32>>c32");
    delay(1000);
    buzzer.stopPlaying();
    c=3;
}

void waitforButtonCountDown(){
  // put your setup code here, to run once:  
  
  // Initialize the reflectance sensors module
  reflectanceSensors.init();
  
  // Wait for the user button to be pressed and released
  button.waitForButton();

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);      
    reflectanceSensors.calibrate();


    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20);
  }
  motors.setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
  buzzer.play(">g32>>c32");

  // Wait for the user button to be pressed and released
  button.waitForButton();

  // Play music and wait for it to finish before we start driving.
  buzzer.play("L16 cdegreg4");
  while(buzzer.isPlaying());

}

void setup() {

}

void loop() {
  // put your main code here, to run repeatedly:
  
  while (c == 0){
    waitforButtonCountDown();
    c=1;
  }
  while (c == 1){
    int a=0;
    while (a<=50) { //do this loop up to x ms
                      //at home one lap lasted 10s
      lineFollower();
      
      a++;
  }
    hcsr04Ultrasonic();

 }
  while (c == 2){
    reverseRobot();
}
  while (c == 3){
    int b=0;
    while (b<=500) { //do this loop up to x ms
      sensors.read(sensor_values); //at home one lap lasted 10s
      if (sensor_values[0] < QTR_THRESHOLD )
      {
      // if leftmost sensor detects line, reverse and turn to the right
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(TURN_DURATION);
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
      }
      else if (sensor_values[5] < QTR_THRESHOLD )
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
      
      b++;    
    hcsr04UltrasonicTwo();
}
}
}
  
