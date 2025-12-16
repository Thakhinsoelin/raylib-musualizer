#include "double_plug.h"
#include <stdio.h>
#include "plugger.h"
#include "plug.h"

#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

DECLARE_HANDLE(HINSTANCE);
typedef HINSTANCE HMODULE;

plug_init_t plug_init = NULL;
plug_update_t plug_update = NULL;
Plug state = {0};

const char* libplug = "libplug_loaded.dll";
HMODULE handle = NULL;

void loadSymbols() {
	plug_init = (plug_init_t)findsymbol(handle, plug_initn);
	if (plug_init == NULL)
	{
		printf("didn't find the function: %s\n", plug_initn);
		

	}
	
	plug_update = (plug_update_t)findsymbol(handle, plug_updaten);
	if (plug_update == NULL) {
		printf("can't find update function\n");
		
	}
}

void reloadDLL() {
	PauseMusicStream(state.music);
	printf("before unloading: %p\n", handle);
	plug_init = NULL;
	plug_update = NULL;
	unloadlibrary((void*)handle);
	printf("after unloading 1\n");
	if (!deletefile("libplug_loaded.dll")) {
    printf("DeleteFile failed: %lu\n", getError());
     }
	printf("after deleting the loaded\n");
	if (!filecopy("libplug.dll", "libplug_loaded.dll", false)) {
		printf("copying failed: %lu\n", getError());
	};
	handle = (HMODULE)loadlibrary(libplug);
	printf("after unloading: %p\n", handle);
	loadSymbols();
	PlayMusicStream(state.music);
}

int call(void) {

	return 0;
}

int init(const char* song_name) {
	filecopy("libplug.dll", "libplug_loaded.dll", false);
	handle = (HMODULE)loadlibrary(libplug);
	loadSymbols();
	plug_init(&state, song_name);
	return 0;
}

int update() {
	plug_update(&state);
	return 0;
}