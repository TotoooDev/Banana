#pragma once

/**
 * Defines the OS we are compiling for.
 * Only Windows and Linux are supported.
 */

#ifdef _WIN64
	#define BANANA_WINDOWS
#elif __linux
	#define BANANA_LINUX
#else
	#error "Your platform is not supported!"
#endif

/**
 * Defines the graphics API the renderer is using. Uncomment the specification you want to use (only one at a time!).
 * Vulkan is not implemented yet.
 */

#define BANANA_OPENGL
// #define BANANA_VULKAN