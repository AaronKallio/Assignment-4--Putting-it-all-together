#include <raylib.h>
#include "ball.h"
#include "map.h"

int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    constexpr int screenWidth = 1000;
    constexpr int screenHeight = 1000;
    
    Ball ball;
    FillMap();
    InitWindow(screenWidth, screenHeight, "Assignment 4 Aaron Kallio");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        ball.Update();
        
        BeginDrawing();
        ClearBackground(BLACK);
        ball.Draw();
        DrawRectangle(0,0,10,10,GREEN);
        RunMap();
        EndDrawing();
        
    }
    
    CloseWindow();
}