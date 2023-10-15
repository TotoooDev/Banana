#pragma once
#include <spdlog/spdlog.h>

// Preprocessor hell

#ifdef BANANA_DEBUG
	#define BANANA_LUA_INFO(msg, ...) spdlog::get("lua")->info(msg, __VA_ARGS__)
	#define BANANA_LUA_WARN(msg, ...) spdlog::get("lua")->warn(msg, __VA_ARGS__)
#else
	#define BANANA_LUA_INFO(msg, ...)
	#define BANANA_LUA_WARN(msg, ...)
#endif

#define BANANA_LUA_ERROR(msg, ...) spdlog::get("lua")->error(msg, __VA_ARGS__)