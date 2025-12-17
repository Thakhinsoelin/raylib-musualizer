#pragma once

#include "raylib.h"
#include <complex>

//Function names
#define plug_initn "plug_init"
#define plug_updaten "plug_update"

struct Plug {
	Music music;
};

typedef void (*add_t)(int, int);
typedef void (*plug_hello_t)(void);
typedef void (*plug_init_t)(void* state, const char* file_path);
typedef void (*plug_update_t)(void* state);
typedef void (*plug_pre_reload_t)(void* state);
typedef void (*plug_post_reload_t)(void* state);

#define LIST_OF_PLUGS \
	PLUG(plug_hello) \
	PLUG(plug_init) \
	PLUG(plug_update) \
	PLUG(plug_pre_reload) \
	PLUG(plug_post_reload)
