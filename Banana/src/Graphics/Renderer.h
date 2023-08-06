#pragma once

namespace Banana
{
	class Renderer
	{
	public:
		Renderer(Renderer&) = delete;
		void operator=(const Renderer&) = delete;

		static Renderer* Get();

	private:
		Renderer() {}

	private:
		static Renderer* m_Instance;
	};
}