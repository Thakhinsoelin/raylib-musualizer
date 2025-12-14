#include "double_plug.h"
#include <stdio.h>
#include "plugger.h"
#include "plug.h"

#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

DECLARE_HANDLE(HINSTANCE);
typedef HINSTANCE HMODULE;

plug_init_t plug_init = NULL;
plug_update_t plug_update = NULL;

const char* libplug = "libplug.dll";
HMODULE handle = (HMODULE)loadlibrary(libplug);

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
	printf("before unloading: %p\n", handle);
	unloadlibrary((void*)handle);
	/*plug_init = NULL;
	plug_update = NULL;*/
	//printf("Error: %s\n", getError());
	handle = (HMODULE)loadlibrary(libplug);
	printf("after unloading: %p\n", handle);
	plug_init = NULL;
	plug_update = NULL;
	loadSymbols();
	plug_update();
	/*plug_init = (plug_init_t)findsymbol(handle, plug_initn);
	if (plug_init == NULL)
	{
		printf("didn't find the function: %s\n", plug_initn);
		

	}

	plug_update = (plug_update_t)findsymbol(handle, plug_updaten);
	if (plug_update == NULL) {
		printf("can't find update function\n");
	}*/
}

int call(void) {

	return 0;
}

int init(const char* song_name) {
	loadSymbols();
	plug_init(song_name);
	return 0;
}

int update() {
	plug_update();
	return 0;
}