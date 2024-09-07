#ifndef TETRISAPP_H
#define TETRISAPP_H

#include "../core/App.h"
#include <Adafruit_ST7735.h>

class AppManager; // Forward declaration to avoid circular dependency

class TetrisApp : public App
{
public:
    TetrisApp(Adafruit_ST7735 *display, AppManager *appManager);

    void begin() override;
    void update() override;
    void drawGrid();
    void handleButtonPress(int buttonID) override;

private:
    Adafruit_ST7735 *display;
    AppManager *appManager;

    static const int BOARD_WIDTH = 10;
    static const int BOARD_HEIGHT = 20;
    static const int BLOCK_SIZE = 8;

    int board[BOARD_WIDTH][BOARD_HEIGHT];
    int currentX, currentY, currentRotation;
    int currentPiece[4][4]; // Example for a 4x4 piece matrix

    // Piece definitions
    static const int pieces[7][4][4]; // 7 types of pieces, each 4x4 matrix
    void drawBlock(int x, int y, uint16_t color);
    void drawPiece(int x, int y, int rotation, uint16_t color);
    void clearPiece(int x, int y, int rotation);
    bool checkCollision(int x, int y, int rotation);
    void lockPiece();
    void clearFullLines();
    void newPiece(); // Generate a new piece

    void left();
    void right();
    void rotate();
    void drop();

    int nextPiece[4][4];  // Stores the next piece
    void drawNextPiece(); // Draw the next piece on the right side of the screen
};

#endif // TETRISAPP_H