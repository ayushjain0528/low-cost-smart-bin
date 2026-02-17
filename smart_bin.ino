#include <Servo.h>

// Ultrasonic pins
const int trigPin = 6;
const int echoPin = 7;

// Moisture sensor
const int moisturePin = A0;

// Servo objects
Servo lidServo;
Servo flapServo;

// Variables
long duration;
int distance;
int moistureValue;

// Settings
int lidOpenAngle = 90;
int lidCloseAngle = 0;

int flapWetAngle = 0;
int flapDryAngle = 180;
int flapNeutral = 90;

int moistureThreshold = 500;  // Adjust after testing

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lidServo.attach(9);
  flapServo.attach(10);

  lidServo.write(lidCloseAngle);
  flapServo.write(flapNeutral);

  Serial.println("Smart Bin Ready");
}

void loop() {

  distance = calculateDistance();

  if (distance > 0 && distance < 20) {   // Hand detected

    Serial.println("Opening Lid...");
    lidServo.write(lidOpenAngle);
    delay(2000);  // Wait for waste drop

    moistureValue = analogRead(moisturePin);
    Serial.print("Moisture Value: ");
    Serial.println(moistureValue);

    if (moistureValue > moistureThreshold) {
      Serial.println("Wet Waste Detected");
      flapServo.write(flapWetAngle);
    } else {
      Serial.println("Dry Waste Detected");
      flapServo.write(flapDryAngle);
    }

    delay(3000);   // Let waste fall

    flapServo.write(flapNeutral);
    delay(500);

    lidServo.write(lidCloseAngle);
    Serial.println("Lid Closed");

    delay(3000);   // Prevent immediate retrigger
  }

  delay(300);
}

// Distance calculation function
int calculateDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout

  if (duration == 0) return -1;

  int dist = duration * 0.034 / 2;
  return dist;
}
