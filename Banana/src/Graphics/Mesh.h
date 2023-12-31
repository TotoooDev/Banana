#pragma once
#include <Core/Ref.h>
#include <Graphics/VertexObjects.h>
#include <Graphics/Material.h>
#include <glm/glm.hpp>

namespace Banana
{
	/**
	 * A class to represent vertex objects and its associated material.
	 * You usually don't need to manipulate a Mesh itself because it gets generated automatically when creating a Model.
	 * You should prefer manipulating models over meshes
	 */
	class Mesh
	{
	public:
		/**
		 * @param numVertices - The number of vertices in the mesh.
		 * @param pos - The vertices positions.
		 * @param normal - The vertices normals.
		 * @param texCoords - The vertices texture coordinates.
		 * @param indices - The indices of the vertices.
		 * @param materialIndex - The material index to use with the MaterialComponent.
		 * @param transform - The transform matrix relative to the mesh model.
		 */
		Mesh(unsigned int numVertices, std::vector<glm::vec3> pos, std::vector<glm::vec3> normal, std::vector<glm::vec2> texCoords, std::vector<unsigned int> indices, unsigned int materialIndex, glm::mat4 transform);

		/**
		 * @returns The Vertex Array Object of the mesh.
		 */
		Ref<VAO> GetVAO() const { return m_VAO; }

		/**
		 * @returns The Element Buffer Object of the mesh.
		 */
		Ref<EBO> GetEBO() const { return m_EBO; }

		/**
		 * @returns The transform of the mesh relative to its model.
		 */
		glm::mat4 GetTransform() const { return m_Transform; }

		/**
		 * @returns The material index of the mesh.
		 */
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