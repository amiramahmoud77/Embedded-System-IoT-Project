// Pin definitions according to the schematic
#define speedL 10  // PWM for left motor
#define IN1 9      // Left motor control 1
#define IN2 8      // Left motor control 2
#define IN3 7      // Right motor control 1
#define IN4 6      // Right motor control 2
#define speedR 5   // PWM for right motor
#define sensorL 4  // Left sensor (IR)
#define sensorR 3  // Right sensor (IR)

int sl = 0;  // Left sensor value
int sr = 0;  // Right sensor value
int stopCounter = 0;  // Stop counter (optional)

void setup() {
  // Set motor pins as output
  for (int i = 5; i <= 10; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(sensorL, INPUT);
  pinMode(sensorR, INPUT);
  
  // Serial for debug (view in Serial Monitor)
  Serial.begin(9600);
  Serial.println("Line Follower Robot Started");
}

void forward() {  // Move forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(speedL, 150);  // Speed 150/255
  analogWrite(speedR, 150);
}

void backward() {  // Move backward (if robot lost)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(speedL, 150);
  analogWrite(speedR, 150);
}

void left() {  // Turn left
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(speedL, 0);
  analogWrite(speedR, 150);
}

void right() {  // Turn right
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(speedL, 150);
  analogWrite(speedR, 0);
}

void stopp() {  // Stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(speedL, 0);
  analogWrite(speedR, 0);
}

void loop() {
  sl = digitalRead(sensorL);  // Read left sensor
  sr = digitalRead(sensorR);  // Read right sensor
  
  // Print to Serial for debug
  Serial.print("Left Sensor: ");
  Serial.print(sl);
  Serial.print(" | Right Sensor: ");
  Serial.println(sr);
  
  if (sl == 0 && sr == 0) {  // Both on line → forward
    forward();
    stopCounter = 0;
  } else if (sl == 0 && sr == 1) {  // Left on line → right
    right();
    stopCounter = 0;
  } else if (sl == 1 && sr == 0) {  // Right on line → left
    left();
    stopCounter = 0;
  } else if (sl == 1 && sr == 1) {  // Both off line → stop
    stopp();
    stopCounter++;
    if (stopCounter > 50) {  // If stopped too long, go back a bit
      backward();
      delay(200);
      stopCounter = 0;
    }
  }
  
  delay(10);  // Small delay for stability
}
