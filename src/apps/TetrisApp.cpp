#include "TetrisApp.h"
#include "MainMenuApp.h"
#include "../core/AppManager.h"
#include <Colors.h>

const uint16_t PIECE_COLOR = HEX2RGB565(0x336647);
const uint16_t BOARD_COLOR = HEX2RGB565(0x87C16D);
const uint16_t GRID_COLOR = HEX2RGB565(0x041819);

// ======= PUBLIC METHODS =======

TetrisApp::TetrisApp(Adafruit_ST7735 *display, AppManager *appManager)
    : display(display), appManager(appManager) {}

void TetrisApp::begin()
{
    display->fillScreen(BOARD_COLOR);

    // Splash screen
    display->setCursor(6, 15);
    display->setTextColor(PIECE_COLOR);
    display->setTextSize(2);
    display->println("Tetris");

    display->setCursor(2, 50);
    display->setTextSize(1);
    display->println("Press 4 to go to menu");

    // Initialize the board with empty values
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            board[x][y] = 0;
        }
    }

    // Initialize the next piece and the first piece
    int nextPieceIndex = random(7);
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            nextPiece[px][py] = pieces[nextPieceIndex][px][py];
        }
    }

    // Create the first piece
    newPiece(); // This will set the first piece to currentPiece and randomize the next one

    delay(1000); // Delay for splash screen
}

void TetrisApp::update()
{
    static unsigned long lastDropTime = 0;
    unsigned long currentTime = millis(); // Get the current time

    display->fillScreen(BOARD_COLOR);

    // Draw the grid and the placed pieces
    drawGrid();

    // Check if it's time to drop the piece
    if (currentTime - lastDropTime > 500) // 500ms interval for piece drop
    {
        if (!checkCollision(currentX, currentY + 1, currentRotation))
        {
            currentY++; // Move piece down
        }
        else
        {
            lockPiece(); // Lock the piece if it can't move down
        }
        lastDropTime = currentTime; // Reset the drop timer
    }

    // Draw current piece
    drawPiece(currentX, currentY, currentRotation, PIECE_COLOR);

    // Draw next piece on the right side
    drawNextPiece();
}

void TetrisApp::drawGrid()
{
    // Draw the grid lines
    for (int x = 0; x <= BOARD_WIDTH; x++)
    {
        display->drawLine(x * BLOCK_SIZE, 0, x * BLOCK_SIZE, BOARD_HEIGHT * BLOCK_SIZE, GRID_COLOR);
    }
    for (int y = 0; y <= BOARD_HEIGHT; y++)
    {
        display->drawLine(0, y * BLOCK_SIZE, BOARD_WIDTH * BLOCK_SIZE, y * BLOCK_SIZE, GRID_COLOR);
    }

    // Draw the locked pieces on the board
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            if (board[x][y] != 0)
            {
                drawBlock(x, y, PIECE_COLOR); // Use the same color for locked pieces
            }
        }
    }
}

void TetrisApp::handleButtonPress(int buttonID)
{
    if (buttonID == 3)
    {
        appManager->setApp(new MainMenuApp(display, appManager)); // Switch to main menu
    }
    else if (buttonID == 0) // Left
    {
        left();
    }
    else if (buttonID == 1) // Right
    {
        right();
    }
    else if (buttonID == 2) // Rotate
    {
        rotate();
    }
}

// ======= PRIVATE METHODS =======

// Define the 7 Tetris pieces (I, J, L, O, S, T, Z)
const int TetrisApp::pieces[7][4][4] = {
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I piece
    {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // J piece
    {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // L piece
    {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // O piece
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // S piece
    {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // T piece
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // Z piece
};

// Wall kick data for SRS
const int wallKickData[4][5][2] = {
    // 0 -> 1
    {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}},
    // 1 -> 2
    {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}},
    // 2 -> 3
    {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}},
    // 3 -> 0
    {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}},
};

void TetrisApp::drawBlock(int x, int y, uint16_t color)
{
    display->fillRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, color);
}

void TetrisApp::drawPiece(int x, int y, int rotation, uint16_t color)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            if (currentPiece[px][py])
            {
                int rotatedX = px;
                int rotatedY = py;

                // Apply rotation
                switch (rotation)
                {
                case 1:
                    rotatedX = py;
                    rotatedY = 3 - px;
                    break;
                case 2:
                    rotatedX = 3 - px;
                    rotatedY = 3 - py;
                    break;
                case 3:
                    rotatedX = 3 - py;
                    rotatedY = px;
                    break;
                }
                drawBlock(x + rotatedX, y + rotatedY, color);
            }
        }
    }
}

void TetrisApp::clearPiece(int x, int y, int rotation)
{
    drawPiece(x, y, rotation, ST77XX_BLACK);
}

bool TetrisApp::checkCollision(int x, int y, int rotation)
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            if (currentPiece[px][py])
            {
                int rotatedX = px;
                int rotatedY = py;

                // Apply rotation to the piece
                switch (rotation)
                {
                case 1:
                    rotatedX = py;
                    rotatedY = 3 - px;
                    break;
                case 2:
                    rotatedX = 3 - px;
                    rotatedY = 3 - py;
                    break;
                case 3:
                    rotatedX = 3 - py;
                    rotatedY = px;
                    break;
                }

                int boardX = x + rotatedX;
                int boardY = y + rotatedY;

                // Check if out of bounds or collides with an existing block
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT || (boardY >= 0 && board[boardX][boardY]))
                {
                    return true; // Collision detected
                }
            }
        }
    }
    return false; // No collision
}

void TetrisApp::lockPiece()
{
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            if (currentPiece[px][py])
            {
                int rotatedX = px;
                int rotatedY = py;

                // Apply the same rotation logic used in drawPiece
                switch (currentRotation)
                {
                case 1:
                    rotatedX = py;
                    rotatedY = 3 - px;
                    break;
                case 2:
                    rotatedX = 3 - px;
                    rotatedY = 3 - py;
                    break;
                case 3:
                    rotatedX = 3 - py;
                    rotatedY = px;
                    break;
                }

                int boardX = currentX + rotatedX;
                int boardY = currentY + rotatedY;

                if (boardX >= 0 && boardX < BOARD_WIDTH && boardY >= 0 && boardY < BOARD_HEIGHT)
                {
                    board[boardX][boardY] = 1; // Lock the rotated piece into the board
                }
            }
        }
    }

    clearFullLines();
    newPiece();
}

void TetrisApp::clearFullLines()
{
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--)
    {
        bool lineFull = true;
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (board[x][y] == 0)
            {
                lineFull = false;
                break;
            }
        }

        if (lineFull)
        {
            // Shift lines down
            for (int y2 = y; y2 > 0; y2--)
            {
                for (int x = 0; x < BOARD_WIDTH; x++)
                {
                    board[x][y2] = board[x][y2 - 1];
                }
            }
            // Clear the top row
            for (int x = 0; x < BOARD_WIDTH; x++)
            {
                board[x][0] = 0;
            }
            y++; // Check this row again after shifting
        }
    }
}

void TetrisApp::newPiece()
{
    // Set current piece to next piece
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            currentPiece[px][py] = nextPiece[px][py];
        }
    }

    // Generate the next random piece
    int nextPieceIndex = random(7);
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            nextPiece[px][py] = pieces[nextPieceIndex][px][py];
        }
    }

    // Check for game over
    currentX = BOARD_WIDTH / 2 - 2;
    currentY = 0;
    currentRotation = 0;

    if (checkCollision(currentX, currentY, currentRotation))
    {
        // Game over, return to main menu
        appManager->setApp(new MainMenuApp(display, appManager));
    }
}

void TetrisApp::drawNextPiece()
{
    int startX = BOARD_WIDTH * BLOCK_SIZE + 10; // Position to start drawing the next piece
    int startY = 10;                            // Slight padding from the top

    // Draw the label "Next"
    display->setCursor(startX, startY - 10);
    display->setTextColor(PIECE_COLOR);
    display->setTextSize(1);
    display->println("Next:");

    // Draw the next piece in a 4x4 grid
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            if (nextPiece[px][py])
            {
                int blockX = startX + px * BLOCK_SIZE;
                int blockY = startY + py * BLOCK_SIZE;
                drawBlock(blockX / BLOCK_SIZE, blockY / BLOCK_SIZE, PIECE_COLOR);
            }
        }
    }
}

void TetrisApp::left()
{
    if (!checkCollision(currentX - 1, currentY, currentRotation))
    {
        currentX--;
    }
}

void TetrisApp::right()
{
    if (!checkCollision(currentX + 1, currentY, currentRotation))
    {
        currentX++;
    }
}

void TetrisApp::rotate()
{
    // Save the current state
    int prevRotation = currentRotation;
    int offsetX = 0;
    int offsetY = 0;

    // Try new rotation
    currentRotation = (currentRotation + 1) % 4;

    // Check collision with new rotation
    if (checkCollision(currentX, currentY, currentRotation))
    {
        // If collision, revert to the previous rotation
        currentRotation = prevRotation;
        return;
    }

    // Center the piece if possible
    while (checkCollision(currentX + offsetX, currentY + offsetY, currentRotation))
    {
        // Move the piece left or right
        if (offsetX == 0)
        {
            offsetX = (currentRotation % 2 == 0) ? -1 : 1; // Move left for even rotations, right for odd
        }
        else
        {
            offsetX = 0;
            offsetY = 1; // Move down if no more horizontal movement is possible
        }
    }

    // Apply the offset to adjust position
    currentX += offsetX;
    currentY += offsetY;
}

void TetrisApp::drop()
{
    while (!checkCollision(currentX, currentY + 1, currentRotation))
    {
        currentY++;
    }
    lockPiece();
}

// ======= END OF FILE =======