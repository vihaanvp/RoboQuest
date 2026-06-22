/*
 * =============================================
 *  Line Follower Code
 *  (Assuming IR on Black Line outputs HIGH)
 * ============================================
*/

// Defining new Data Type for Robot State
enum States {
  FOLLOW_LINE = 1,
  AVOID_OBJECT = 2,
  FOLLOW_LINE_AGAIN = 3,
  AVOID_SND_OBJECT = 4,
  FINAL_FOLLOW_LINE = 5
};
States RobotState = FOLLOW_LINE;

int L, R;

// Defining Motor Pins
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

int DistanceThreshold = 12;
int LDRThreshold = 450;

int RightTurnDelay = 400;
int ForwardDelay = 800;
int LeftTurnDelay = 400;
int ForwardDelayLong = 1100;

void setup() {
  // Declare the Motor Pins
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);

  // Declare the IR pins
  pinMode(IR_L, INPUT);
  pinMode(IR_R, INPUT);

  // Declare peripheral pins
  pinMode(LED, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  //Serial.begin(9600);
}

void loop() {
  L = digitalRead(IR_L);
  R = digitalRead(IR_R);

  int distance = getDistance();

  int LDR = analogRead(A0);

  switch (RobotState) {
    case FOLLOW_LINE:
      L = digitalRead(IR_L);
      R = digitalRead(IR_R);
      FollowLine();
      if (distance < DistanceThreshold && distance > 5) {
        RobotState = AVOID_OBJECT;
      }
      break;

    case AVOID_OBJECT:
      AvoidObject();
      delay(200);
      RobotState = FOLLOW_LINE_AGAIN;
      break;

    case FOLLOW_LINE_AGAIN:
      L = digitalRead(IR_L);
      R = digitalRead(IR_R);
      FollowLine();
      if (distance < DistanceThreshold && distance > 5) {
        RobotState = AVOID_SND_OBJECT;
      }
      break;

    case AVOID_SND_OBJECT:
      AvoidObject();
      delay(200);
      RobotState = FINAL_FOLLOW_LINE;
      break;

    case FINAL_FOLLOW_LINE:
      L = digitalRead(IR_L);
      R = digitalRead(IR_R);
      FollowLine();
      break;
  }

  // If LDR less than 350, turn on LED
  if (LDR < LDRThreshold) {
    digitalWrite(LED, HIGH);
  }

  // If LDR more than 350, turn off LED
  else {
    digitalWrite(LED, LOW);
  }

  Serial.print("Distance: ");
  Serial.println(distance);

  Serial.print("Current State: ");
  if (RobotState == 1) {
    Serial.println("FOLLOW_LINE");
  }
  else if (RobotState == 2) {
    Serial.println("AVOID_OBJECT");
  }
  else if (RobotState == 3) {
    Serial.println("FOLLOW_LINE_AGAIN");
  }
  else if (RobotState == 4) {
    Serial.println("AVOID_SND_OBJECT");
  }
  else {
    Serial.println("FINAL_LINE_FOLLOW");
  }
}

int getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  unsigned long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2;
}

void FollowLine() {
  int distance = getDistance();
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

  // L=HIGH R=HIGH then stop (Line detected on both IRs)
  else {
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
  }
}

void AvoidObject() {
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
  delay(400);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  delay(1000);

  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(800);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  delay(1000);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(400);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  delay(1000);

  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(1200);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  delay(1000);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(400);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  delay(1000);

  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, LOW);
  delay(750);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  delay(1000);

  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, HIGH);
  delay(400);

  digitalWrite(LM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(RM2, LOW);
  delay(1000);
}