#include <Graphics/Mesh.h>

namespace Banana
{
	Mesh::Mesh(unsigned int numVertices, std::vector<glm::vec3> pos, std::vector<glm::vec3> normal, std::vector<glm::vec2> texCoords, std::vector<unsigned int> indices, unsigned int materialIndex, glm::mat4 transform)
		: m_MaterialIndex(materialIndex), m_Transform(transform)
	{
		VertexLayout layout;
		layout.AddAttribute(Type::Vec3); // Pos
		layout.AddAttribute(Type::Vec3); // Normal
		if (!texCoords.empty())
			layout.AddAttribute(Type::Vec2); // Texture coords

		m_VAO = VAO::Create(layout, numVertices);
		m_VAO->SetData(0, pos);
		m_VAO->SetData(1, normal);
		if (!texCoords.empty())
			m_VAO->SetData(2, texCoords);

		m_EBO = EBO::Create();
		m_EBO->SetData(indices);
	}
}