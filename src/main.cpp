#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Fonts/FreeSansBold9pt7b.h>

// --- Pin Configuration ---
const int TFT_CS = 1;
const int TFT_DC = 0;
const int TFT_MOSI = 2;
const int TFT_SCLK = 3;
const int TFT_RST = 11;
const int TFT_BACKLIGHT = 4;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// --- Screen Dimensions ---
const int16_t SCREEN_WIDTH = 160;
const int16_t SCREEN_HEIGHT = 80;

// --- Animation Variables ---
// The off-screen canvas has been removed for diagnostic purposes.
uint16_t textWidth, textHeight;    // Use uint16_t for correct type from getTextBounds
uint16_t boxWidth, boxHeight;      // Dimensions of the logo box
const int PADDING = 4;             // Padding around the text
int16_t textX, textY;              // Current position of the text's top-left corner
int16_t oldTextX, oldTextY;        // Previous position of the text
int xSpeed = 6;                    // Horizontal speed in pixels per frame
int ySpeed = 4;                    // Vertical speed in pixels per frame
const char* text = "BOBEK";
int16_t x1, y1;                    // Store text boundary offsets from getTextBounds

void setup() {
  // --- Initialize Display ---
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);
  tft.initR(INITR_MINI160x80);
  tft.setRotation(3);       // Landscape
  tft.invertDisplay(true);
  tft.fillScreen(ST77XX_BLACK);

  // --- Get Text Dimensions for Animation ---
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  // Get the bounding box of the text to know its width and height.
  tft.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
  boxWidth = textWidth + (2 * PADDING);
  boxHeight = textHeight + (2 * PADDING);

  // --- Initialize Animation Position ---
  textX = 0;
  textY = (SCREEN_HEIGHT - boxHeight) / 2; // Center vertically
  oldTextX = textX;
  oldTextY = textY;
}

void loop() {
  // Wait a short amount of time to control the frame rate
  delay(10);

  // --- Update Animation ---
  oldTextX = textX; // Store the current position before updating
  oldTextY = textY;

  // Move the text
  textX += xSpeed;
  textY += ySpeed;

  // Check for collision with horizontal screen edges and reverse direction
  if ((textX + boxWidth) >= SCREEN_WIDTH) {
    textX = SCREEN_WIDTH - boxWidth; // Clamp to edge
    xSpeed = -xSpeed; // Reverse direction
  } else if (textX < 1) {
    textX = 1; // Clamp to edge
    xSpeed = -xSpeed; // Reverse direction
  }

  // Check for collision with vertical screen edges and reverse direction
  if ((textY + boxHeight) >= SCREEN_HEIGHT) {
    textY = SCREEN_HEIGHT - boxHeight; // Clamp to edge
    ySpeed = -ySpeed; // Reverse direction
  } else if (textY < 2) {
    textY = 2; // Clamp to edge
    ySpeed = -ySpeed; // Reverse direction
  }

  // --- Render Frame (Diagnostic Mode: Direct to screen) ---

  // 1. Erase the old text's bounding box
  // This is less efficient than the canvas method and will cause flicker.
  if (textX != oldTextX || textY != oldTextY) {
      tft.fillRect(oldTextX, oldTextY, boxWidth, boxHeight, ST77XX_BLACK);
  }

  // 2. Draw the text directly to the screen
  // This bypasses the canvas to see if basic rendering works.
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);

  // Draw the box around the text
  tft.drawRect(textX, textY, boxWidth, boxHeight, ST77XX_CYAN);

  // Set the cursor based on where the top-left corner should be.
  // (textX, textY) is the top-left, but setCursor wants the baseline start.
  // The (x1,y1) values from getTextBounds tell us the offset.
  tft.setCursor(textX + PADDING - x1, textY + PADDING - y1);
  tft.print(text);
}