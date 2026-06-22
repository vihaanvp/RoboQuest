enum RobotState {
  FIND_SPOT,
  TURN,
  PARK
};

int L, R;

RobotState state = FIND_SPOT;

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
int LeftTurnDelay = 400 ;
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
  L = digitalRead(IR_L);
  R = digitalRead(IR_R);
  int dist = getDistance();

  switch (state) {
    case FIND_SPOT:
      if (dist < 30) {
        digitalWrite(LM1, HIGH);
        digitalWrite(LM2, LOW);
        digitalWrite(RM1, HIGH);
        digitalWrite(RM2, LOW);
      } else {
        state = TURN;
      }
    break;

    case TURN:
      digitalWrite(LM1, LOW);
      digitalWrite(LM2, LOW);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, LOW);
      delay(900);

      digitalWrite(LM1, HIGH);
      digitalWrite(LM2, LOW);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, LOW);
      state = PARK;
    break;

    case PARK:
      FollowLine();
    break;
  
  }

  
}

void FollowLine() {
  // L=LOW R=LOW then move forward (Floor is detected)
  if (L == LOW && R == LOW) {
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
  }

  // L=LOW R=HIGH then move RIGHT (Line detected on right IR but not on left IR)
  else if (L == LOW && R == HIGH) {
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
  }

  // L=HIGH R=LOW then move LEFT (Line detected on left IR but not on right IR)
  else if (L == HIGH && R == LOW) {
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
  }
  
  L = digitalRead(IR_L);
  R = digitalRead(IR_R);

  // L=HIGH R=HIGH then stop (Line detected on both IRs)
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