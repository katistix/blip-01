#ifndef MAINMENUAPP_H
#define MAINMENUAPP_H

#include "../core/App.h"
#include <Adafruit_ST7735.h>

class AppManager; // Forward declaration to avoid circular dependency

// Struct for menu options
struct MenuOption
{
    String label;
    App *(*appFactory)(Adafruit_ST7735 *, AppManager *); // Factory function to create app instance
};

class MainMenuApp : public App
{
public:
    MainMenuApp(Adafruit_ST7735 *display, AppManager *appManager);

    void begin() override;
    void update() override;
    void handleButtonPress(int buttonID) override;

private:
    Adafruit_ST7735 *display;
    AppManager *appManager;
    int selectedOption;

    // List of menu options
    static const MenuOption menuOptions[];

    void drawMenu(); // Helper method to draw the menu on the display
};

// Factory functions
App *createHelloWorldApp(Adafruit_ST7735 *display, AppManager *appManager);
App *createTetrisApp(Adafruit_ST7735 *display, AppManager *appManager);

#endif // MAINMENUAPP_H
