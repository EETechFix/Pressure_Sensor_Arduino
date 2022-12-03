/*********************************************************************
 This sketch controls an electronic circuit that can sense 
 the environment using a sensor (electronic components that convert 
 real-world measurements into electrical signals). The device 
 processes the information from the sensors with the behavior 
 described in the sketch. The device will then be able to interact 
 with the world by using actuators, electronic components that can 
 convert an electric signal into physical action.
 
 EETechFix
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // display width, in pixels
#define SCREEN_HEIGHT 64 // display height, in pixels
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3D // OLED

const int sensorPin = A0;
const float alpha = 0.95; // Low Pass Filter alpha (0 - 1 )
const float aRef = 4.6; // analog reference
float filteredVal = 512.0; // midway starting point
float sensorVal;
float voltage;
float psiVal;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()   {
  Serial.begin(9600);
  while (!Serial);

  //analogReference(EXTERNAL);
  //analogReference(INTERNAL);

  //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1); // Don't proceed, wait forever
  }
  display.clearDisplay();
  display.display();
}

void loop() {

  sensorVal = (float)analogRead(sensorPin); // Read pressure sensor val (A0)
  filteredVal = (alpha * filteredVal) + ((1 - alpha) * sensorVal);// Low Pass Filter
  voltage = (filteredVal / 1024.0) * aRef; // calculate voltage
  psiVal = (voltage - 0.4784) / 0.0401; // x=(y-b)/m

  Serial.print ("raw_adc "); Serial.print (sensorVal, 0);
  Serial.print(", filtered_adc "); Serial.println (filteredVal, 0);
  //Serial.print(", voltage ");
  //Serial.println(voltage, 2);

  updateOLED ();

  delay(200);
}

void updateOLED () {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("filtered_adc ");
  display.print(filteredVal, 0);

  display.setCursor(0, 15);
  display.print("voltage ");
  display.print(voltage, 2);
  display.print(" Volts");

  display.setTextSize(3);
  display.setCursor(0, 30);
  display.print(psiVal, 0);
  display.print(" psi");
  display.display();
}
