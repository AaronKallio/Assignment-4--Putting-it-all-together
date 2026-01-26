#include <raylib.h>
#include "ball.h"
//#include "map.h"
//#include "character.h"
#include "manager.h"


int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    constexpr int screenWidth = 1000;
    constexpr int screenHeight = 1000;
    InitWindow(screenWidth, screenHeight, "Assignment 4 Aaron Kallio");
    //InitObjectTextures();
    //SetTargetFPS(120);
    Ball ball;
    FillMap();
    Place();
    SetTask();
    int gameSpeed = 20;
    int frame = 1;
    
    while (!WindowShouldClose())
    {
        //ball.Update();
        
        
        //ball.Draw();
        for(int i = 0; i < gameSpeed; i++){
            //RunMap();
            //RunCharacters();
            if (frame==60) {
                frame = 1;
            }

            
            RunTask();
            if (frame ==30) {
                ReturnToWork();
            }
            //if (GetFrameTime()) {
            //cout << GetFrameTime();
            //}
            frame++;
            
        }
        BeginDrawing();
        ClearBackground(BLACK);
        RunMap();
        DrawCharacters();
        DrawItemsCollected();
        //RunMap();
        //DrawCharacters();
        //RunTask();
        int fps = GetFPS();
        DrawText(TextFormat("FPS: %d", fps), 10, 10, 20, PURPLE);
        EndDrawing();
        
    }
    
    CloseWindow();
}