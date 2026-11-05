#include <raylib.h>
#include "ball.h"
//#include "map.h"
//#include "character.h"
#include "manager.h"
constexpr int screenWidth = 1000;
constexpr int screenHeight = 1000;

int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    
    InitWindow(screenWidth, screenHeight, "Assignment 4 Aaron Kallio");
    //InitObjectTextures();
    //SetTargetFPS(120);
    Ball ball;
    FillMap();
    Place();
    SetTask();
    MakeBuildZone();
    FillList();
    AssignSoldiers();
    int gameSpeed = 100;
    int frame = 1;

    //mapTexture = LoadRenderTexture(screenWidth, screenHeight);
    LoadTextureRender();
    while (!WindowShouldClose())
    {
        //ball.Update();
        
        
        //ball.Draw();

        if (IsKeyPressed(KEY_ONE)) {
            gameSpeed = 1;
        }

        if (IsKeyPressed(KEY_TWO)) {
            gameSpeed = 10;
        }

        if (IsKeyPressed(KEY_THREE)) {
            gameSpeed = 100;
        }





        for(int i = 0; i < gameSpeed; i++){
            //RunMap();
            //RunCharacters();

            if (frame==60) {
                frame = 1;
            }

            ProduceItems();
            RunTask();
            if (frame ==30) {
                ReturnToWork();
                SoldierSlots();
            }
            //if (GetFrameTime()) {
            //cout << GetFrameTime();
            //}
            frame++;
            
        }
        BeginDrawing();
        ClearBackground(BLACK);

        //RunMap();
        DrawMapOptimized();
        DrawBuildings();
        DrawCharacters();
        DrawItemsCollected();
        
        //RunMap();
        //DrawCharacters();
        //RunTask();
        UpdateMapTexture();
        int fps = GetFPS();
        DrawText(TextFormat("FPS: %d", fps), 10, 10, 20, PURPLE);
        EndDrawing();
        
    }
    
    CloseWindow();
}