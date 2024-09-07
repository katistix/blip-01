#ifndef HELLOWORLDAPP_H
#define HELLOWORLDAPP_H

#include "../core/App.h"
#include <Adafruit_ST7735.h>

class AppManager; // Forward declaration to avoid circular dependency

class HelloWorldApp : public App
{
public:
    HelloWorldApp(Adafruit_ST7735 *display, AppManager *appManager);

    void begin() override;
    void update() override;
    void handleButtonPress(int buttonID) override;

private:
    Adafruit_ST7735 *display;
    AppManager *appManager;

    int counter = 0;
};
#endif // HELLOWORLDAPP_H
