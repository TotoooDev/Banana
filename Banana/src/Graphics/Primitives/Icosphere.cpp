#include <Graphics/Primitives/Icosphere.h>
#include <array>

namespace Banana
{
	Icosphere::Icosphere(unsigned int subdivisions)
	{
		const float X = .525731112119133606f;
		const float Z = .850650808352039932f;
		const float N = 0.f;

		VertexList vertices =
		{
		  {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
		  {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
		  {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
		};

		TriangleList triangles =
		{
		  {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
		  {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
		  {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
		  {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
		};

		for (unsigned int i = 0; i < subdivisions; i++)
			triangles = Subdivide(vertices, triangles);

		// VAO creation

		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < triangles.size(); i++)
		{
			indices.push_back(triangles[i].Vertex[0]);
			indices.push_back(triangles[i].Vertex[1]);
			indices.push_back(triangles[i].Vertex[2]);
		}

		VertexLayout layout;
		layout.AddAttribute(Type::Vec3); // Pos

		m_VAO = VAO::Create(layout, vertices.size());
		m_VAO->SetData(0, vertices);

		m_EBO = EBO::Create();
		m_EBO->SetData(indices);
	}

	unsigned int Icosphere::VertexForEdge(Lookup& lookup, VertexList& vertices, unsigned int first, unsigned int second)
	{
		Lookup::key_type key(first, second);
		if (key.first > key.second)
			std::swap(key.first, key.second);

		auto inserted = lookup.insert({ key, vertices.size() });
		if (inserted.second)
		{
			glm::vec3 edge0 = vertices[first];
			glm::vec3 edge1 = vertices[second];
			glm::vec3 point = glm::normalize(edge0 + edge1);
			vertices.push_back(point);
		}

		return inserted.first->second;
	}

	Icosphere::TriangleList Icosphere::Subdivide(VertexList& vertices, TriangleList triangles)
	{
		Lookup lookup;
		TriangleList result;

		for (auto&& each : triangles)
		{
			std::array<unsigned int, 3> mid;
			for (int edge = 0; edge < 3; edge++)
				mid[edge] = VertexForEdge(lookup, vertices, each.Vertex[edge], each.Vertex[(edge + 1) % 3]);
		
			result.push_back({ each.Vertex[0], mid[0], mid[2] });
			result.push_back({ each.Vertex[1], mid[1], mid[0] });
			result.push_back({ each.Vertex[2], mid[2], mid[1] });
			result.push_back({ mid[0], mid[1], mid[2] });
		}
		
		return result;
	}
}