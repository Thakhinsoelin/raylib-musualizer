
#include "raylib.h"
#include <assert.h>
#include <complex>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "double_plug.h"
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


static const int screenWidth = 800;
static const int screenHeight = 450;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

char *shit_args(int *argc, char ***argv) {
  assert(*argc > 0);
  char *result = (**argv);
  (*argv) += 1;
  (*argc) -= 1;
  return result;
}



//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main(int argc, char **argv) {
  // Initialization
  //---------------------------------------------------------
    
  call();
  
  // const char *program = shit_args(&argc, &argv);
  // if (argc == 0) {
  //   fprintf(stderr, "Usage: %s <input>\n", program);
  //   fprintf(stderr, "ERROR: no input file is provided\n");
  //   return 1;
  // }

  // const char *file_path = shit_args(&argc, &argv);

  SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
  InitWindow(screenWidth, screenHeight, "raylib game template");
  SetTargetFPS(60);

  InitAudioDevice();
  
  if (init() == 1) {
      //return 1;
  }


  
  // #if defined(PLATFORM_WEB)
  //     emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
  // #else

  //--------------------------------------------------------------------------------------
  /*Music music = LoadMusicStream(file_path);
  PlayMusicStream(music);*/
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or {undefined} key
  {
    #ifdef HOTRELOAD
      if (IsKeyPressed(KEY_R)) {
          reloadDLL();
      }
    #endif
      update();
  }
  // #endif
  CloseWindow(); // Close window and OpenGL context
  // system("pause");
  //--------------------------------------------------------------------------------------

  return 0;
}