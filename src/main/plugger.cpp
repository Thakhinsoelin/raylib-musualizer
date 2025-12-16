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

unsigned long getError() {
	//return FormatMessage(GetLastError());
	return GetLastError();
}

bool filecopy(const char* source, const char* destination, bool write) {
	return CopyFileA(source, destination, write);
};

bool deletefile(const char* filename) {
	return DeleteFileA(filename);
};