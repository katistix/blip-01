#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include "Colors.h"
#include "Fonts/FreeSans12pt7b.h"
#include <WiFi.h>
#include "CustomFonts/JetBrainsMono_ExtraBold16pt7b.h"
#include "core/AppManager.h"
#include "apps/HelloWorldApp.h"
#include "core/EventManager.h"
#include "apps/MainMenuApp.h"

// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_CS 17
#define TFT_RST 21 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 20
#define TFT_BACKLIGHT 22

#define SCREEN_WIDTH 128  // ST7735 width is 128 pixels
#define SCREEN_HEIGHT 160 // ST7735 height is 160 pixels

// For 1.44" and 1.8" TFT with ST7735 use:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Define the button pins
int button1Pin = 6;
int button2Pin = 7;
int button3Pin = 8;
int button4Pin = 9;

float p = 3.1415926;

EventManager eventManager;

// Create an instance of the AppManager
AppManager appManager(&tft);

// Initialize app instances
// HelloWorldApp helloWorldApp(&tft, &appManager);
MainMenuApp mainMenuApp(&tft, &appManager);

// Button interrupt handlers
void handleButtonPress(int buttonID)
{
  uint32_t currentTime = millis();
  eventManager.addEvent(Event(EventType::BUTTON_PRESS, buttonID, currentTime));
}

void setup()
{
  // Set the button pins as INPUT_PULLUP (since buttons are connected to GND)
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // Attach interrupt service routines
  attachInterrupt(
      digitalPinToInterrupt(button1Pin), []
      { handleButtonPress(0); },
      FALLING);
  attachInterrupt(
      digitalPinToInterrupt(button2Pin), []
      { handleButtonPress(1); },
      FALLING);
  attachInterrupt(
      digitalPinToInterrupt(button3Pin), []
      { handleButtonPress(2); },
      FALLING);
  attachInterrupt(
      digitalPinToInterrupt(button4Pin), []
      { handleButtonPress(3); },
      FALLING);

  Serial.begin(9600);

  // Backlight control
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH);

  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB); // Init ST7735S chip, black tab
  tft.setRotation(2);        // Portrait mode

  tft.fillScreen(ST7735_WHITE);
  delay(500);

  appManager.setApp(&mainMenuApp); // Set the MainMenuApp as the current app
}

void loop()
{
  // Process events
  Event event(EventType::BUTTON_PRESS, -1, 0);
  while (eventManager.getNextEvent(event))
  {
    appManager.handleButtonPress(event.buttonID);
  }

  // Update the current app
  appManager.update();
  delay(100);
}
