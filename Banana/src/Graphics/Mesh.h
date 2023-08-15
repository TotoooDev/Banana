#pragma once
#include <Core/Ref.h>
#include <Graphics/VertexObjects.h>
#include <Graphics/Material.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>

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

	private:
		Ref<VAO> m_VAO;
		Ref<EBO> m_EBO;
		glm::mat4 m_Transform;
		unsigned int m_MaterialIndex;
	};

	// The model geometry is loaded separatly from its materials.
	// To get the materials, you can call the LoadMaterials method
	class Model
	{
	public:
		Model(const std::string& path, bool flipTextures = false);

		std::vector<Material> LoadMaterials();
		std::vector<Mesh> GetMeshes() const { return m_Meshes; }
		std::string GetPath() const { return m_Path; }

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transform);
		
		void ProcessNodeMaterial(aiNode* node, const aiScene* scene, std::vector<Material>* materials, std::vector<std::string>* loadedMaterials);
		void ProcessMeshMaterial(aiMesh* mesh, const aiScene* scene, std::vector<Material>* materials, std::vector<std::string>* loadedMaterials);

	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Path;
		bool m_FlipTextures;
	};
}