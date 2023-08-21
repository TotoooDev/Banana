#pragma once
#include <Core/Ref.h>
#include <Graphics/VertexObjects.h>
#include <Graphics/Material.h>
#include <glm/glm.hpp>

namespace Banana
{
	class Mesh
	{
	public:
		Mesh(unsigned int numVertices, std::vector<glm::vec3> pos, std::vector<glm::vec3> normal, std::vector<glm::vec2> texCoords, std::vector<unsigned int> indices, unsigned int materialIndex, glm::mat4 transform);

		Ref<VAO> GetVAO() const { return m_VAO; }
		Ref<EBO> GetEBO() const { return m_EBO; }
		glm::mat4 GetTransform() const { return m_Transform; }
		unsigned int GetMaterialIndex() const { return m_MaterialIndex; }

	protected:
		Mesh() : m_VAO(nullptr), m_EBO(nullptr), m_Transform(glm::mat4(1.0f)), m_MaterialIndex(0) {}

	protected:
		Ref<VAO> m_VAO;
		Ref<EBO> m_EBO;
		glm::mat4 m_Transform;
		unsigned int m_MaterialIndex;
	};
}