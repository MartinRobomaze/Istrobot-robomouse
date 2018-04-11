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
//include wire library for I2C communication with compass
#include <Wire.h>
//include encoder library
#include <Encoder.h>
#include <math.h>

float PR, DR, previousIrError;
float PE, DE, previousEncError;
const float kPR = 20, kDR = 10;
const float kPE = 1, kDE = .5;

int encError = 0;

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

float gyroZ;

/**
 * function for initialize components
 */
void begin() {
   Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
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

  float encPid = 0;
  float speedA = getSpeed();
  float speedB = getSpeed();

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

  Serial.print("A: ");
  Serial.println(speedA);
  Serial.print("B: ");
  Serial.println(speedB);
  
  float targetLeftDistance = 1.2;
  float irError = targetLeftDistance - readIRSensor(1);

  PR = irError;
  DR = irError - previousIrError;

  previousIrError = irError;

  float irPid = (PR * kPR) - (DR * kDR);
  
  moveTank(speedA - irPid, speedB + irPid);
}

/**
 * go forward for given lenght.
 */
void forward(byte speed, float cm) {

  float rotations = getRotationsFromCm(cm);
  float startPos = readEncoderPos('A');
    
  do {
    moveTank(speed, speed);
  }
  while (readEncoderPos('A') < startPos + rotations);

  stop();
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


//function to turn left/right after it will come to declared angle




/**
 * function for move robot like tank
 */
void moveTank(byte speedA, byte speedB) {

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
 * 
 */

void turn(byte speed, float angle) {
  while (!hasTurned) {
    float yaw = readAccel();
  
    if (targetAngle == 0) {
      targetAngle = yaw + angle; 
    }
    
    float diff = targetAngle - yaw;
    Serial.println("diff");
    Serial.println(diff);
  
    if (targetAngle < yaw) {
      moveTank(speed, 0);
    } else {
      moveTank(0, speed);
    }
  
    if (abs(diff) < 8) {
      hasTurned = true;
      Serial.println("====================TURN DONE===================");
      stop();
    }
  }
}

/**
 * function for stop motors
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
 * reading gyro
 */
float readAccel() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  int ax = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  int ay = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  int az = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  int accelX = ax * 3.9;
  int accelZ = az * 3.9;
  return ax / 182.04;
}

/**
 * function for reading IR sensors
 */
float readIRSensor(int sensor) {
  return analogRead(sensors[sensor - 1]) * 5.0 / 1024.0;
}

/**
 * function for reading rotary encoders
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


