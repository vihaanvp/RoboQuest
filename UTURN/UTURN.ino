/*
 * =============================================
 *  Line Follower Code
 *  (Assuming IR on Black Line outputs HIGH)
 * ============================================
*/

// Defining new Data Type for Robot State
enum States {
  FOLLOW_LINE,
  U_TURN,
  FOLLOW_LINE_BACK,
  STOP
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

  switch (RobotState) {
    case FOLLOW_LINE:
      FollowLine();
      if (L == HIGH && R == HIGH) {
        RobotState = U_TURN;
      }
    break;

    case U_TURN:
      digitalWrite(LM1, HIGH);
      digitalWrite(LM2, LOW);
      digitalWrite(RM1, LOW);
      digitalWrite(RM2, HIGH);
      delay(900);
      RobotState = FOLLOW_LINE_BACK;
    break;

    case FOLLOW_LINE_BACK:
      FollowLine();
      if (L == HIGH && R == HIGH) {
        digitalWrite(LM1, LOW);
        digitalWrite(LM2, LOW);
        digitalWrite(RM1, LOW);
        digitalWrite(RM2, LOW);
      }
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

  // L=HIGH R=HIGH then stop (Line detected on both IRs)
}