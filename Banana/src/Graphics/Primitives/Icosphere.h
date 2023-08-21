#pragma once
#include <Graphics/Mesh.h>
#include <map>

namespace Banana
{
	class Icosphere : public Mesh
	{
	public:
		Icosphere(unsigned int subdivisions);
	
	private:
		struct Triangle
		{
			unsigned int Vertex[3];
		};

		using TriangleList = std::vector<Triangle>;
		using VertexList = std::vector<glm::vec3>;
		using Lookup = std::map<std::pair<unsigned int, unsigned int>, unsigned int>;

	private:
		unsigned int VertexForEdge(Lookup& lookup, VertexList& vertices, unsigned int first, unsigned int second);
		TriangleList Subdivide(VertexList& vertices, TriangleList triangles);
	};
}