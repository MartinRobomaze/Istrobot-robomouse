/*
 * This is maze solver program based on left hand rule with optimizing path
 * 
 * versions:
 * v0.1a - tested driving of the motors - sucessfull
 * v0.1a2 - tested encoders - sucessful
 */
//
//int index = 0;
//int indexS = 0;

#include <MechaQMC5883.h>


MechaQMC5883 c1;

bool replay = false;

void setup() {
  begin();
  Serial.begin(9600);
}


void loop() {
  int x, y, z;
  c1.read(&x,&y,&z);
  Serial.println(x);
  Serial.println(y);
  delay(500);
}

