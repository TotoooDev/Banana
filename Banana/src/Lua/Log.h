#pragma once
#include <spdlog/spdlog.h>

// Since the user might use these macros in release mode, they are always defined.

// Preprocessor hell, part 2

#define BANANA_LUA_INFO(msg, ...) spdlog::get("lua")->info(msg, __VA_ARGS__)
#define BANANA_LUA_WARNING(msg, ...) spdlog::get("lua")->warn(msg, __VA_ARGS__)
#define BANANA_LUA_ERROR(msg, ...) spdlog::get("lua")->error(msg, __VA_ARGS__)