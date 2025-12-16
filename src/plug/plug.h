#pragma once
#define N 256
#include "raylib.h"
#include <complex>

//Function names
#define plug_initn "plug_init"
#define plug_updaten "plug_update"

typedef struct Plug {
	Music music;
};

typedef void (*add_t)(int, int);
typedef void (*plug_hello_t)(void);
typedef void (*plug_init_t)(void* state, const char* file_path);
typedef void (*plug_update_t)(void* state);