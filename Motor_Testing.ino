// --- PIN DEFINITIONS (Motor Driver - L293D or similar) ---
// Pin 9 MUST be a PWM pin for speed control (Enable 1 or 2)
#define MOTOR_ENABLE_PIN 9 // Connected to ENA or ENB pin of L293D
#define MOTOR_IN1_PIN 7    // Connected to IN1 (or IN3) pin of L293D
#define MOTOR_IN2_PIN 8    // Connected to IN2 (or IN4) pin of L293D

// Maximum PWM value for full speed (0-255)
const int MAX_SPEED_PWM = 255;

void setup() {
  Serial.begin(9600);

  // Initialize Motor Driver Pins as OUTPUTs
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);

  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, LOW);

  Serial.println("--- Motor Test Mode Started ---");
  Serial.print("Running motor at MAX Speed (PWM: ");
  Serial.print(MAX_SPEED_PWM);
  Serial.println(")");
}

void loop() {
  
  analogWrite(MOTOR_ENABLE_PIN, MAX_SPEED_PWM);

  delay(1000);
}
