
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

typedef struct {
  float left;
  float right;
} Frame;
#define N 256
float in[N] = {0};
std::complex<float> out[N] = {0};
float max_amp = 0;


static const int screenWidth = 800;
static const int screenHeight = 450;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void fft(float in[], size_t stride, std::complex<float> out[], size_t n) {
  assert(n > 0);
  if (n == 1) {
    out[0] = in[0];
    return;
  }
  fft(in, stride * 2, out, n / 2);
  fft(in + stride, stride * 2, out + n / 2, n / 2);
  for (size_t k = 0; k < n / 2; k++) {
    float t = (float)k / n;
    std::complex<float> v =
        exp(-2.0f * std::complex<float>(0, 1) * PI * t) * out[k + n / 2];
    std::complex<float> e = out[k];
    out[k] = e + v;
    out[k + n / 2] = e - v;
  }
}

float amp(std::complex<float> z) {
  float a = fabsf(std::real<float>(z));
  float b = fabsf(std::imag<float>(z));
  if (a < b)
    return b;
  return a;
}

void callback(void *bufferData, unsigned int frames) {
  if (frames < N) {
    return;
  }

  Frame *fframes = (Frame *)bufferData;

  for (size_t i = 0; i < frames; i++) {
    in[i] = fframes[i].left;
  }
  fft(in, 1, out, N);

  for (size_t i = 0; i < frames; i++) {
    float a = amp(out[i]);
    if (a > max_amp) {
      max_amp = a;
    }
  }
}

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
  
  const char *program = shit_args(&argc, &argv);
  if (argc == 0) {
    fprintf(stderr, "Usage: %s <input>\n", program);
    fprintf(stderr, "ERROR: no input file is provided\n");
    return 1;
  }

  const char *file_path = shit_args(&argc, &argv);

  SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
  InitWindow(screenWidth, screenHeight, "raylib game template");
  SetTargetFPS(60);

  InitAudioDevice();
  
  if (init(file_path) == 1) {
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
      if (IsKeyPressed(KEY_R)) {
          reloadDLL();
      }
      update();
  }
  // #endif
  CloseWindow(); // Close window and OpenGL context
  // system("pause");
  //--------------------------------------------------------------------------------------

  return 0;
}