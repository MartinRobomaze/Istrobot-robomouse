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

bool replay = false;

void setup() {
  begin();
  Serial.begin(9600);
}


void loop() {
  Serial.println(readAngle());
}

