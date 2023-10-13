#pragma once
#include <spdlog/spdlog.h>
#include <cstdlib>

// Preprocessor hell, part 1
// The logging macros are disabled when compiling in release mode.

#ifdef BANANA_DEBUG
	#define BANANA_INFO(msg, ...)  spdlog::info((msg), ##__VA_ARGS__)
	#define BANANA_WARN(msg, ...)  spdlog::warn((msg), ##__VA_ARGS__)
	#define BANANA_ASSERT(x, msg, ...) if (!x) { BANANA_ABORT((msg), ##__VA_ARGS__); }
#else
	#define BANANA_INFO(msg, ...)
	#define BANANA_WARN(msg, ...)
	#define BANANA_ASSERT(x, msg, ...)
#endif

#define BANANA_ERROR(msg, ...) spdlog::error((msg), ##__VA_ARGS__)
#define BANANA_ABORT(msg, ...) BANANA_ERROR((msg), ##__VA_ARGS__); std::abort()
