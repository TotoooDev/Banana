#pragma once

namespace Banana
{
	class Renderer
	{
	public:
		Renderer() = delete;
		void operator=(const Renderer&) = delete;

		Renderer* Get();

	private:
		Renderer() {}

	private:
		Renderer* m_Instance;
	};
}