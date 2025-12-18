#include "double_plug.h"
#include <stdio.h>
#include "plugger.h"
#include "plug.h"

#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

DECLARE_HANDLE(HINSTANCE);
typedef HINSTANCE HMODULE;

#ifdef HOTRELOAD
#define PLUG(name, ...) name##_t name = NULL;
#else
#define PLUG(name, ...) extern "C" void name(...);
#endif
LIST_OF_PLUGS
#undef PLUG
// plug_init_t plug_init = NULL;
// plug_update_t plug_update = NULL;
// plug_post_reload_t plug_pos_reload = NULL;
// plug_pre_reload_t plug_pre_reload = NULL;


const char* libplug = "libplug_loaded.dll";
HMODULE handle = NULL;

void loadSymbols() {
	#ifdef HOTRELOAD
	#define PLUG(name, ...) \
		name = (name##_t)findsymbol(handle, #name); \
		if (name == NULL) \
		{ \
			fprintf(stderr, "ERROR: could not find %s symbol in %s.\n", \
				#name, libplug); \
		}
	LIST_OF_PLUGS
	#undef PLUG
	#endif

}

#ifdef HOTRELOAD
void reloadDLL() {
	//PauseMusicStream(state.music);
	void* state = plug_pre_reload();
	printf("before unloading: %p\n", handle);
	#define PLUG(name, ...) name = NULL;
	LIST_OF_PLUGS
	#undef PLUG
	// plug_init = NULL;
	// plug_update = NULL;
	// plug_post_reload = NULL;
	// plug_pre_reload = NULL;
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
	plug_post_reload(state);
	//PlayMusicStream(state.music);
}
#else
void reloadDLL() {

}
#endif

int call(void) {

	return 0;
}

int init() {
	#ifdef HOTRELOAD
	filecopy("libplug.dll", "libplug_loaded.dll", false);
	handle = (HMODULE)loadlibrary(libplug);
	loadSymbols();
	#endif
	plug_init();
	return 0;
}

int update() {
	plug_update();
	return 0;
}