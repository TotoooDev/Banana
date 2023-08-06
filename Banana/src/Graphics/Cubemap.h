#pragma once
#include <string>
#include <vector>

namespace Banana
{
	class Cubemap
	{
	public:
		Cubemap();
		Cubemap(std::vector<std::string> facePaths);
		~Cubemap();

		void Bind(unsigned int slot = 0);

	private:
		void CreateFromFiles(std::vector<std::string> facePaths);

	private:
		unsigned int m_ID;
	};
}