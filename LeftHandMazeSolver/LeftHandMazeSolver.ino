#include <avr/sleep.h>
#include <MPU6050.h>
/*
 * This is maze solver program based on left hand rule with optimizing path
 * 
 * versions:
 * v0.1
 */

//char path[200];
//byte pathIndex = 0;
//byte readIndex = 0;

const byte baseSpeed = 80;

bool hasTurned = false;

int targetAngle = 0;


byte i = 0;


void setup() {
  begin();
  Serial.begin(9600);
  pinMode(A7, INPUT);
  while (analogRead(A7) != 1023) {}
  delay(1000);
}

void loop() {  
  if (readIRSensor(2) < 0.8) {
    forward();
  } else {
    stop();
  }
}

//bool turn(byte speed, int angle) {
//
//  int target = 0;
//
//  while (1) {
//
//    timer = millis();
//
//    // Read normalized values
//    Vector norm = mpu.readNormalizeGyro();
//  
//    // Calculate Pitch, Roll and Yaw
//    yaw = yaw + norm.ZAxis * timeStep;
//
//    // Wait to full timeStep period
//    delay((timeStep*1000) - (millis() - timer));
//
//    Serial.println("Current angle");
//    Serial.println(yaw);
//
//    if (target == 0) {
//      target = yaw + angle; 
//    }
//
//    float diff = target - yaw;
//    Serial.println("diff");
//    Serial.println(diff);
//
//    if (target < yaw) {
//      moveTank(speed, 0);
//    } else {
//      moveTank(0, speed);
//    }
//
//    if (abs(diff) < 8) {
//      Serial.println("====================TURN DONE===================");
//      stop();
//      return true;
//    }
//  }
//}
//
//
//void leftHandMazeSolve() {
//  if (readIRSensor(1) < 0.8) {
//    stop();
//    //forward(150, 0.5);
//    turn(150, -90);
//    forward(150, 1);
//    path[pathIndex] = 'L';
//  } else if (readIRSensor(2) < 0.8) {
//    straight(150);
//    if (readIRSensor(3) < 0.8) {
//      path[pathIndex]= 'S';
//    }
//  } else if (readIRSensor(3) < 0.8) {
//    stop();
//    //forward(150, 0.5);
//    turn(150, 90);
//    forward(150, 1);
//    path[pathIndex] = 'R';
//  } else {
//    turnTank(150, 180);
//    forward(150, 1);
//    path[pathIndex] = 'B';
//  }
//  pathIndex++;
//  shortPath();
//}
//
//
//void shortPath() {
//  if (path[pathIndex - 1] == 'B') {
//    if (path[pathIndex - 2] == 'L' && path[pathIndex] == 'L') {
//      pathIndex -= 3;
//      path[pathIndex] = 'S';
//    } else if (path[pathIndex - 2] == 'L' && path[pathIndex] == 'R') {
//      pathIndex -= 3;
//      path[pathIndex] = 'B';
//    } else if (path[pathIndex - 2] == 'L' && path[pathIndex] == 'S') {
//      pathIndex -= 3;
//      path[pathIndex] = 'R';
//    } else if (path[pathIndex - 2] == 'R' && path[pathIndex] == 'L') {
//      pathIndex -= 3;
//      path[pathIndex] = 'B';
//    } else if (path[pathIndex - 2] == 'S' && path[pathIndex] == 'L') {
//      pathIndex -= 3;
//      path[pathIndex] = 'R';
//    } else if (path[pathIndex - 2] == 'S' && path[pathIndex] == 'S') {
//      pathIndex -= 3;
//      path[pathIndex] = 'B';
//    }
//
//    pathIndex++;
//  } else {
//    return;
//  }
//}
//
//
//void repeatMotion() {
//  if ((readIRSensor(1) > .65 && readIRSensor(1) < .9) && (readIRSensor(3) > .65 && readIRSensor(3) < .9)) {
//    straight(255);
//  } else {
//    if (path[readIndex] == 'L') {
//      stop();
//      //forward(150, 0.5);
//      turn(200, -90);
//      forward(255, 1);
//    } else if (path[readIndex] == 'S') {
//      straight(255);
//    } else if (path[readIndex] == 'R') {
//      stop();
//      //forward(150, 0.5);
//      turn(150, 90);
//      forward(150, 1);
//    } else if (path[readIndex] == 'D') {
//      stop();
//      delay(1000);
//      moveTank(-255, 255); // pinky sa toci a toci a toci...
//    }
//  }
//}
//
//
//void straight(byte speed) {
//  byte speedA = speed - readEncoderPos('b');
//  byte speedB = speed + readEncoderPos('a');
//
//  moveTank(speedA, speedB);
//}
//
//void power_off() {
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//  sleep_enable();
//}

int getSpeed() {
  return baseSpeed;
}

