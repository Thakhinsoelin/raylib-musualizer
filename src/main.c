#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#if defined(PLATFORM_WEB)
//    #include <emscripten/emscripten.h>
//#endif

void callback(void* bufferData, unsigned int frames) {
    int16_t *frames = bufferData;
    (void)bufferData;
    //printf("callback(frames = %u)\n", frames);
}
//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void sample(void) {

};

//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib game template");
    SetTargetFPS(60);
    SetExitKey(KEY_F3);

    InitAudioDevice();
    Music music = LoadMusicStream("ai_kotoba4.mp3");
    printf("Music framecounts = %u\n", music.frameCount);
    printf("Music stream sample rate = %u\zn", music.stream.sampleRate);
    printf("Music stream sample size = %u\n", music.stream.sampleSize);
    printf("Music stream sample channels = %u\n", music.stream.channels);
    music.looping = false;
    PlayMusicStream(music);
    SetMusicVolume(music, 0.5f);
    AttachAudioStreamProcessor(music.stream, callback);
//#if defined(PLATFORM_WEB)
//    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
//#else
    
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_SPACE)) {
            if (IsMusicStreamPlaying(music))
            {
                PauseMusicStream(music);
            }
            else {
                ResumeMusicStream(music);
            }
        }
        UpdateMusicStream(music);
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }
//#endif
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
