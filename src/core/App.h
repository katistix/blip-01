#ifndef APP_H
#define APP_H

class App
{
public:
    virtual ~App() = default;                         // Ensure proper cleanup of derived classes
    virtual void begin() = 0;                         // Start or reset the app
    virtual void update() = 0;                        // Called continuously to update the app
    virtual void handleButtonPress(int buttonID) = 0; // Handle input events
};

#endif // APP_H
