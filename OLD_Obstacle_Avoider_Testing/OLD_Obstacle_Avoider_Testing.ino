int LM1 = 2;
int LM2 = 3;
int RM1 = 4;
int RM2 = 5;

// Defining IR Pins
int IR_L = 7;
int IR_R = 8;

// Define Peripheral Pins
int LED = 6;
int TRIG = 10;
int ECHO = 11;

int RightTurnDelay = 400;
int ForwardDelay = 600;
int LeftTurnDelay = 400;
int ForwardDelayLong = 1000;

void setup() {
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);

  pinMode(IR_L, INPUT);
  pinMode(IR_R, INPUT);

  pinMode(LED, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
  delay(1000);

}

void loop() {
  
}

void AvoidObject() {
  int distance = getDistance();
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
  delay(RightTurnDelay);



  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(ForwardDelay);



  digitalWrite(LM1, LOW);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(LeftTurnDelay);



  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(ForwardDelayLong);



  digitalWrite(LM1, LOW);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(LeftTurnDelay);


  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(ForwardDelay);



  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
  delay(RightTurnDelay);

 
}

int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2;
}