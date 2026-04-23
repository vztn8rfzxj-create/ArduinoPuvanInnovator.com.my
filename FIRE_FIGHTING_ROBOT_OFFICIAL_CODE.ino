#include <Servo.h>

// Define flame sensor pins
#define FLAME_SENSOR_LEFT A1
#define FLAME_SENSOR_CENTER A0
#define FLAME_SENSOR_RIGHT A2

// Define motor driver pins
#define ENB 2
#define ENA 7
#define IN1 3  // Left Motor Forward
#define IN2 4  // Left Motor Backward
#define IN3 5  // Right Motor Forward
#define IN4 6 // Right Motor Backward

// Define relay for water pump
#define RELAY_PIN 10

// Define servo motor pin
#define SERVO_PIN 8

// Create Servo object
Servo waterServo;

// Flame sensor threshold
#define FIRE_DETECTED_THRESHOLD 300  // Adjust based on sensor readings

void setup() {
    // Initialize Serial Monitor
    Serial.begin(9600);

    // Set motor pins as output
    pinMode(ENB, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // Set relay as output
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);  // Ensure relay (pump) is OFF at startup

    // Initialize servo motor
    waterServo.attach(SERVO_PIN);
    waterServo.write(90); // Set initial servo position to center

    // Set flame sensor pins as input
    pinMode(FLAME_SENSOR_LEFT, INPUT);
    pinMode(FLAME_SENSOR_CENTER, INPUT);
    pinMode(FLAME_SENSOR_RIGHT, INPUT);

    Serial.println("🚀 Fire Fighting Robot Initialized...");
}

void loop() {
    // Read flame sensor values
    int flameLeft = analogRead(FLAME_SENSOR_LEFT);
    int flameCenter = analogRead(FLAME_SENSOR_CENTER);
    int flameRight = analogRead(FLAME_SENSOR_RIGHT);

    // Print sensor values to Serial Monitor
    Serial.print("Flame Left: ");
    Serial.print(flameLeft);
    Serial.print(" | Flame Center: ");
    Serial.print(flameCenter);
    Serial.print(" | Flame Right: ");
    Serial.println(flameRight);

    // Check flame detection
    if (flameCenter < FIRE_DETECTED_THRESHOLD) {
        Serial.println("🔥 Fire Detected at Center! Moving Forward...");
        moveForward();
        activateWaterPump();
    } 
    else if (flameLeft < FIRE_DETECTED_THRESHOLD) {
        Serial.println("🔥 Fire Detected at Left! Turning Left...");
        turnLeft();
        activateWaterPump();
    } 
    else if (flameRight < FIRE_DETECTED_THRESHOLD) {
        Serial.println("🔥 Fire Detected at Right! Turning Right...");
        turnRight();
        activateWaterPump();
    } 
    else {
        Serial.println("✅ No Fire Detected. Stopping...");
        stopMotors();
        
     // Ensure pump is OFF when no fire is detected
    }

    delay(500); // Delay for stability
}

// Function to move forward
void moveForward() {
  int speed =110;
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    digitalWrite(ENB,speed);
    digitalWrite(ENA,speed);
}

// Function to turn left
void turnLeft() {
  int speed = 110;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    digitalWrite(ENB,speed);
    digitalWrite(ENA,speed);  // Adjust timing
    stopMotors();
    
}

// Function to turn right
void turnRight() {
  int speed =110;
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    digitalWrite(ENB,speed);
    digitalWrite(ENA,speed);  // Adjust timing
    stopMotors();
}

// Function to stop motors
void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(ENB,0);
    digitalWrite(ENA,0);
}

// Function to activate water pump and move servo
void activateWaterPump() {
    Serial.println("🚰 Activating Water Pump...");
    digitalWrite(RELAY_PIN, LOW);  // Turn ON the water pump

    // Move the servo from left to right
    for (int pos = 60; pos <= 120; pos += 5) {  
        waterServo.write(pos);
    }    
   // Move the servo from right to left
    for (int pos = 120; pos >= 60; pos -= 5) {  
        waterServo.write(pos);
        delay(100);
    }

    delay(2000);  // Pump stays on for 2 seconds
    deactivateWaterPump();  // Turn off the water pump

}        


// Function to deactivate water pump
void deactivateWaterPump() {
    Serial.println("✅ Deactivating Water Pump...");
    digitalWrite(RELAY_PIN, HIGH);  // Turn OFF the water pump
    waterServo.write(90); // Reset servo to center position
}
