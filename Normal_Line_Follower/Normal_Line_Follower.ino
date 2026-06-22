#define LEFT_IR_PIN 9
#define RIGHT_IR_PIN 12
#define AVOID_LEFT 8
#define AVOID_RIGHT 7

#define MOTOR1_PIN1 4
#define MOTOR1_PIN2 3
#define MOTOR2_PIN1 5
#define MOTOR2_PIN2 6

void setup() {
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
}

void loop() {

  int leftSensor = digitalRead(LEFT_IR_PIN);
  int rightSensor = digitalRead(RIGHT_IR_PIN);
  
  if (leftSensor == LOW && rightSensor == LOW) {
    moveForward();
  } 

  else if (leftSensor == LOW && rightSensor == HIGH) {
    turnRight();
  } 

  else if (leftSensor == HIGH && rightSensor == LOW) {
    turnLeft();
  } 

  else {
    stopMoving();
  }
}

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
