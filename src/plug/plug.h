#pragma once

#include "raylib.h"
#include <complex>

//Function names
#define plug_initn "plug_init"
#define plug_updaten "plug_update"

// typedef void (*add_t)(int, int);
// typedef void (*plug_hello_t)(void);
// typedef void (*plug_init_t)(void* state, const char* file_path);
// typedef void (*plug_update_t)(void* state);
// typedef void (*plug_pre_reload_t)(void* state);
// typedef void (*plug_post_reload_t)(void* state);


#define LIST_OF_PLUGS \
	PLUG(plug_init, void, void) \
	PLUG(plug_update, void, void) \
	PLUG(plug_pre_reload, void*, void) \
	PLUG(plug_post_reload, void, void*)

#define PLUG(name, ret, ...) typedef ret (*name##_t)(__VA_ARGS__);
LIST_OF_PLUGS
#undef PLUG