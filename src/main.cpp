// Simple Reaction Game, WIP

// Circle Reaction Game for Arduino with ST7735 Display
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Fonts/FreeSansBold9pt7b.h>

// --- Display and Pin Configuration ---
const int TFT_CS = 1;
const int TFT_DC = 0;
const int TFT_MOSI = 2;
const int TFT_SCLK = 3;
const int TFT_RST = 11;
const int TFT_BACKLIGHT = 4;
const int BUTTON_PIN = 6;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// --- Game States ---
enum GameState {
  STATE_START,
  STATE_GROWING,
  STATE_JUDGE,
  STATE_RESULT
};

// --- Game Colors ---
#define COLOR_BACKGROUND   ST77XX_BLACK
#define COLOR_TARGET_CIRCLE ST77XX_BLUE
#define COLOR_GROW_CIRCLE  ST77XX_WHITE
#define COLOR_TEXT         ST77XX_MAGENTA
#define COLOR_PERFECT      ST77XX_GREEN
#define COLOR_GOOD         ST77XX_YELLOW
#define COLOR_MISS         ST77XX_RED

// --- Game Settings ---
#define SCREEN_CENTER_X    (tft.width() / 2)
#define SCREEN_CENTER_Y    (tft.height() / 2)
#define TARGET_RADIUS      50
#define START_RADIUS       5
#define PERFECT_THRESHOLD  2  // Within 2px for a "Perfect"
#define GOOD_THRESHOLD     6  // Within 6px for a "Good"
#define MISS_THRESHOLD     10 // If player goes past the circle by this much it's a miss
#define RESULT_DELAY       1000 // Show result for 1 second

// --- Game Variables ---
GameState gameState;
int score = 0;
float currentRadius = START_RADIUS;
float lastRadius = START_RADIUS; // To erase the previous circle
float growthRate = 1.0;
unsigned long resultTimer = 0;
String resultMessage = "";
uint16_t resultColor = COLOR_MISS;

// Button handling
bool button_state = false;
bool last_button_state = false;

// --- Function Prototypes ---
void handleInput();
void startNewRound();
void drawStartScreen();
void drawCenteredString(const String &buf, int x, int y);
void drawUI();

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);

  randomSeed(analogRead(A0));

  tft.initR();
  tft.setRotation(3); // Landscape
  tft.invertDisplay(true);
  tft.setFont(&FreeSansBold9pt7b);

  // Draw the first screen and set the initial state
  drawStartScreen();
  gameState = STATE_START;
}

void loop() {
  handleInput();

  switch (gameState) {
    case STATE_START:
      // In this state, we just wait for input to start the game
      if (button_state && !last_button_state) {
        startNewRound();
        gameState = STATE_GROWING;
      }
      break;

    case STATE_GROWING: {
      // Erase the old circle from its last position
      tft.drawCircle(SCREEN_CENTER_X, SCREEN_CENTER_Y, (int)lastRadius, COLOR_BACKGROUND);
      
      // Update radius for the new frame
      currentRadius += growthRate;
      lastRadius = currentRadius;
      
      // Draw the new circle
      tft.drawCircle(SCREEN_CENTER_X, SCREEN_CENTER_Y, (int)currentRadius, COLOR_GROW_CIRCLE);

      // Check for player input to judge their timing
      if (button_state && !last_button_state) {
        gameState = STATE_JUDGE;
      }
      // Or, check if the circle grew too large (a miss)
      else if (currentRadius > TARGET_RADIUS + MISS_THRESHOLD) {
        resultMessage = "Miss!";
        resultColor = COLOR_MISS;
        score -= 5;
        
        // Go to show the result
        tft.fillScreen(COLOR_BACKGROUND);
        drawUI();
        tft.setTextColor(resultColor);
        drawCenteredString(resultMessage, SCREEN_CENTER_X, SCREEN_CENTER_Y);
        resultTimer = millis();
        gameState = STATE_RESULT;
      }
      break;
    }

    case STATE_JUDGE: {
      int diff = abs(TARGET_RADIUS - (int)currentRadius);
      if (diff <= PERFECT_THRESHOLD) {
        resultMessage = "Perfect!";
        resultColor = COLOR_PERFECT;
        score += 10;
      } else if (diff <= GOOD_THRESHOLD) {
        resultMessage = "Good";
        resultColor = COLOR_GOOD;
        score += 5;
      } else {
        resultMessage = "Miss!";
        resultColor = COLOR_MISS;
        score -= 2;
      }
      
      // Show the result screen
      tft.fillScreen(COLOR_BACKGROUND);
      drawUI();
      tft.setTextColor(resultColor);
      drawCenteredString(resultMessage, SCREEN_CENTER_X, SCREEN_CENTER_Y);
      resultTimer = millis();
      gameState = STATE_RESULT;
      break;
    }

    case STATE_RESULT:
      // Wait for the result display time to pass
      if (millis() - resultTimer > RESULT_DELAY) {
        // Transition to the start screen for the next round
        drawStartScreen();
        gameState = STATE_START;
      }
      break;
  }

}

void handleInput() {
  last_button_state = button_state;
  button_state = (digitalRead(BUTTON_PIN) == LOW);
}

void startNewRound() {
  currentRadius = START_RADIUS;
  lastRadius = START_RADIUS;
  // Randomize growth rate for the new round (1.6 to 4 pixels/frame)
  growthRate = random(160, 401) / 100.0;
  
  // Draw the UI for the new round
  tft.fillScreen(COLOR_BACKGROUND);
  drawUI();
}

void drawStartScreen(){
  tft.fillScreen(COLOR_BACKGROUND);
  drawUI();
  drawCenteredString("Press to Start", SCREEN_CENTER_X, SCREEN_CENTER_Y);
}

void drawUI() {
  // Draw the outer target circle
  tft.drawCircle(SCREEN_CENTER_X, SCREEN_CENTER_Y, TARGET_RADIUS, COLOR_TARGET_CIRCLE);
  // Draw the score using the default font for clarity
  tft.setFont();
  tft.setTextSize(2);
  tft.setTextColor(COLOR_TEXT);
  tft.setCursor(5, 28);
  tft.print(score);
}

// Helper function to draw centered text
void drawCenteredString(const String &buf, int x, int y) {
  // Always set the font and size for centered messages
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  
  int16_t x1, y1;
  uint16_t w, h;
  // Get the bounding box of the text
  tft.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
  
  // Calculate the cursor position to center the text
  // x1, y1 are the top-left offset of the text relative to the cursor
  int16_t cursor_x = x - (x1 + w / 2);
  int16_t cursor_y = y - (y1 + h / 2);

  tft.setCursor(cursor_x, cursor_y);
  tft.print(buf);
}
