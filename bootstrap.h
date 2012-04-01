#ifndef _BOOTSTRAP_H_
#define _BOOTSTRAP_H_ 1

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef pc1000
  #include "user-malloc.h"
#endif

int genericMain(void);
void outOfMemory(void);
void printErrorAndDie(const char *message);

// these functions should be defined in the platform specific main.c
char getKey(void);
void clearScreen(void);
void printXY(int x, int y, const char *message);
void platformLuaBindingSetupHook(lua_State *L);
void displayMultilineText(const char *text); // display the given text in a pagable manner, waiting returning when the cancel/esc button is pressed

#endif
