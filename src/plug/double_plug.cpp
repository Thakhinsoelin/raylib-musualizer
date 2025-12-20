#include "double_plug.h"
//#include "plug.h"
#include <assert.h>
#include <complex>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <math.h>
#include "raylib.h"
typedef struct {
    float left;
    float right;
} Frame;
#define N (1 << 13)
float in1[N] = { 0 };
float in2[N] = { 0 };
std::complex<float> out[N] = { 0 };
float max_amp = 0;

int counter = 0;
struct Plug {
    bool loadError = false;
	Music music;
};

Plug* state = NULL;
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
    // float a = fabsf(std::real<float>(z));
    // float b = fabsf(std::imag<float>(z));
    // if (a < b)
    //     return b;
    // return a;
    float a = std::real(z);
    float b = std::imag(z);
    return log(a*a + b*b);
}

void callback(void* bufferData, unsigned int frames) {
    

    float (*fframes)[2] = (float(*)[2])bufferData;

    for (size_t i = 0; i < frames; i++) {
        memmove(in1, in1+1, (N - 1)*sizeof(in1[0]));
        in1[N-1] = fframes[i][0];
        //printf("data: %0.7f", fframes[i].right);
    }
    
}

// change this
void m_post_reload(void* s) {
    state = (Plug*)s;
    if(IsMusicValid(state->music)) {
        AttachAudioStreamProcessor(state->music.stream, callback);
    }
};

// change this 
void* m_pre_reload() {
    
    if(IsMusicValid(state->music)) {
        DetachAudioStreamProcessor(state->music.stream, callback);
        }
    return (void*)state;
};

void m_plug_init() {
    state = new Plug;
    memset(state, 0, sizeof(Plug));
	state->music = LoadMusicStream("miku_marshmellow.mp3");
	PlayMusicStream(state->music);
	SetMusicVolume(state->music, 0.25f);

    AttachAudioStreamProcessor(state->music.stream, callback);
    



	// assert(state->music.stream.sampleSize == 32);
	// printf("state->music framecounts = %u\n", state->music.frameCount);
	// printf("state->music stream sample rate = %u\n", state->music.stream.sampleRate);
	// printf("state->music stream sample size = %u\n", state->music.stream.sampleSize);
	// printf("state->music stream sample channels = %u\n", state->music.stream.channels);
	
};


int screenWidth = 800;
int screenHeight = 450;


void m_plug_update() {
    if(IsMusicValid(state->music)) {
        UpdateMusicStream(state->music);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        if(IsMusicValid(state->music)) {
            if (IsMusicStreamPlaying(state->music)) {
                PauseMusicStream(state->music);
            }
            else {
                ResumeMusicStream(state->music);
            }
        }
    }

    if(IsFileDropped()) {
        FilePathList droppedFiles = LoadDroppedFiles();
        const char* loadedfile = droppedFiles.paths[0];
        if(IsMusicValid(state->music)) {
            DetachAudioStreamProcessor(state->music.stream, callback);
            StopMusicStream(state->music);
            UnloadMusicStream(state->music);
        }

        state->music = LoadMusicStream(loadedfile);
        if(IsMusicValid(state->music)) {
            state->loadError = false;
            AttachAudioStreamProcessor(state->music.stream, callback);
            PlayMusicStream(state->music);
            SetMusicVolume(state->music, 0.25f);
        } else {
            state->loadError = true;
        }

        UnloadDroppedFiles(droppedFiles);
    }

    if(IsKeyPressed(KEY_Q)) {
        if(IsMusicValid(state->music)) {
            StopMusicStream(state->music);
            PlayMusicStream(state->music);
            SetMusicVolume(state->music, 0.25f);
        }
    }
    BeginDrawing();
    ClearBackground(GetColor(0x151515FF));
    if (IsMusicValid(state->music)) {
        // float snapshot[N];
        // memcpy(snapshot, in1, sizeof(float)*N);
        for(int i = 0; i < N; ++i) {
            float t = (float)i/N;
            float hann = 0.5 - 0.5 * cosf(2*PI*t);
            in2[i] = in1[i]*hann;
        }
        
        fft(in2, 1, out, N);
        float max_amp = 0.f;
        for (size_t i = 0; i < N; i++) {
            float a = amp(out[i]);
            if (a > max_amp) {
                max_amp = a;
            }
        }
        float step = 1.06f;
        size_t m =0;
        float lowf = 1.f;
        for (float f = lowf; (size_t)f < N/2; f = ceil(f *step)) {
            m += 1;
        }

        
        // printf("m is %d\n", m);
        float cell_width = (float)screenWidth / m;
        m = 0;
        for (float f = lowf; (size_t)f < N/2; f = ceil(f *step)) {
            float f1 = ceil(f *step);
            float a = 0.f;
            for(size_t q = (size_t)f; q < N/2 && q < (size_t) f1; ++q) {
                float b = amp(out[q]);
                if (b > a) {
                    a = b;
                }
            }
            // a /= (size_t)f1 - (size_t)f + 1;
            // printf("%f, %f", in[i], out[i]);
            float t = a / max_amp;
            DrawRectangle(m * cell_width, screenHeight / 2 - screenHeight / 2 * t,
            cell_width, screenHeight / 2 * t, GREEN);
            // DrawCircle(m * cell_width, screenHeight / 2,
            // screenHeight / 2 * t, RED);
                m += 1;
        }
    } else {
        const char* label;
        Color color;
        int height = 50;
        if (state->loadError) {
            label = "Could not load file";
            color = RED;
        } else {
            label = "Drag & Drop the music here";
            color = WHITE;
        }
        int width = MeasureText(label, height);
        DrawText(label, screenWidth/2 - width/2, screenHeight/2 - height/2, height, color);
    }
    EndDrawing();
}

void m_plug_shutdown() {
    
};