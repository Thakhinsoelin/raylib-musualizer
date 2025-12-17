#include "double_plug.h"
#include "plug.h"
#include <assert.h>
#include <cstddef>
#include <string.h>

typedef struct {
    float left;
    float right;
} Frame;
#define N (1 << 14)
float in[N] = { 0 };
std::complex<float> out[N] = { 0 };
float max_amp = 0;

int counter = 0;

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
    

    Frame* fframes = (Frame*)bufferData;

    for (size_t i = 0; i < frames; i++) {
        memmove(in, in+1, (N - 1)*sizeof(in[0]));
        in[N-1] = fframes[i].left;
        //printf("data: %0.7f", fframes[i].right);
    }
    
}

void m_post_reload(void* s) {
    Plug* state = (Plug*)s;
    AttachAudioStreamProcessor(state->music.stream, callback);
};

void m_pre_reload(void* s) {
    Plug* state = (Plug*)s;
    DetachAudioStreamProcessor(state->music.stream, callback);
};

void m_plug_init(void* s, const char* file_path) {
    Plug* state = (Plug*)s;
	state->music = LoadMusicStream(file_path);
    max_amp = 0;



	assert(state->music.stream.sampleSize == 32);
	assert(state->music.stream.channels == 2);
	printf("state->music framecounts = %u\n", state->music.frameCount);
	printf("state->music stream sample rate = %u\n", state->music.stream.sampleRate);
	printf("state->music stream sample size = %u\n", state->music.stream.sampleSize);
	printf("state->music stream sample channels = %u\n", state->music.stream.channels);
	//state->music.looping = false;
	PlayMusicStream(state->music);
	SetMusicVolume(state->music, 0.25f);

    printf("entering the callback\n");
    AttachAudioStreamProcessor(state->music.stream, callback);
    printf("ending the call back\n");
};


int screenWidth = 800;
int screenHeight = 450;


void m_plug_update(void* s) {
    counter = counter + 1;
    Plug* state = (Plug*)s;
    UpdateMusicStream(state->music);
    if (IsKeyPressed(KEY_SPACE)) {
        if (IsMusicStreamPlaying(state->music)) {
            PauseMusicStream(state->music);
        }
        else {
            ResumeMusicStream(state->music);
        }
    }

    if(IsKeyPressed(KEY_Q)) {
        StopMusicStream(state->music);
        PlayMusicStream(state->music);
    }
    fft(in, 1, out, N);
    float max_amp = 0.f;
    for (size_t i = 0; i < N; i++) {
        float a = amp(out[i]);
        if (a > max_amp) {
            max_amp = a;
        }
    }
    float step = 1.06;
    size_t m =0;
    for (float f = 20.0f; (size_t)f < N; f *= step) {
        m += 1;
    }

    BeginDrawing();

    ClearBackground(GetColor(0x151515FF));
    
    // printf("m is %d\n", m);
    float cell_width = (float)screenWidth / m;
    m = 0;
    for (float f = 20.0f; (size_t)f < N; f *= step) {
        float f1 = f * step;
        float a = 0.f;
        for(size_t q = (size_t)f; q < N && q < (size_t) f1; ++q) {
            a += amp(out[q]);
        }
        a /= (size_t)f1 - (size_t)f + 1;
        // printf("%f, %f", in[i], out[i]);
        float t = a / max_amp;
        DrawRectangle(m * cell_width, screenHeight / 2 - screenHeight / 2 * t,
           cell_width, screenHeight / 2 * t, RED);
        // DrawCircle(m * cell_width, screenHeight / 2,
        // screenHeight / 2 * t, GREEN);
             m += 1;
    }

    EndDrawing();
}

void m_plug_shutdown(void* s) {
    Plug* state = (Plug*)s;
};