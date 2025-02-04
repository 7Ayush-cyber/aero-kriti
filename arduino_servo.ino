#include <Servo.h> // Standard Arduino Servo library

Servo myServo; // Create a servo object

// Define the servo signal pin
#define SERVO_PIN 9 // Change this pin based on your setup

void setup() {
  // Attach the servo to the defined pin
  myServo.attach(SERVO_PIN);

  // Move the servo to the initial position
  myServo.write(0); // Set the servo to 0 degrees
  delay(5000); // Wait for 5 second
}

void loop() {
  // Wait for 2 minutes before moving the servo
  delay(120000); // 2 minutes = 120,000 milliseconds

  // Rotate the servo to 90 degrees
  myServo.write(90);
  delay(5000); // Wait for 5 seconds

  // Rotate the servo back to 0 degrees
  myServo.write(0);
  delay(5000); // Wait for another 5 seconds
}

