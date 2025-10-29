// ==================== LIBRARIES ====================
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ==================== PIN DEFINITIONS ====================
#define LDR_PIN A0
#define DHT_PIN 2
#define DHTTYPE DHT11
#define MOTOR_ENABLE_PIN 9
#define MOTOR_IN1_PIN 7
#define MOTOR_IN2_PIN 8

// ==================== OBJECTS ====================
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHTTYPE);

// ==================== PID GAINS ====================
double Kp = 60.0;
double Ki = 0.5;
double Kd = 0.05;

// ==================== CONTROL VARIABLES ====================
double T_Setpoint = 27.0;
double T_Actual   = 0.0;
double fanOutput  = 0.0;

double propError = 0;
double lastPropError = 0;
double integralError = 0;
double diffError = 0;

const double outputLimitMin = 0;
const double outputLimitMax = 255;

// Timing
unsigned long currentTime;
unsigned long previousTime = 0;
const float SAMPLE_TIME_SEC = 1.0;   // 1-second sample period

// ==================== OTHER CONSTANTS ====================
const float T_MIN = 28.7;     // Setpoint in bright light
const float T_MAX = 30.0;     // Setpoint in darkness
const int FAN_DEADBAND = 15;  // Ignore tiny PID outputs
const double TEMP_DEADBAND = 0.1;  // ±0.2°C near setpoint = fan off
const int MIN_START_PWM = 150;     // Minimum speed once running

// ==================== FUNCTIONS ====================

// --- Adaptive Setpoint from LDR ---
void calculateAdaptiveSetpoint() {
  int lightReading = analogRead(LDR_PIN);
  const int LDR_MIN_RANGE = 50;
  const int LDR_MAX_RANGE = 800;
  int constrainedReading = constrain(lightReading, LDR_MIN_RANGE, LDR_MAX_RANGE);

  // Dark → higher allowed temp (T_MAX), Bright → lower temp (T_MIN)
  T_Setpoint = map(constrainedReading, LDR_MIN_RANGE, LDR_MAX_RANGE,
                   (int)(T_MAX * 10), (int)(T_MIN * 10)) / 10.0;
}

// --- Manual PID computation (Corrected Direction) ---
double computePID() {
  currentTime = millis();
  double deltaTime = (double)(currentTime - previousTime) / 1000.0;
  if (deltaTime <= 0) deltaTime = SAMPLE_TIME_SEC;

  // Positive when too hot
  propError = T_Actual - T_Setpoint;

  // P term
  double pTerm = Kp * propError;

  // I term
  integralError += propError * deltaTime;
  // Anti-windup
  if (integralError > outputLimitMax) integralError = outputLimitMax;
  else if (integralError < outputLimitMin) integralError = outputLimitMin;
  double iTerm = Ki * integralError;

  // D term
  diffError = (propError - lastPropError) / deltaTime;
  double dTerm = Kd * diffError;

  // Output
  double output = pTerm + iTerm + dTerm;

  // Clamp output
  if (output > outputLimitMax) output = outputLimitMax;
  else if (output < outputLimitMin) output = outputLimitMin;

  // Integral bleed-off near setpoint (prevents holding small PWM)
  if (abs(propError) < TEMP_DEADBAND) integralError *= 0.9;

  lastPropError = propError;
  previousTime = currentTime;

  return output;
}

// --- Apply fan output (smooth LDR-scaled PWM) ---
int applyFanOutput(double pidOutput) {
  int lightReading = analogRead(LDR_PIN);
  lightReading = constrain(lightReading, 50, 800);
  int lightFactor = map(lightReading, 50, 800, 0, 255);

  // Scale PID output by light level (dark → less fan)
  int scaledOutput = (int)(pidOutput * (lightFactor / 255.0));
  scaledOutput = constrain(scaledOutput, 0, 255);

  int appliedPWM = 0;

  // Deadband near setpoint → turn fan off
  if (abs(propError) < TEMP_DEADBAND) {
    appliedPWM = 0;
  } else if (scaledOutput < FAN_DEADBAND) {
    appliedPWM = 0;
  } else {
    // Soft minimum: ramp from 150 up
    appliedPWM = map(scaledOutput, FAN_DEADBAND, 255, MIN_START_PWM, 255);
  }

  analogWrite(MOTOR_ENABLE_PIN, appliedPWM);
  return appliedPWM;
}

// --- LCD Display ---
void displayData(int ldrValue, double setTemp, double actTemp, int pwmPercent) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LDR:");
  lcd.print(ldrValue);
  lcd.print(" Set:");
  lcd.print(setTemp, 1);

  lcd.setCursor(0, 1);
  lcd.print("Act:");
  lcd.print(actTemp, 1);
  lcd.print(" FAN:");
  lcd.print(pwmPercent);
  lcd.print("%");
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Adaptive Climate");
  lcd.setCursor(0, 1);
  lcd.print("Manual PID Init...");

  dht.begin();
  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);
  digitalWrite(MOTOR_IN1_PIN, HIGH);
  digitalWrite(MOTOR_IN2_PIN, LOW);

  previousTime = millis();
  delay(2000);
  lcd.clear();
  Serial.println("--- Adaptive Climate Controller (Manual PID + Smooth PWM) ---");
}

// ==================== MAIN LOOP ====================
void loop() {
  // 1. Read temperature
  float newTemp = dht.readTemperature();
  if (!isnan(newTemp)) T_Actual = newTemp;
  else Serial.println("DHT11 Read Failed!");

  // 2. Adaptive setpoint
  calculateAdaptiveSetpoint();

  // 3. Compute manual PID output
  fanOutput = computePID();

  // 4. Apply scaled fan control
  int appliedPWM = applyFanOutput(fanOutput);

  // 5. Display
  int lightValue = analogRead(LDR_PIN);
  int pwmPercent = (int)(appliedPWM * 100.0 / 255.0);
  displayData(lightValue, T_Setpoint, T_Actual, pwmPercent);

  // 6. Serial debug
  double error = T_Actual - T_Setpoint;
  Serial.print("LDR: "); Serial.print(lightValue);
  Serial.print(" | Set: "); Serial.print(T_Setpoint, 1);
  Serial.print("C | Act: "); Serial.print(T_Actual, 2);
  Serial.print("C | Err: "); Serial.print(error, 2);
  Serial.print(" | P: "); Serial.print(Kp * error, 1);
  Serial.print(" | I: "); Serial.print(Ki * integralError, 1);
  Serial.print(" | D: "); Serial.print(Kd * diffError, 1);
  Serial.print(" | Out: "); Serial.print(fanOutput, 1);
  Serial.print(" | PWM: "); Serial.println(appliedPWM);

  delay(1000);
}
