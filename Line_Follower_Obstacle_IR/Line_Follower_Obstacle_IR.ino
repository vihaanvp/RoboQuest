// Define IR sensor pins
#define LEFT_IR_PIN 9
#define RIGHT_IR_PIN 12
#define AVOID_LEFT 8
#define AVOID_RIGHT 7

// Define motor pins
#define MOTOR1_PIN1 4
#define MOTOR1_PIN2 3
#define MOTOR2_PIN1 5
#define MOTOR2_PIN2 6
#define EN1 10
#define EN2 11

void setup() {
  // Initialize IR sensor pins as inputs
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(AVOID_LEFT, INPUT);
  pinMode(AVOID_RIGHT, INPUT);
  
  // Initialize motor pins as outputs
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
}

void loop() {
  // Read sensor values
  analogWrite(EN1, 50);
  analogWrite(EN2, 50);

  int leftSensor = digitalRead(LEFT_IR_PIN);
  int rightSensor = digitalRead(RIGHT_IR_PIN);
  int avoidLeft = digitalRead(AVOID_LEFT);
  int avoidRight = digitalRead(AVOID_RIGHT);
  
  // Check for obstacle avoidance first
  if (avoidLeft == 0 && avoidRight == 0 || avoidLeft == 0 || avoidRight == 0) {
    stopMoving();
    delay(500); // Delay to avoid continuous triggering
    // Move backward and turn left
    digitalWrite(MOTOR1_PIN1, HIGH);
    digitalWrite(MOTOR1_PIN2, HIGH);
    digitalWrite(MOTOR2_PIN1, HIGH);
    digitalWrite(MOTOR2_PIN2, HIGH);
    delay(1000); // Delay to make the turn
    // Resume line following
    return;
  }

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
  else {
    stopMoving();
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
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, HIGH);
}
