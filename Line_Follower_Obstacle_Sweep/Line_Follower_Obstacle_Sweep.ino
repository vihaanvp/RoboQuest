// Define IR sensor pins
#define LEFT_IR_PIN 7
#define RIGHT_IR_PIN 8

// Define motor pins
#define MOTOR1_PIN1 3
#define MOTOR1_PIN2 2
#define MOTOR2_PIN1 5
#define MOTOR2_PIN2 4

// Ultrasonic pins
#define TRIG_PIN 10
#define ECHO_PIN 11

#define OBSTACLE_DISTANCE 5

// ---- TIMINGS (ms) ----
// Turns
int TURN_1_TIME = 400;  // first right
int TURN_2_TIME = 400;  // first left
int TURN_3_TIME = 400;  // second left
int TURN_4_TIME = 350;  // final right

// Forward movements
int FORWARD_1_TIME = 600;
int FORWARD_2_TIME = 1000;
int FORWARD_3_TIME = 900;

// Pauses
int STEP_PAUSE = 120;

// Line finding
int LINE_CONFIRM_TIME = 120;
int SWEEP_INTERVAL = 70;

// ----- STATE MACHINE -----
enum State { FOLLOW_LINE, AVOIDING };
State currentState = FOLLOW_LINE;

int avoidStep = 0;
unsigned long stepStartTime = 0;
unsigned long lineDetectStart = 0;

void setup() {
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);

  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {

  if (currentState == FOLLOW_LINE) {

    if (getDistance() < OBSTACLE_DISTANCE) {
      currentState = AVOIDING;
      avoidStep = 0;
      stepStartTime = millis();
      return;
    }

    int left = digitalRead(LEFT_IR_PIN);
    int right = digitalRead(RIGHT_IR_PIN);

    if (left == LOW && right == LOW) moveForward();
    else if (left == LOW && right == HIGH) turnRight();
    else if (left == HIGH && right == LOW) turnLeft();
    else stopMoving();
  }

  else {
    avoidSequence();
  }
}

// -------- ULTRASONIC --------
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long d = pulseIn(ECHO_PIN, HIGH, 20000);
  return d * 0.034 / 2;
}

// -------- AVOIDANCE --------
void avoidSequence() {
  unsigned long now = millis();

  switch (avoidStep) {

    case 0:
      turnRight();
      if (now - stepStartTime > TURN_1_TIME) nextStep(now);
      break;

    case 1:
      pauseStep(now);
      break;

    case 2:
      moveForward();
      if (now - stepStartTime > FORWARD_1_TIME) nextStep(now);
      break;

    case 3:
      pauseStep(now);
      break;

    case 4:
      turnLeft();
      if (now - stepStartTime > TURN_2_TIME) nextStep(now);
      break;

    case 5:
      pauseStep(now);
      break;

    case 6:
      moveForward();
      if (now - stepStartTime > FORWARD_2_TIME) nextStep(now);
      break;

    case 7:
      pauseStep(now);
      break;

    case 8:
      turnLeft();
      if (now - stepStartTime > TURN_3_TIME) nextStep(now);
      break;

    case 9:
      pauseStep(now);
      break;

    case 10:
      moveForward();
      if (now - stepStartTime > FORWARD_3_TIME) nextStep(now);
      break;

    case 11:
      pauseStep(now);
      break;

    case 12: {
      // ---- LINE FINDING ----
      static bool sweepDir = false;
      static unsigned long lastSwitch = 0;

      int left = digitalRead(LEFT_IR_PIN);
      int right = digitalRead(RIGHT_IR_PIN);

      if (millis() - lastSwitch > SWEEP_INTERVAL) {
        sweepDir = !sweepDir;
        lastSwitch = millis();
      }

      // forward + slight correction
      if (sweepDir) {
        // slight right
        digitalWrite(MOTOR1_PIN1, LOW);
        digitalWrite(MOTOR1_PIN2, HIGH);
        digitalWrite(MOTOR2_PIN1, HIGH);
        digitalWrite(MOTOR2_PIN2, HIGH);
      } else {
        // slight left
        digitalWrite(MOTOR1_PIN1, HIGH);
        digitalWrite(MOTOR1_PIN2, HIGH);
        digitalWrite(MOTOR2_PIN1, LOW);
        digitalWrite(MOTOR2_PIN2, HIGH);
      }

      // strict lock
      if (left == LOW && right == LOW) {
        if (lineDetectStart == 0) lineDetectStart = millis();

        if (millis() - lineDetectStart > LINE_CONFIRM_TIME) {
          nextStep(millis());
          lineDetectStart = 0;
        }
      } else {
        lineDetectStart = 0;
      }

      break;
    }

    case 13:
      turnRight();
      if (now - stepStartTime > TURN_4_TIME) {
        currentState = FOLLOW_LINE;
      }
      break;
  }
}

void nextStep(unsigned long now) {
  avoidStep++;
  stepStartTime = now;
}

void pauseStep(unsigned long now) {
  stopMoving();
  if (now - stepStartTime > STEP_PAUSE) {
    nextStep(now);
  }
}

// -------- MOVEMENT --------
void moveForward() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
}

void turnLeft() {
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
}

void turnRight() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, HIGH);
}

void stopMoving() {
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, HIGH);
}