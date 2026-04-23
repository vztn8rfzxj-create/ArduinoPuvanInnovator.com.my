#include <Servo.h>

// Pin definitions
const int flameSensorPin = 2;
const int motor1Pin1 = 3;  // Motor A
const int motor1Pin2 = 4;
const int motor2Pin1 = 5;  // Motor B
const int motor2Pin2 = 6;
const int relayPin = 7;
const int servoPin = 9;

Servo scanner;

void setup() {
  pinMode(flameSensorPin, INPUT);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Pump off initially (active LOW relay)

  scanner.attach(servoPin);

  Serial.begin(9600);
}

void loop() {
  // Sweep left to right
  for (int angle = 45; angle <= 135; angle += 5) {
    scanner.write(angle);
    delay(150);
    if (detectFire()) {
      handleFire();
    }
  }

  // Sweep right to left
  for (int angle = 135; angle >= 45; angle -= 5) {
    scanner.write(angle);
    delay(150);
    if (detectFire()) {
      handleFire();
    }
  }

  moveForward();
}

// Detect fire (LOW = fire for most flame sensors)
bool detectFire() {
  return digitalRead(flameSensorPin) == LOW;
}

// When fire is detected
void handleFire() {
  Serial.println("🔥 Fire detected!");
  stopMotors();
  digitalWrite(relayPin, LOW);   // Turn on pump
  delay(5000);                   // Spray water for 5 seconds
  digitalWrite(relayPin, HIGH);  // Turn off pump
  delay(2000);                   // Pause before resuming
}

// Move forward with both motors
void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

// Stop all motors
void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}
