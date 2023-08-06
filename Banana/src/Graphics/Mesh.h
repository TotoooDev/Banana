#pragma once
#include <Core/Ref.h>
#include <Graphics/VertexObjects.h>
#include <Graphics/Texture.h>
#include <Graphics/Material.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>

namespace Banana
{
	struct Vertex
	{
		glm::vec3 Pos;
		glm::vec3 Normal;
		glm::vec2 TextureCoords;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::mat4 transform, unsigned int materialIndex);
		Mesh(Ref<VAO> vao, Ref<EBO> ebo, glm::mat4 transform, unsigned int materialIndex);

		Ref<VAO> GetVAO() const { return m_VAO; }
		Ref<EBO> GetEBO() const { return m_EBO; }
		glm::mat4 GetTransform() const { return m_Transform; }
		unsigned int GetMaterialIndex() const { return m_MaterialIndex; }

	private:
		Ref<VAO> m_VAO;
		Ref<EBO> m_EBO;
		glm::mat4 m_Transform;
		unsigned int m_MaterialIndex = 0;
	};

	class Model
	{
	public:
		Model(const std::string& path, bool flipUVs = false);
		Model(std::vector<Mesh> meshes)
			: m_Meshes(meshes)
		{}

		std::vector<Mesh> GetMeshes() const { return m_Meshes; }
		std::vector<Material> GetMaterials() const { return m_Materials; }

	private:
		void CreateModelFromFile(const std::string& path, bool flipUVs);
		void ProcessNode(aiNode* node, const aiScene* scene, bool flipUVs);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transform, bool flipUVs);
		void ProcessTextures(aiMaterial* mat, const aiScene* scene, bool flipUVs);
		Ref<Texture> ProcessTexture(aiMaterial* mat, TextureType type, bool flipUVs);

	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Material> m_Materials;
		std::vector<Material> m_LoadedMaterials;
	};
}