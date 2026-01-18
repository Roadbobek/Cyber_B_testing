#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Fonts/FreeSansBold9pt7b.h>

const int TFT_CS = 1;
const int TFT_DC = 0;
const int TFT_MOSI = 2;
const int TFT_SCLK = 3;
const int TFT_RST = 11;
const int TFT_BACKLIGHT = 4;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// The setup() function runs once when you press reset or power on the board.
void setup() {
  // Setup BT6 on the board, which is our only usable button.
  pinMode(6, INPUT_PULLUP); // Set pin 6 as input with internal pull-up resistor for the button, making its default state HIGH, and LOW when pressed.

  // Initialize the TFT display
  // Turn on the backlight
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);

  // delay(500); // Add a short delay before initializing the display.

  // Initialize configuration for the TFT display.
  tft.initR();

  tft.setRotation(3); // Set desired rotation (landscape)

  // Fix color inversion
  tft.invertDisplay(true);

  // // Black background
  tft.fillScreen(ST77XX_MAGENTA);

  // Set text font
  tft.setFont(&FreeSansBold9pt7b);

  // Draw some text
  // This information is when using setTextSize(1), setCursor() alignment seems to increase by 12 pixels per size increment.
  // The default fonts origin is the top left corner of the text bounding box, we use (0,24) to align it properly below the top edge of the display.
  // The new font's origin is at the baseline of the text, so we set the cursor to (0,36) to align it properly below the top edge of the display.
  tft.setCursor(5, 60 + 20);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.println("I <3 U");

}

void loop() {
  // put your main code here, to run repeatedly:
}