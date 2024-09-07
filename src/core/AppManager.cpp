#include "AppManager.h"

AppManager::AppManager(Adafruit_ST7735 *display) : currentApp(nullptr), display(display) {}

void AppManager::setApp(App *newApp)
{
    if (currentApp != nullptr)
    {
        // Optionally, cleanup or stop the current app
        display->fillScreen(ST77XX_BLACK);
        // currentApp->end(); // Reinitialize the new app
    }
    currentApp = newApp;
    currentApp->begin(); // Initialize the new app
}

App *AppManager::getCurrentApp()
{
    return currentApp;
}

void AppManager::update()
{
    if (currentApp != nullptr)
    {
        currentApp->update();
    }
}

void AppManager::handleButtonPress(int buttonID)
{
    if (currentApp != nullptr)
    {
        currentApp->handleButtonPress(buttonID);
    }
}