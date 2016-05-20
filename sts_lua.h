////////////////////////////////////////////////////////////////////////////////
/*
 sts_lua.h - v0.01 - public domain
 written 2016 by Sebastian Steinhauer

  VERSION HISTORY
    0.01 (2016-05-20) initial version

  LICENSE
    Public domain. See "unlicense" statement at the end of this file.

  ABOUT
    Some simple Lua helper functions. I've chosen to collect all this functions here, as I repeated them over and over again in various projects :)

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __INCLUDED__STS_LUA_H__
#define __INCLUDED__STS_LUA_H__


////////////////////////////////////////////////////////////////////////////////
//
//  additional luaL_check* functions
//
#define sts_lua_checkint(L, arg)    ((int)luaL_checknumber(L, arg))
int sts_lua_checkbool(lua_State *L, int arg);


////////////////////////////////////////////////////////////////////////////////
//
//  sts_lua_set* set the given value as t[key] = value where table t is on the top of the stack.
//
void sts_lua_setstring(lua_State *L, const char *key, const char *value);
void sts_lua_setinteger(lua_State *L, const char *key, lua_Integer value);
void sts_lua_setnumber(lua_State *L, const char *key, lua_Number value);
void sts_lua_setbool(lua_State *L, const char *key, int value);


////////////////////////////////////////////////////////////////////////////////
//
//  Constant registration / Metaclass registration / Object creation
//
typedef struct {
  const char  *name;
  int         value;
} sts_lua_constant_t;

//  Registers all constants in the array c into the table on the top of the stack.
void sts_lua_setconsts(lua_State *L, const sts_lua_constant_t *c);

// Creates a new metatable with the given name and registers the given array of functions to it.
// All functions beginning with "__" will be added as metafunctions.
void sts_lua_createmeta(lua_State *L, const char *tname, const luaL_Reg *f);

// Creates a new "object" with the given size and metatable.
void *sts_lua_newobject(lua_State *L, const char *tname, size_t size);


////////////////////////////////////////////////////////////////////////////////
//
//  Simple results for functions
//
int sts_lua_pushok(lua_State *L);
int sts_lua_pusherr(lua_State *L, const char *fmt, ...);


#endif // __INCLUDED__STS_LUA_H__

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////
////    IMPLEMENTATION
////
////
#ifdef STS_LUA_IMPLEMENTATION
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


int sts_lua_checkbool(lua_State *L, int arg) {
  luaL_checkany(L, arg);
  return lua_toboolean(L, arg);
}


void sts_lua_setstring(lua_State *L, const char *key, const char *value) {
  lua_pushstring(L, value);
  lua_setfield(L, -2, key);
}


void sts_lua_setinteger(lua_State *L, const char *key, lua_Integer value) {
  lua_pushinteger(L, value);
  lua_setfield(L, -2, key);
}


void sts_lua_setnumber(lua_State *L, const char *key, lua_Number value) {
  lua_pushnumber(L, value);
  lua_setfield(L, -2, key);
}


void sts_lua_setbool(lua_State *L, const char *key, int value) {
  lua_pushboolean(L, value);
  lua_setfield(L, -2, key);
}


void sts_lua_setconsts(lua_State *L, const sts_lua_constant_t *c) {
  for (; c->name != NULL; ++c) {
    lua_pushinteger(L, c->value);
    lua_setfield(L, -2, c->name);
  }
}


void sts_lua_createmeta(lua_State *L, const char *tname, const luaL_Reg *f) {
  luaL_newmetatable(L, tname);
  lua_newtable(L);
  for (; f->name != NULL; ++f) {
    lua_pushstring(L, f->name);
    lua_pushcfunction(L, f->func);
    lua_settable(L, (f->name[0] == '_' && f->name[1] == '_') ? -4 : -3);
  }
  lua_setfield(L, -2, "__index");
  lua_pop(L, 1);
}


void *sts_lua_newobject(lua_State *L, const char *tname, size_t size) {
  void *obj = lua_newuserdata(L, size);
  luaL_setmetatable(L, tname);
  return obj;
}


int sts_lua_pushok(lua_State *L) {
  lua_pushboolean(L, 1);
  return 1;
}


int sts_lua_pusherr(lua_State *L, const char *fmt, ...) {
  va_list va;
  lua_pushnil(L);
  va_start(va, fmt);
  lua_pushvfstring(L, fmt, va);
  va_end(va);
  return 2;
}
#endif // STS_LUA_IMPLEMENTATION
/*
  This is free and unencumbered software released into the public domain.

  Anyone is free to copy, modify, publish, use, compile, sell, or
  distribute this software, either in source code form or as a compiled
  binary, for any purpose, commercial or non-commercial, and by any
  means.

  In jurisdictions that recognize copyright laws, the author or authors
  of this software dedicate any and all copyright interest in the
  software to the public domain. We make this dedication for the benefit
  of the public at large and to the detriment of our heirs and
  successors. We intend this dedication to be an overt act of
  relinquishment in perpetuity of all present and future rights to this
  software under copyright law.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  For more information, please refer to <http://unlicense.org/>
*/
