#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <splash.h>
#include <Adafruit_ADS1015.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define clk 2
#define dt 3
#define sw 4
#define ACS A0
#define cont_test 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_ADS1115 ads;

//Updated by the ISR
volatile boolean TurnDetected = false;
volatile boolean up = false;
volatile boolean button = false;
boolean mode = false;


ISR(PCINT2_vect) {
  if (digitalRead(sw) == LOW) {
    button = true;
  }
}

void isr0 ()  {
  TurnDetected = true;
  up = (digitalRead(clk) == digitalRead(dt));
}
/*
void displaydata(){
 display.setCursor(0,5);
 display.print("V = ");
 display.print(voltreading);
 display.setCursor(0,12);
 display.print("A =");
 display.print(ampreading);
}*/

// ------------------------------------------------------------------
// SETUP    SETUP    SETUP
// ------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  pinMode(ACS, INPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(cont_test, INPUT_PULLUP);
  
  //attach Interrupt to Pin 2 (CLK)
  attachInterrupt (digitalPinToInterrupt(2), isr0, FALLING);
  
  ads.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Adress 0x3c for 128x32 version
  
  display.setTextSize(2);   //from 1 to 8
  display.clearDisplay();   //reset all pixels
  display.setTextColor(WHITE);  //white text
  //display.setCursor(0,5);   //set cursor at x=0, y=0
  //display.println(“startup”)  //print the characters at location x,y
}

// ------------------------------------------------------------------
// LOOP     LOOP      LOOP
// ------------------------------------------------------------------
void loop() {
  int voltreading = ads.readADC_Differential_0_1();
  int ampreading = analogRead(ACS);
  display.setCursor(0,5);
  display.print("V = ");
  display.print(voltreading);
  display.setCursor(0,12);
  display.print("A =");
  display.print(ampreading);

  if (!(digitalRead(sw))){
    mode=!mode;
    Serial.println("Button pressed");
    delay(500);
  }
    
  if (TurnDetected){
    Serial.print("Rotating...");
      if(up){
        Serial.print("to the right");
      }
    else {
      Serial.print("to the left");
    }
  }
  else {
    TurnDetected = false;
  }
delay(500);
}
