/****
 * This is program for motor control and sensor reading.
 * 
 * v0.1a - finished developing ArduinoInterface class
 * v0.1b - tested motor driving - sucessful
 * v0.1b2 - tested encoders - sucessful
 * v0.1b3 - tested compass reading - sucessful
 * v0.1b4 - replace left and right functions with pointTo(speed, angle) and turn(speed, angle)
 *          issue: compass reading is not good
 * v0.2a1 - replacing compass with gyro + accelerometer MPU6050 - using Jeff Rowberg's library - link: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050
 * v0.2b1 - tested gyro - absolutely sucessful
 * creator: MartinLinux
 */

//include arduino library
#include <Arduino.h>
//include encoder library
#include <Encoder.h>


float PR, DR, previousIrError;
float PE, DE, previousEncError;
float PE2, DE2, previousEncError2;

const float kPR = 20, kDR = 10;
const float kPE = 1, kDE = .5;

int encError = 0;
int encError2;

//defining pins for motor control
const int motorBF = 5;
const int motorBB = 6;
const int motorAF = 10;
const int motorAB = 9;

//defining pins for sensor reading
const int sensors[4] = {A0, A1, A2, A3};

//defining pins for encoder reading
Encoder rotationsB(2, 8);
Encoder rotationsA(3, 12);

/**
 * function for initialize components
 */
void begin() {
  pinMode(motorAF, OUTPUT);
  pinMode(motorAB, OUTPUT);
  pinMode(motorBF, OUTPUT);
  pinMode(motorBB, OUTPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(sensors[i], INPUT);
  }
}

/**
 * function for going forward until you stop motors manually
 */
void forward() {

  float speedA = getSpeed();
  float speedB = getSpeed();

  if (readIRSensor(1) > 0.8 && readIRSensor(3) > 0.8) {
    float irError = readIRSensor(1) - readIRSensor(3);
  
    PR = irError;
    DR = irError - previousIrError;
  
    previousIrError = irError;
  
    float irPid = (PR * kPR) - (DR * kDR);

    speedA = speedA + irPid;
    speedB = speedB - irPid;

    Serial.print("PID: ");
    Serial.println(irPid);
    Serial.print("A: ");
    Serial.println(speedA);
    Serial.print("B: ");
    Serial.println(speedB);
  }
  else {
    float encPid = 0;
  
    // The B encoder will return negative value so we have to 
    // add the value.
    encError = readEncoderPos('A') + readEncoderPos('B');
    // To prevent incrementing we define encError as global and to get
    // the current error value we discount the previous error value.
    encError -= previousEncError;
    
    PE = encError;
    DE = encError - previousEncError;
  
    encPid = (PE * kPE) - (DE * kDE);
    speedB += encPid;
    previousEncError = encError;
  }

  moveTank(speedA, speedB);
}

/**
 * function for going backward until you stop them manually
 */
void back(byte speed) {
  analogWrite(motorAF, 0);
  analogWrite(motorAB, speed);
  analogWrite(motorBF, 0);
  analogWrite(motorBB, speed);
}

/**
 * function for move robot like tank
 */
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
    analogWrite(motorBF, speedB);
    analogWrite(motorBB, 0);
  }
  else if (speedB < 0) {
    analogWrite(motorBF, 0);
    analogWrite(motorBB, speedB);
  }
  else {
    analogWrite(motorBF, 0);
    analogWrite(motorBB, 0);
  }
}

/**
 * turn left/right
 */
void turn(float angle) {
  int originalPosA = readEncoderPos('A');
  int originalPosB = readEncoderPos('B');
  
  if (angle == 90) {
    int compensation = getSpeed() < 200 ? 80 : 230;
    while (readEncoderPos('A') - originalPosA < 441 - compensation) {
      moveTank(getSpeed() + 20, 0);
      Serial.println(readEncoderPos('A'));
    }
    
    stop();
  } else if (angle == -90) {
    int compensation = getSpeed() < 200 ? 140 : 200;
    while (readEncoderPos('B') + 20 - originalPosB > -441 + compensation) {
      moveTank(0, getSpeed);
    }
    
    stop();
  }
  rotationsA.write(0);
  rotationsB.write(0);
}

/**
 * stop motors
 */
void stop() {
  digitalWrite(motorAF, 1);
  digitalWrite(motorAB, 1);
  digitalWrite(motorBF, 1);
  digitalWrite(motorBB, 1);
  delay(3);
  digitalWrite(motorAF, 0);
  digitalWrite(motorAB, 0);
  digitalWrite(motorBF, 0);
  digitalWrite(motorBB, 0);
}

/**
 * reading IR sensors
 */
float readIRSensor(int sensor) {
  return analogRead(sensors[sensor - 1]) * 5.0 / 1024.0;
}

/**
 * reading rotary encoders
 */
float readEncoderPos(char sensor) {
  if (sensor == 'A') {
    return rotationsA.read();
  }
  else if (sensor == 'B') {
    return rotationsB.read();
  }
  else {
    return -999999999999;
  }
}

/**
 * transforms centimeters to rotations
 */
float getRotationsFromCm(float cm) {
  return (cm - 6) / (PI * 3);
}


