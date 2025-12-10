#include "raylib.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #if defined(PLATFORM_WEB)
//     #include <emscripten/emscripten.h>
// #endif

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])
#define DOUBLE_SAMPLE int32_t
#define SINGLE_SAMPLE int16_t
#define SINGLE_SAMPLE_MAX INT16_MAX
#define SINGLE_SAMPLE_MIN INT16_MIN

typedef struct {
    float left;
    float right;
}Frame;

static const int screenWidth = 800;
static const int screenHeight = 450;
Frame global_frames[4800*2] = {0};
size_t global_frames_count = 0;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void callback(void *bufferData, unsigned int frames) {
  size_t capacity = ARRAY_LEN(global_frames);
  if (frames <= capacity - global_frames_count) {
    memcpy(global_frames + global_frames_count, bufferData, frames * sizeof(Frame));
    global_frames_count += frames;
  } else if (frames <= capacity) {
    memmove(global_frames, global_frames + frames,
            sizeof(Frame) * (capacity - frames));
    memcpy(global_frames + (capacity - frames), bufferData,
           sizeof(Frame) * frames);
  } else {
    memcpy(global_frames, bufferData, sizeof(Frame) * capacity);
    global_frames_count = capacity;
  } 
  // printf("callback(frames = %u)\n", frames);
}

//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //---------------------------------------------------------
  InitWindow(screenWidth, screenHeight, "raylib game template");
  SetTargetFPS(60);

  InitAudioDevice();
  Music music = LoadMusicStream("ai_kotoba4.mp3");

  assert(music.stream.sampleSize == 32);
  assert(music.stream.channels == 2);
  printf("Music framecounts = %u\n", music.frameCount);
  printf("Music stream sample rate = %u\n", music.stream.sampleRate);
  printf("Music stream sample size = %u\n", music.stream.sampleSize);
  printf("Music stream sample channels = %u\n", music.stream.channels);
  music.looping = false;
  PlayMusicStream(music);
  SetMusicVolume(music, 0.5f);
  AttachAudioStreamProcessor(music.stream, callback);
  // #if defined(PLATFORM_WEB)
  //     emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
  // #else

  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or {undefined} key
  {
    if (IsKeyPressed(KEY_SPACE)) {
      if (IsMusicStreamPlaying(music)) {
        PauseMusicStream(music);
      } else {
        ResumeMusicStream(music);
      }
    }
    UpdateMusicStream(music);
    BeginDrawing();

    ClearBackground(RAYWHITE);
    float cell_width = (float)screenWidth / global_frames_count;
    for (size_t i = 0; i < global_frames_count; i++) {
        float t = global_frames[i].left;
      if (t > 0) {
        /*float t = (float)sample / SINGLE_SAMPLE_MAX;*/
        DrawRectangle(i * cell_width, screenHeight / 2 - screenHeight / 2 * t,
                      1, screenHeight / 2 * t, RED);
      } else {
        /*float t = (float)sample / SINGLE_SAMPLE_MIN; */
        DrawRectangle(i * cell_width, screenHeight/2, 1, screenHeight / 2 * t, RED);
      }
    }
    // DrawText("Congrats! You created your first window!", 190, 200, 20,
    // LIGHTGRAY); if(global_frames_count > 0) exit(1);
    EndDrawing();
  }
  // #endif
  CloseWindow(); // Close window and OpenGL context
  // system("pause");
  //--------------------------------------------------------------------------------------

  return 0;
}
