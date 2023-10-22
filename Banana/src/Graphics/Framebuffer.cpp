#include <Graphics/Log.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/RendererAPI.h>
#include <Graphics/OpenGL/OpenGLFramebuffer.h>

namespace Banana
{
	void FramebufferSpecs::AddTexture(FramebufferTexture texture)
	{
		if (texture != FramebufferTexture::Depth24Stencil8)
			m_Textures.push_back(texture);
		else
			m_DepthTexture = texture;
	}

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecs& specs)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIName::OpenGL:
			return CreateRef<OpenGLFramebuffer>(specs);
		case RendererAPIName::Vulkan:
			BANANA_ABORT("Vulkan is not supported yet :(");
		case RendererAPIName::None:
		default:
			BANANA_ABORT("Unsupported renderer API {}!", (int)RendererAPI::GetAPI());
		}

		return nullptr;
	}
}