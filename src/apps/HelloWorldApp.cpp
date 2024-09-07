#include "HelloWorldApp.h"
#include "MainMenuApp.h"
#include "../core/AppManager.h"

HelloWorldApp::HelloWorldApp(Adafruit_ST7735 *display, AppManager *appManager)
    : display(display), appManager(appManager) {}

void HelloWorldApp::begin()
{
    // display->fillScreen(ST77XX_BLACK);
    // display->setCursor(0, 0);
    // display->setTextColor(ST77XX_WHITE);
    // display->setTextSize(1);
    // display->println("Hello, World!");
}

void HelloWorldApp::update()
{
    display->fillScreen(ST77XX_BLACK);
    display->setCursor(0, 10);
    display->setTextColor(ST77XX_WHITE);
    display->setTextSize(1);
    display->print("Counter: ");
    display->println(counter);
}

void HelloWorldApp::handleButtonPress(int buttonID)
{
    if (buttonID == 1)
    {
        counter++;
    }
    else if (buttonID == 2)
    {
        counter--;
    }
    else if (buttonID == 3)
    {
        appManager->setApp(new MainMenuApp(display, appManager)); // Switch to the main menu
    }
}
