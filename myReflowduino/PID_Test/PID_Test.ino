#include <SoftwareSerial.h>

#include <SPI.h>
#include "Adafruit_MAX31855.h"

#include <PID_v1.h>

#define relay 7
#define BT_RX 9
#define BT_TX 10
#define LED 13
#define MAX_CS 8

Adafruit_MAX31855 thermocouple(MAX_CS);

#define T_const 5;

#define desiredTemp 75 - T_const

// PID parameters.
#define PID_sampleTime 1000
#define Kp 150
#define Ki 0
#define Kd 100

double temperature, output, setPoint; // Input, output, set point
PID myPID(&temperature, &output, &setPoint, Kp, Ki, Kd, DIRECT);

// Logic flags
bool justStarted = true;
bool reflow = false; // Baking process is underway!

int windowSize = 2000;
unsigned long previousMillis = 0;
unsigned long windowStartTime, timer;

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(relay, OUTPUT);

  digitalWrite(LED, LOW);
  digitalWrite(relay, LOW); // Set default relay state to OFF

  setPoint = desiredTemp;

  myPID.SetOutputLimits(0, windowSize);
  myPID.SetSampleTime(PID_sampleTime);
  myPID.SetMode(AUTOMATIC); // Turn on PID control

//   while (!Serial) delay(1); // OPTIONAL: Wait for serial to connect
  Serial.println("PID Test");
}

void loop() {
  temperature = thermocouple.readCelsius(); // Read temperature
  
  if (reflow) {
    digitalWrite(LED, HIGH); // Red LED indicates process is underway

    // This only runs when you first start the test
    if (justStarted) {
      justStarted = false;
      windowStartTime = millis();
      
      if (isnan(temperature)) {
       Serial.println("Invalid reading, check thermocouple!");
      }
      else {
       Serial.print("Starting temperature: ");
       Serial.print(temperature);
       Serial.println(" *C");
      }
    }
  
    // Compute PID output (from 0 to windowSize) and control relay accordingly
    myPID.Compute(); // This will only be evaluated at the PID sampling rate
    if (millis() - windowStartTime >= windowSize) windowStartTime += windowSize; // Shift the time window
    if (output > millis() - windowStartTime) digitalWrite(relay, HIGH);
    else digitalWrite(relay, LOW);
  }
  else digitalWrite(LED, LOW);

  if (request == startReflow) { // Command from app to start reflow process
    justStarted = true;
    reflow = true; // Reflow started!
    timer = millis(); // Timer for logging data points
    Serial.println("<-- ***Reflow process started!"); // Left arrow means it received a command
  }
  else if (request == stopReflow) { // Command to stop reflow process
    digitalWrite(relay, LOW); // Turn off appliance and set flag to stop PID control
    reflow = false;
    Serial.println("<-- ***Reflow process aborted!");
  }
  
  char serialByte;
  
  if (Serial.available() > 0) {
    serialByte = Serial.read();
  }
  if (serialByte == startReflow) {
    justStarted = true;
    reflow = true; // Reflow started!
    t_start = millis(); // Record the start time
    timer = millis(); // Timer for logging data points
    Serial.println("<-- ***Reflow process started!"); // Left arrow means it received a command
  }
  else if (serialByte == stopReflow) { // Command to stop reflow process
    digitalWrite(relay, LOW); // Turn off appliance and set flag to stop PID control
    reflow = false;
    Serial.println("<-- ***Reflow process aborted!");
  }
}
