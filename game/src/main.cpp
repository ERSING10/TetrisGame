#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>

double lastUpdateTime = 0;
int previousScore = 0; // Önceki skor değeri last score

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main()
{
    Color darkPurple = {112, 31, 126, 255};
    InitWindow(1500, 800, "raylib Tetris");
    SetTargetFPS(60);
    
    Font font = LoadFontEx("Font/monogram.ttf", 80, 0, 0);
   
    Game game = Game();
    
    // Arka plan 
    Texture2D background = LoadTexture("resources/icardi.png");

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);

        game.HandleInput();
        if (EventTriggered(0.5))
        {
            game.MoveBlockDown();
        }
       
        BeginDrawing();
        
        // Arka plan 
        DrawTexture(background, 850, 0, WHITE);

        DrawTextEx(font, " SCORE", {900, 20}, 75, 2, WHITE);
        DrawTextEx(font, " NEXT", {900, 300}, 75, 2, WHITE);
        if (game.gameOver)
        {
            DrawTextEx(font, "GAME OVER!", 
            
            {900, 700}, 75, 2, WHITE);
            previousScore=game.score;
        }
        DrawTextEx(font, " ERSING", {1320, 735}, 50, 2, WHITE);
        DrawRectangleRounded({900, 100, 300, 200}, 0.3, 6, BLANK);

        // Önceki skoru 
        
        DrawTextEx(font, " LAST SCORE:", {1150, 80}, 50, 2, WHITE);
        DrawTextEx(font, std::to_string(previousScore).c_str(), {1300, 130}, 50, 2, WHITE);

        char scoreText[10];
        sprintf(scoreText, "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 75, 2);
        

        DrawTextEx(font, scoreText, {900 + (300 - textSize.x) / 2, 90}, 75, 2, WHITE);
        DrawRectangleRounded({900, 375, 400, 300}, 0.3, 6, BLANK);
        
        game.Draw();
        EndDrawing();
    }

    // Bellekten arka planı temizledim
    UnloadTexture(background);

    CloseWindow();

    
}
