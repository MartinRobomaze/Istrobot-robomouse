/*
 * This is program for motor control and sensor reading.
 * 
 * v0.1a - started developing ArduinoInterface class
 * v0.2a - finished developing ArduinoInterface class
 * v0.2b - tested motor driving - sucessful
 * v0.2b2 - tested encoders - sucessful
 * v0.2b3 - tested compass reading - sucessful
 * v0.2b4 - replace left and right functions with pointTo(speed, angle) and turn(speed, angle)
 * creator: MartinLinux
 */

//include arduino library
#include <Arduino.h>
//include wire library for I2C communication with compass
#include <Wire.h>
//include encoder library
#include <Encoder.h>
//include compass library
#include <MechaQMC5883.h>


//defining pins for motor control
const int motorAF = 5;
const int motorAB = 6;
const int motorBF = 9;
const int motorBB = 10;

//defining pins for sensor reading
const int sensors[5] = {A0, A1, A2, A3, A4};

//defining pins for encoder reading
Encoder rotationsA(2, 4);
Encoder rotationsB(7, 3);

//define object for compass reading
MechaQMC5883 compass;


//function for initialize components
void begin() {
  Wire.begin();
  compass.init();
  pinMode(motorAF, OUTPUT);
  pinMode(motorAB, OUTPUT);
  pinMode(motorBF, OUTPUT);
  pinMode(motorBB, OUTPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(sensors[i], INPUT);
  }
}


//function for going forward for rotations
void forward(int speed, float rotations) {
  float posA = readEncoderPos('A');
  float posB = readEncoderPos('B');
  while (readEncoderPos('A') < posA + rotations && readEncoderPos('B') < rotations) {
    analogWrite(motorAF, speed);
    analogWrite(motorAB, 0);
    analogWrite(motorBF, speed);
    analogWrite(motorBB, 0);
  }
  stop();
}


//function for going forward until you stop motors manually
void forward(int speed) {
  analogWrite(motorAF, speed);
  analogWrite(motorAB, 0);
  analogWrite(motorBF, speed);
  analogWrite(motorBB, 0);
}


//function for going backward for rotations
void back(int speed, float rotations) {
  while (readEncoderPos('A') + rotations < rotations && readEncoderPos('B') + rotations < rotations) {
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


//function to turn left/right after it will come to declared angle
void pointTo(int speed, float angle) {
  int target = angle;
  target = target % 360;
  
  if(target < 0) {
    target += 360;
  }
  
  int direction = angle;
  
  while(1) {
    int currentAngle = readCompass();
    int diff = target-currentAngle;
    direction = 180 - (diff + 360) % 360;
    
    if(direction > 0){
      moveTank(speed, 0);
      delay(10);
    }else{
      moveTank(0, speed);
      delay(10);
    }
    
    if(abs(diff) < 8){
      stop();
      return;
    }
  }
}


void turn(int speed, int angle) {
  int originalAngle = readCompass();
  int target = originalAngle + angle;

  pointTo(speed, target);
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


void moveTank(int speedA, int speedB, float rotations) {
  while (readEncoderPos('a') + rotations < rotations && readEncoderPos('b') + rotations < rotations) {
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
}


//function for stop motors
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


//function for reading IR sensors
int readIRSensor(int sensor) {
  return analogRead(sensors[sensor - 1]);
}


//function for reading compass
int readCompass() {
  int x, y, z;
  compass.read(&x, &y, &z);
  int heading = atan2(x, y);
  heading += 0.0872665;

  if(heading < 0) {
    heading += 2*PI;
  }  
  if(heading > 2*PI) {
    heading -= 2*PI;
  } 
  float headingDegrees = heading * 180/M_PI; 

  return headingDegrees;
}


//function for reading rotary encoders
float readEncoderPos(char sensor) {
  if (sensor == 'a' || sensor == 'A') {
    return rotationsA.read() / 30 / 7;
  }
  else if (sensor == 'b' || sensor == 'B') {
    return 0 - (rotationsB.read() / 30 / 7);
  }
  else {
    return -999999999999;
  }
}

