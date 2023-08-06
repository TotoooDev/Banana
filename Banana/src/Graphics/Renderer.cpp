#include <Graphics/Renderer.h>

namespace Banana
{
	Renderer* Renderer::m_Instance = nullptr;

	Renderer* Renderer::Get()
	{
		if (!m_Instance)
			m_Instance = new Renderer;
		return m_Instance;
	}
}