#pragma once

/* This shit wouldn't have been necessary if window compiler wasn't bitching about
* name collision and raylib didn't make the function the same name as windows. 
* If the compiler knows that there is indeed a name collision, why 
* don't it just fix it itself.
*/
//#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
//
//DECLARE_HANDLE(HINSTANCE);
//typedef HINSTANCE HMODULE;

void* loadlibrary(const char* string);

// Find the symbol in the dll
void* findsymbol(void* handle, const char* address_name);
void unloadlibrary(void* handle);
bool filecopy(const char* source, const char* destination, bool write);
bool deletefile(const char* filename);
unsigned long getError();