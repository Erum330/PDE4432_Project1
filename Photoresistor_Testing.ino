#define LDR_PIN A0 

// --- CONTROL VARIABLES ---
int lightValue = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("--- Photoresistor (LDR) Reader Started ---");
  Serial.println("Light values range from 0 (Bright) to 1023 (Dark) depending on wiring.");
  delay(1000);
}

void loop() {
  
  // The value will range from 0 to 1023.
  lightValue = analogRead(LDR_PIN);

  Serial.print("Timestamp (ms): ");
  Serial.print(millis());
  Serial.print(" | Light Reading: ");
  Serial.println(lightValue);

  // Delay for a short period before the next reading
  delay(100);
}
