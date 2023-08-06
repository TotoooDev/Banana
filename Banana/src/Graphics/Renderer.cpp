#include <Graphics/Renderer.h>

namespace Banana
{
	Renderer* Renderer::Get()
	{
		if (!m_Instance)
			m_Instance = new Renderer;
		return m_Instance;
	}
}