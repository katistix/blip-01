#ifndef APPMANAGER_H
#define APPMANAGER_H

#include "App.h"
#include <Adafruit_ST7735.h>

class AppManager
{
public:
    AppManager(Adafruit_ST7735 *display);

    void setApp(App *newApp); // Switch to a new app
    App *getCurrentApp();     // Get the currently active app

    void update();                        // Called continuously to update the current app
    void handleButtonPress(int buttonID); // Forward button events to the current app

private:
    App *currentApp;
    Adafruit_ST7735 *display;
};
#endif // APPMANAGER_H
