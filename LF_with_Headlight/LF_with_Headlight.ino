int LM1 = 2;
int LM2 = 3;
int RM1 = 4;
int RM2 = 5;

int IR_L = 7;
int IR_R = 8;

int LED = 6;

int L, R;

void setup() {
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);

  pinMode(IR_L, INPUT);
  pinMode(IR_R, INPUT);

  pinMode(LED, OUTPUT);

}

void loop() {
  L = digitalRead(IR_L);
  R = digitalRead(IR_R);

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

  if (analogRead(A0) < 450) {
    digitalWrite(LED, HIGH);
  }

  else {
    digitalWrite(LED, LOW);
  }
}