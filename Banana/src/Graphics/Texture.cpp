#include <Core/Log.h>
#include <Graphics/Texture.h>
#include <Graphics/RendererAPI.h>
#include <Graphics/OpenGL/OpenGLTexture.h>

namespace Banana
{
	Ref<Texture> Texture::Create(const TextureSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIName::OpenGL:
			return CreateRef<OpenGLTexture>(specs);
		case RendererAPIName::Vulkan:
			BANANA_ABORT("Vulkan is not supported yet :(");
		case RendererAPIName::None:
		default:
			BANANA_ABORT("Unsupported renderer API {}!", (int)RendererAPI::GetAPI());
		}

		return nullptr;
	}

	Ref<Texture> Texture::Create(const TextureSpecs& specs, const std::string& path, bool flip)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIName::OpenGL:
			return CreateRef<OpenGLTexture>(specs, path, flip);
		case RendererAPIName::Vulkan:
			BANANA_ABORT("Vulkan is not supported yet :(");
		case RendererAPIName::None:
		default:
			BANANA_ABORT("Unsupported renderer API {}!", (int)RendererAPI::GetAPI());
		}

		return nullptr;
	}
}