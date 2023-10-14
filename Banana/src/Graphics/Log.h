#pragma once
#include <Core/Log.h>

#ifdef BANANA_DEBUG
	#define BANANA_RENDERER_INFO(msg, ...) spdlog::get("renderer")->info(msg, ##__VA_ARGS__)
	#define BANANA_RENDERER_WARN(msg, ...) spdlog::get("renderer")->warn(msg, ##__VA_ARGS__)
#else
	#define BANANA_RENDERER_INFO(msg, ...)
	#define BANANA_RENDERER_WARN(msg, ...)
#endif

#define BANANA_RENDERER_ERROR(msg, ...) spdlog::get("renderer")->error(msg, ##__VA_ARGS__)