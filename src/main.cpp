#include <raylib.h>
#include "ball.h"
#include "map.h"

int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    constexpr int screenWidth = 1000;
    constexpr int screenHeight = 1000;
    
    Ball ball;
    
    InitWindow(screenWidth, screenHeight, "Assignment 4 Aaron Kallio");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        ball.Update();
        
        BeginDrawing();
        ClearBackground(BLACK);
        ball.Draw();
        EndDrawing();
        DrawRectangle(0,0,10,10,GREEN);
    }
    
    CloseWindow();
}