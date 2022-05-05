#include <Wire.h>
#include <ZumoShield.h>

ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
int lastError = 0;

#include <HCSR04.h>

// Initialize sensor that uses digital pins 13 and 12.
const byte triggerPin = A4;
const byte echoPin = A5;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)
const int MAX_SPEED = 200;

// Pins
const int TRIG_PIN = A4;
const int ECHO_PIN = A5;

// Anything over 400 cm (23200 us pulse) is "out of range"
const unsigned int MAX_DIST = 580;

void setup()
{
  // Play a little welcome song
  buzzer.play(">g32>>c32");

  // Initialize the reflectance sensors module
  reflectanceSensors.init();

  // Wait for the user button to be pressed and released
  button.waitForButton();

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // The Trigger pin will tell the sensor to range find
//  pinMode(TRIG_PIN, OUTPUT);
//  digitalWrite(TRIG_PIN, LOW);  
//  pinMode(ECHO_PIN, INPUT);
  
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

void loop()
{
//  unsigned long t1;
//  unsigned long t2;
//  unsigned long pulse_width;
//  digitalWrite(TRIG_PIN, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(TRIG_PIN, LOW);
//  while ( digitalRead(ECHO_PIN) == 0 );
//  t1 = micros();
//  while ( digitalRead(ECHO_PIN) == 1);
//  t2 = micros();
//  
//  pulse_width = t2 - t1;
//  
  unsigned int sensors[6];
  int position = reflectanceSensors.readLine(sensors, QTR_EMITTERS_ON, 1);
  int error = position - 2500;
  int speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;
  int m1Speed = MAX_SPEED + speedDifference;
  int m2Speed = MAX_SPEED - speedDifference;
  int i;
  float distance = distanceSensor.measureDistanceCm();
  if (distance >= MAX_DIST)   {
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
  else if (distance <= MAX_DIST)
    motors.setSpeeds(0, 0); 
    delay(50);
//    for(i = 0; i < 40; i++)     {
//      if (i> 0 && i <= 30)
//        motors.setSpeeds(-200,200);
//      else
//        motors.setSpeeds(200,200);
//       }
  
}
