#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ImageReader.h>

const int TFT_CS = 1;
const int TFT_DC = 0;
const int TFT_MOSI = 2;
const int TFT_SCLK = 3;
const int TFT_RST = 11;
const int TFT_BACKLIGHT = 4;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Just for reference, the display is 160x80 pixels
const int TFT_WIDTH = 160;
const int TFT_HEIGHT = 80;

void setup() {
  // Initialize the TFT display
  // Turn on the backlight
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);

  delay(500); // Add a short delay before initializing the display.

  // Initialize configuration for the TFT display.
  tft.initR(INITR_GREENTAB);

  tft.setRotation(3); // Set desired rotation (landscape)

  // Fix color inversion
  tft.invertDisplay(true);

  // Test different colors on the display
  tft.fillScreen(ST77XX_WHITE);
  tft.fillScreen(ST77XX_RED);
  tft.fillScreen(ST77XX_GREEN);
  tft.fillScreen(ST77XX_BLUE);
  tft.fillScreen(ST77XX_BLACK);
  
  // // Black background
  // tft.fillScreen(ST77XX_BLACK);

  // Draw some text
  tft.setCursor(0, 24);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(2);
  tft.println("Roadbobek");

  // Draw more some text
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.println("Roadbobek");

  // Even more text
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(2);
  tft.println("Roadbobek");

  // Even even more text
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  tft.println("Roadbobek");

  // Even even even more text
  tft.setTextColor(ST77XX_MAGENTA);
  tft.setTextSize(2);
  tft.println("Roadbobek");


  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only.
  }
  
  Serial.println("Hello from your Cyber B Necklace!");
}

void loop() {
  // The loop() function runs over and over again forever.
  // For this example, we don't need to do anything here.
  delay(1000);
  Serial.println("Still here!");
}
