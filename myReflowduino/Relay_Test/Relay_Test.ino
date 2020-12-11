#define relay 7

char charBuff[15];

void setup() {
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW); // Default is OFF

//   while (!Serial) delay(1);
  Serial.println("Relay Test");
  Serial.println("Enter \"ON\" or \"OFF\" into serial monitor");
}

void loop() {
  if (!Serial.available()) return;
  
  int x = 0;
  while (Serial.available()) {
    charBuff[x] = Serial.read();
    x++;
  }
  charBuff[x] = 0; // Null termination
  
  if (strcmp(charBuff, "ON") == 0) {
    Serial.println("Turning relay ON!");
    digitalWrite(relay, HIGH);
  }
  else if (strcmp(charBuff, "OFF") == 0) {
    Serial.println("Turning relay OFF!");
    digitalWrite(relay, LOW);
  }
}
