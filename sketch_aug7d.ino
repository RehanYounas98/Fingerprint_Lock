const int trigPin = 3;
const int echoPin = 2;
const int buzzer = 5;

long duration;
int distance;
const float speedOfSound = 0.0343;

char command = ' ';           // Command from Bluetooth
bool isCommandRunning = false; // Is any app command active
unsigned long commandStartTime = 0;
unsigned long commandDuration = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // 🔷 Check for Bluetooth command
  if (Serial.available() > 0) {
    command = Serial.read();
    isCommandRunning = true;
    commandStartTime = millis();

    if (command == 'T') {
      commandDuration = 1000;  // Test buzzer: 1 sec
    } else if (command == 'F') {
      commandDuration = 3000;  // Find my glasses: 3 sec
    } else if (command == 'S') {
      commandDuration = 500;   // Short beep: 0.5 sec
    } else if (command == 'L') {
      commandDuration = 1500;  // Long beep: 1.5 sec
    } else {
      isCommandRunning = false; // Unknown command
    }

    if (isCommandRunning) {
      digitalWrite(buzzer, HIGH); // Start buzzer
    }
  }

  // 🔷 Turn off buzzer after command duration ends
  if (isCommandRunning && (millis() - commandStartTime >= commandDuration)) {
    digitalWrite(buzzer, LOW); // Stop buzzer
    isCommandRunning = false;  // Reset flag
  }

  // 🔷 Obstacle detection only if no command is running
  if (!isCommandRunning) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * speedOfSound) / 2;

    if (distance <= 30 && distance > 0) {
      digitalWrite(buzzer, HIGH);
    } else {
      digitalWrite(buzzer, LOW);
    }

    Serial.print("Distance: ");
    Serial.println(distance);
  }

  delay(50); // Slight delay for stability
}