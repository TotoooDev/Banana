#pragma once
#include <Core/Log.h>

// I think there will be a lot of logging for Lua,
// so implementing a different logging macro for Lua to differentiate easily between the messages is a good idea

#define BANANA_LUA_INFO(msg, ...) BANANA_INFO("[LUA] " ## msg, ##__VA_ARGS__)
#define BANANA_LUA_WARNING(msg, ...) BANANA_WARNING("[LUA] " ## msg, ##__VA_ARGS__)
#define BANANA_LUA_ERROR(msg, ...) BANANA_ERROR("[LUA] " ## msg, ##__VA_ARGS__)