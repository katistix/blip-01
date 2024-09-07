#include "MainMenuApp.h"
#include "HelloWorldApp.h"
#include "../core/AppManager.h"
#include "TetrisApp.h"

// Define menu options
const MenuOption MainMenuApp::menuOptions[] = {
    {"Hello World 1", createHelloWorldApp},
    {"Tetris", createTetrisApp}};

MainMenuApp::MainMenuApp(Adafruit_ST7735 *display, AppManager *appManager)
    : display(display), appManager(appManager), selectedOption(0) {}

void MainMenuApp::begin()
{
    drawMenu(); // Draw the initial menu
}

void MainMenuApp::update()
{
    // Update the menu display if needed
}

void MainMenuApp::handleButtonPress(int buttonID)
{
    if (buttonID == 0) // Button 0, decrement selectedOption
    {
        selectedOption--;
        if (selectedOption < 0)
        {
            selectedOption = sizeof(menuOptions) / sizeof(menuOptions[0]) - 1; // Wrap around
        }
    }
    else if (buttonID == 1) // Button 1, increment selectedOption
    {
        selectedOption++;
        if (selectedOption >= sizeof(menuOptions) / sizeof(menuOptions[0]))
        {
            selectedOption = 0; // Wrap around
        }
    }
    else if (buttonID == 2) // Button 2, select the current option
    {
        if (selectedOption >= 0 && selectedOption < sizeof(menuOptions) / sizeof(menuOptions[0]))
        {
            MenuOption selectedMenuOption = menuOptions[selectedOption];
            App *selectedApp = selectedMenuOption.appFactory(display, appManager); // Create app instance
            appManager->setApp(selectedApp);                                       // Switch to the selected app
        }
    }

    drawMenu(); // Redraw the menu to show the current selection
}

void MainMenuApp::drawMenu()
{
    display->fillScreen(ST77XX_BLACK);
    display->setCursor(0, 0);
    display->setTextColor(ST77XX_WHITE);
    display->setTextSize(1);
    display->println("Main Menu");

    // Print menu options with an indicator for the selected option
    for (int i = 0; i < sizeof(menuOptions) / sizeof(menuOptions[0]); i++)
    {
        if (i == selectedOption)
        {
            display->print("> "); // Indicator for the selected option
        }
        else
        {
            display->print("  ");
        }
        display->print(i + 1);
        display->print(". ");
        display->println(menuOptions[i].label);
    }

    display->println();
    display->println("Press 3 to select.");
}

// Factory function to create HelloWorldApp instances
App *createHelloWorldApp(Adafruit_ST7735 *display, AppManager *appManager)
{
    return new HelloWorldApp(display, appManager);
}

// Factory function to create TetrisApp instances
App *createTetrisApp(Adafruit_ST7735 *display, AppManager *appManager)
{
    return new TetrisApp(display, appManager);
}