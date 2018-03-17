/*
 * This is program for motor control and sensor reading.
 * 
 * v0.1a - started developing ArduinoInterface class
 * v0.2a - finished developing ArduinoInterface class
 * v0.2b - tested motor driving - sucessfull
 * 
 * creator: MartinLinux
 */

//include arduino library
#include <Arduino.h>
//include wire library for I2C communication with compass
#include <Wire.h>
//include encoder library
#include <Encoder.h>
//include compass library
#include <HMC5883L_Simple.h>


//defining pins for motor control
const int motorAF = 5;
const int motorAB = 6;
const int motorBF = 9;
const int motorBB = 10;

//defining pins for sensor reading
const int sensors[5] = {A0, A1, A2, A3, A4};

//defining pins for encoder reading
//Encoder rotationsA(2, 4);
//Encoder rotationsB(3, 7);

//define object for compass reading
HMC5883L_Simple compass;


//function for initialize components
void begin() {
  Wire.begin();
  pinMode(motorAF, OUTPUT);
  pinMode(motorAB, OUTPUT);
  pinMode(motorBF, OUTPUT);
  pinMode(motorBB, OUTPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(sensors[i], INPUT);
  }
}

//function for going forward for rotations
void forward(float rotations, int speed) {
  while ((readEncoderPos('A') / 360 < rotations) && (readEncoderPos('B') / 360) < rotations) {
    analogWrite(motorAF, speed);
    analogWrite(motorAB, 0);
    analogWrite(motorBF, speed);
    analogWrite(motorBB, 0);
  }
  stop();
}

//function for going forward until you stop them manually
void forward(int speed) {
  analogWrite(motorAF, speed);
  analogWrite(motorAB, 0);
  analogWrite(motorBF, speed);
  analogWrite(motorBB, 0);
}

//function for going backward for rotations
void back(float rotations, int speed) {
  while ((readEncoderPos("A") / 360 < rotations) && (readEncoderPos("B") / 360) < rotations) {
    analogWrite(motorAF, 0);
    analogWrite(motorAB, speed);
    analogWrite(motorBF, 0);
    analogWrite(motorBB, speed);
  }
  stop();
}

//function for going backward until you stop them manually
void back(int speed) {
  analogWrite(motorAF, 0);
  analogWrite(motorAB, speed);
  analogWrite(motorBF, 0);
  analogWrite(motorBB, speed);
}

//function for going left for angle
void left(int angle, int speed) {
  while (readCompass() <= angle) {
    analogWrite(motorAF, 0);
    analogWrite(motorAB, 0);
    analogWrite(motorBF, speed);
    analogWrite(motorBB, 0);
  }
  stop();
}

//function for going right for angle
void right(int angle, int speed) {
  while (readCompass <= angle) {
    analogWrite(motorAF, speed);
    analogWrite(motorAB, 0);
    analogWrite(motorBF, 0);
    analogWrite(motorBB, 0);
  }
  stop();
}

//function for move robot like tank
void moveTank(int speedA, int speedB) {
  if (speedA > 0) {
    analogWrite(motorAF, speedA);
    analogWrite(motorAB, 0);
  }
  else if (speedA < 0) {
    analogWrite(motorAF, 0);
    analogWrite(motorAB, speedA);
  }
  else {
    analogWrite(motorAF, 0);
    analogWrite(motorAB, 0);
  }
 
  if (speedB > 0) {
    analogWrite(motorBF, speedA);
    analogWrite(motorBB, 0);
  }
  else if (speedB < 0) {
    analogWrite(motorBF, 0);
    analogWrite(motorBB, speedA);
  }
  else {
    analogWrite(motorBF, 0);
    analogWrite(motorBB, 0);
  }
}

//function for stop motors
void stop() {
  analogWrite(motorAF, 0);
  analogWrite(motorAB, 0);
  analogWrite(motorBF, 0);
  analogWrite(motorBB, 0);
}

//function for reading IR sensors
int readIRSensor(int sensor) {
  return analogRead(sensors[sensor - 1]);
}

//function for reading compass
int readCompass() {
  compass.SetDeclination(5, 22, 'E');
  compass.SetSamplingMode(COMPASS_SINGLE);
  compass.SetScale(COMPASS_SCALE_130);

  return compass.GetHeadingDegrees();
}

//function for reading rotary encoders
long readEncoderPos(char sensor) {
  if (sensor == 'a' || sensor == 'A') {
    return rotationsA.read() / 30;
  }
  else if (sensor == 'b' || sensor == 'B') {
    return rotationsB.read() / 30;
  }
  else {
    return -999999999999;
  }
}

