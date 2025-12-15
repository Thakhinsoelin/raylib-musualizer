#include "plugger.h"
#include <Windows.h>

void* loadlibrary(const char* string) {
	return LoadLibraryA(string);
};

void* findsymbol(void* handle, const char* address_name) {
	return reinterpret_cast<void*>(
        GetProcAddress((HMODULE)handle, address_name)
    );
};

void unloadlibrary(void* handle) {
	if(handle != NULL) FreeLibrary((HMODULE)handle);
	handle = NULL;
}

DWORD getError() {
	//return FormatMessage(GetLastError());
	return 4;
}