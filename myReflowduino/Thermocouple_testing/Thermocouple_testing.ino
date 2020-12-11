#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <MAX6675.h>
#include <SPI.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define CS_PIN 8
MAX6675 tcouple(CS_PIN);
double sum = 0.0;
int i = 0;

void setup()
{
  Serial.begin(9600);
  delay(500);
  Serial.println("MAX6675 thermocouple test");
  Serial.println();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
}
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}
void loop()
{
  double celsius = tcouple.readTempC();
  celsius=100.0/(100.5-3.5)*celsius-3.5; //calibrated values
  Serial.println("T in C=");
  Serial.println(celsius);

  display.clearDisplay();
  display.setCursor(10, 0);
  display.print(celsius);
  
  display.display();
  
  sum = sum + celsius;
  //Serial.println(sum);
  i++;
  if(i==50) {
    sum = sum/50.0;
    Serial.println("average in C=");
    Serial.println(sum);
    display.setCursor(80, 0);
    display.print(sum);
    i=0;
    sum=0.0;
    display.display();
    delay(2000);
  }
  delay(250);
}
