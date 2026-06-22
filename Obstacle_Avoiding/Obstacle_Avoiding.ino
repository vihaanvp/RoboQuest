#define LEFT_IR_PIN 9
#define RIGHT_IR_PIN 12

#define TRIG_PIN 8
#define ECHO_PIN 7

#define MOTOR1_PIN1 4
#define MOTOR1_PIN2 3
#define MOTOR2_PIN1 5
#define MOTOR2_PIN2 6

#define OBSTACLE_CM 12     // detect obstacle
#define CLEAR_CM 18        // consider "clear" when distance > this

// Timings (ms) - tune on your bot + arena
#define STOP_MS        80
#define TURN_OUT_MS   320   // turn right to leave line
#define GO_PAR_MS     450   // go forward to get beside obstacle
#define TURN_IN_MS    340   // turn left back towards line
#define REACQUIRE_MIN 250   // must see line continuously this long to "lock"
#define REACQUIRE_MAX 1800  // timeout to avoid getting stuck

enum State {
  FOLLOW_LINE,
  AVOID_STOP,
  AVOID_TURN_OUT,
  AVOID_GO_PARALLEL,
  AVOID_PASS_OBSTACLE,
  AVOID_TURN_IN,
  REACQUIRE
};

State state = FOLLOW_LINE;
unsigned long stateStart = 0;

// for line validation in REACQUIRE
unsigned long lineSeenStart = 0;

void setup() {
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  stopMoving();
}

void loop() {
  switch (state) {
    case FOLLOW_LINE: {
      int d = distanceCM();
      if (d > 0 && d <= OBSTACLE_CM) {
        enterState(AVOID_STOP);
        break;
      }
      followLine();
      break;
    }

    case AVOID_STOP:
      stopMoving();
      if (elapsed(STOP_MS)) enterState(AVOID_TURN_OUT);
      break;

    case AVOID_TURN_OUT:
      // leave the line: turn right in place
      turnRight();
      if (elapsed(TURN_OUT_MS)) enterState(AVOID_GO_PARALLEL);
      break;

    case AVOID_GO_PARALLEL:
      // move forward to get alongside the obstacle
      moveForward();
      if (elapsed(GO_PAR_MS)) enterState(AVOID_PASS_OBSTACLE);
      break;

    case AVOID_PASS_OBSTACLE: {
      // keep moving forward until ultrasonic says obstacle is no longer close
      moveForward();
      int d = distanceCM();
      if (d < 0) break; // no reading; keep going
      if (d > CLEAR_CM) {
        enterState(AVOID_TURN_IN);
      }
      break;
    }

    case AVOID_TURN_IN:
      // turn left to head back toward the line area
      turnLeft();
      if (elapsed(TURN_IN_MS)) {
        enterState(REACQUIRE);
        lineSeenStart = 0; // reset validation
      }
      break;

    case REACQUIRE: {
      // Go slow-ish forward and only accept line if stable.
      // (We keep using your digital motor writes; "slow" is just forward with short checks.)
      moveForward();

      bool L = (digitalRead(LEFT_IR_PIN) == LOW);
      bool R = (digitalRead(RIGHT_IR_PIN) == LOW);
      bool seen = (L || R);

      if (seen) {
        if (lineSeenStart == 0) lineSeenStart = millis();
        // require stable line detection before locking
        if (millis() - lineSeenStart >= REACQUIRE_MIN) {
          enterState(FOLLOW_LINE);
        }
      } else {
        lineSeenStart = 0;
      }

      // timeout: if we can't find line, do a gentle spin search then try again
      if (elapsed(REACQUIRE_MAX)) {
        // search behavior: rotate a bit, then re-enter REACQUIRE
        turnLeft();
        delay(250);
        enterState(REACQUIRE);
        lineSeenStart = 0;
      }
      break;
    }
  }
}

// ---------------- Helpers ----------------
void enterState(State s) {
  state = s;
  stateStart = millis();
}

bool elapsed(unsigned long ms) {
  return (millis() - stateStart) >= ms;
}

int distanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  if (duration == 0) return -1;
  return (int)(duration * 0.0343 / 2.0);
}

// ---------------- Line following (your logic) ----------------
void followLine() {
  int leftSensor  = digitalRead(LEFT_IR_PIN);
  int rightSensor = digitalRead(RIGHT_IR_PIN);

  if (leftSensor == LOW && rightSensor == LOW) moveForward();
  else if (leftSensor == LOW && rightSensor == HIGH) turnRight();
  else if (leftSensor == HIGH && rightSensor == LOW) turnLeft();
  else stopMoving();
}

// ---------------- Motors (your style) ----------------
void moveForward() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
}

void moveBackward() {
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
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
