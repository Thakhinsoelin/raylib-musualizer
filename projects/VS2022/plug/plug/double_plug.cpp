#include "double_plug.h"
#include "plug.h"
#include <assert.h>

typedef struct {
    float left;
    float right;
} Frame;

float in[N] = { 0 };
std::complex<float> out[N] = { 0 };
float max_amp = 0;

Plug state = { 0 };

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

void callback(void* bufferData, unsigned int frames) {
    if (frames < N) {
        return;
    }

    Frame* fframes = (Frame*)bufferData;

    for (size_t i = 0; i < frames; i++) {
        in[i] = fframes[i].left;
        //printf("data: %0.7f", fframes[i].right);
    }
    
}


void m_plug_init(const char* file_path) {
    
	state.music = LoadMusicStream(file_path);
    /*if (!state.music) {
        printf("Loading music failed\n");
    }*/
    max_amp = 0;



	assert(state.music.stream.sampleSize == 32);
	assert(state.music.stream.channels == 2);
	printf("state.music framecounts = %u\n", state.music.frameCount);
	printf("state.music stream sample rate = %u\n", state.music.stream.sampleRate);
	printf("state.music stream sample size = %u\n", state.music.stream.sampleSize);
	printf("state.music stream sample channels = %u\n", state.music.stream.channels);
	state.music.looping = false;
	PlayMusicStream(state.music);
	SetMusicVolume(state.music, 0.5f);

    printf("entering the callback\n");
    AttachAudioStreamProcessor(state.music.stream, callback);
    printf("ending the call back\n");
};


int screenWidth = 800;
int screenHeight = 450;


void m_plug_update() {
    UpdateMusicStream(state.music);
    if (IsKeyPressed(KEY_SPACE)) {
        if (IsMusicStreamPlaying(state.music)) {
            PauseMusicStream(state.music);
        }
        else {
            ResumeMusicStream(state.music);
        }
    }
    fft(in, 1, out, N);

    for (size_t i = 0; i < N; i++) {
        float a = amp(out[i]);
        if (a > max_amp) {
            max_amp = a;
        }
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);
    
    

    float cell_width = (float)screenWidth / N;
    for (size_t i = 0; i < N; i++) {
        float t = amp(out[i]) / max_amp;
        
        DrawRectangle(i * cell_width, screenHeight / 2 - screenHeight / 2 * t,
            cell_width, screenHeight / 2 * t, GREEN);
    }

    // for (size_t i = 0; i < global_frames_count; i++) {
    //     float t = global_frames[i].left;
    //   if (t > 0) {
    //     /*float t = (float)sample / SINGLE_SAMPLE_MAX;*/
    //     DrawRectangle(i * cell_width, screenHeight / 2 - screenHeight / 2 *
    //     t,
    //                   1, screenHeight / 2 * t, RED);
    //   } else {
    //     /*float t = (float)sample / SINGLE_SAMPLE_MIN; */
    //     DrawRectangle(i * cell_width, screenHeight/2, 1, screenHeight / 2 *
    //     t, RED);
    //   }
    // }
    //  DrawText("Congrats! You created your first window!", 190, 200, 20,
    //  LIGHTGRAY); if(global_frames_count > 0) exit(1);
    EndDrawing();
}