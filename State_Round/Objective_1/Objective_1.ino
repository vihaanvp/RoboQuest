// Define IR sensor pins
#define LEFT_IR_PIN 10
#define RIGHT_IR_PIN 9

// Define motor pins
#define MOTOR1_PIN1 5
#define MOTOR1_PIN2 4
#define MOTOR2_PIN1 3
#define MOTOR2_PIN2 2

#define TRIG_PIN 6
#define ECHO_PIN 7

long duration;
float distance;

int y = 0;

int leftSensor;
int rightSensor;

enum RobotState { FOLLOW_LINE,
                  STOP_2s,
                  FOLLOW_LINE_2,
                  STOP_3s,
                  FOLLOW_LINE_3,
                  STOP_3s_2,
                  FOLLOW_LINE_4,
                  FINAL_STOP_1s,
                  BOTTLES_PRE_CIRCLE,
                  BOTTLES_IN_CIRCLE };
RobotState CurrentState = FOLLOW_LINE;

void setup() {
  // Initialize IR sensor pins as inputs
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);

  // Initialize motor pins as outputs
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  leftSensor = digitalRead(LEFT_IR_PIN);
  rightSensor = digitalRead(RIGHT_IR_PIN);

  // Trigger ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo with timeout (prevents freeze)
  duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms timeout

  // Convert to distance
  if (duration == 0) {
    distance = -1;  // no reading / out of range
  } else {
    distance = duration * 0.034 / 2;
  }

  switch (CurrentState) {
    case FOLLOW_LINE:
      leftSensor = digitalRead(LEFT_IR_PIN);
      rightSensor = digitalRead(RIGHT_IR_PIN);

      // If both sensors are on the line, move forward
      if (leftSensor == LOW && rightSensor == LOW) {
        moveForward();
      }
      // If left sensor detects the line, turn right
      else if (leftSensor == LOW && rightSensor == HIGH) {
        turnRight();
      }
      // If right sensor detects the line, turn left
      else if (leftSensor == HIGH && rightSensor == LOW) {
        turnLeft();
      }
      // If both sensors are off the line, stop
      else if (leftSensor == HIGH && rightSensor == HIGH) {
        CurrentState = STOP_2s;
      }
      break;

    case STOP_2s:
      stopMoving();
      delay(2000);
      moveForward();
      delay(250);
      CurrentState = FOLLOW_LINE_2;
      break;

    case FOLLOW_LINE_2:
      leftSensor = digitalRead(LEFT_IR_PIN);
      rightSensor = digitalRead(RIGHT_IR_PIN);

      // If both sensors are on the line, move forward
      if (leftSensor == LOW && rightSensor == LOW) {
        moveForward();
      }
      // If left sensor detects the line, turn right
      else if (leftSensor == LOW && rightSensor == HIGH) {
        turnRight();
      }
      // If right sensor detects the line, turn left
      else if (leftSensor == HIGH && rightSensor == LOW) {
        turnLeft();
      }
      // If both sensors are off the line, stop
      else if (leftSensor == HIGH && rightSensor == HIGH) {
        CurrentState = STOP_3s;
      }
      break;

    case STOP_3s:
      stopMoving();
      delay(2000);
      moveForward();
      delay(200);
      CurrentState = FOLLOW_LINE_3;
      break;

    case FOLLOW_LINE_3:
      leftSensor = digitalRead(LEFT_IR_PIN);
      rightSensor = digitalRead(RIGHT_IR_PIN);

      if (distance <= 17 && distance >= 1) {
        digitalWrite(MOTOR1_PIN1, LOW);
        digitalWrite(MOTOR1_PIN2, LOW);
        digitalWrite(MOTOR2_PIN1, LOW);
        digitalWrite(MOTOR2_PIN2, LOW);
        delay(1000);
      } else {
        Serial.println(distance);
        if (leftSensor == LOW && rightSensor == LOW) {
          moveForward();
        }
        // If left sensor detects the line, turn right
        else if (leftSensor == LOW && rightSensor == HIGH) {
          turnRight();
        }
        // If right sensor detects the line, turn left
        else if (leftSensor == HIGH && rightSensor == LOW) {
          turnLeft();
        }
        // If both sensors are off the line, stop
        else if (leftSensor == HIGH && rightSensor == HIGH) {
          CurrentState = STOP_3s_2;
        }
      }
      break;

    case STOP_3s_2:
      stopMoving();
      delay(2000);
      moveForward();
      delay(250);
      CurrentState = FOLLOW_LINE_4;
      break;

    case FOLLOW_LINE_4:
      leftSensor = digitalRead(LEFT_IR_PIN);
      rightSensor = digitalRead(RIGHT_IR_PIN);

      // If both sensors are on the line, move forward
      if (leftSensor == LOW && rightSensor == LOW) {
        moveForward();
      }
      // If left sensor detects the line, turn right
      else if (leftSensor == LOW && rightSensor == HIGH) {
        turnRight();
      }
      // If right sensor detects the line, turn left
      else if (leftSensor == HIGH && rightSensor == LOW) {
        turnLeft();
      }
      // If both sensors are off the line, stop
      else if (leftSensor == HIGH && rightSensor == HIGH) {
        CurrentState = FINAL_STOP_1s;
      }
      break;

    case FINAL_STOP_1s:
      stopMoving();
      delay(1000);
      CurrentState = BOTTLES_PRE_CIRCLE;
      break;

    case BOTTLES_PRE_CIRCLE:
      moveForward();
      if (leftSensor == HIGH && rightSensor == HIGH) {
        y = 1;
      }
      delay(400);
      stopMoving();
      CurrentState = BOTTLES_IN_CIRCLE;
      break;

    case BOTTLES_IN_CIRCLE:
      moveForward();
      if (leftSensor == HIGH && rightSensor == HIGH && y == 1) {
        digitalWrite(MOTOR1_PIN1, HIGH);
        digitalWrite(MOTOR1_PIN2, LOW);
        digitalWrite(MOTOR2_PIN1, HIGH);
        digitalWrite(MOTOR2_PIN2, LOW);
        delay(400);

        digitalWrite(MOTOR1_PIN1, LOW);
        digitalWrite(MOTOR1_PIN2, HIGH);
        digitalWrite(MOTOR2_PIN1, HIGH);
        digitalWrite(MOTOR2_PIN2, LOW);
        delay(600);

        digitalWrite(MOTOR1_PIN1, LOW);
        digitalWrite(MOTOR1_PIN2, LOW);
        digitalWrite(MOTOR2_PIN1, LOW);
        digitalWrite(MOTOR2_PIN2, LOW);
        delay(500);
      }

      if (leftSensor == LOW && rightSensor == HIGH) {
        turnRight();
        delay(200);
        stopMoving();
      }
      // If right sensor detects the line, turn left
      if (leftSensor == HIGH && rightSensor == LOW) {
        turnLeft();
        delay(200);
        stopMoving();
      }
      break;
  }
}

// Function to move the robot forward
void moveForward() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
}

// Function to turn the robot left
void turnLeft() {
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
}

// Function to turn the robot right
void turnRight() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, HIGH);
}

// Function to stop the robot
void stopMoving() {
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW);
}