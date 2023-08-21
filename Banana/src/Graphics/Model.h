#pragma once
#include <Graphics/Mesh.h>
#include <assimp/scene.h>
#include <vector>

namespace Banana
{
	// The model geometry is loaded separatly from its materials.
	// To get the materials, you can call the LoadMaterials method
	class Model
	{
	public:
		Model(const std::string& path, bool flipTextures = false);

		std::vector<Material> LoadMaterials();
		std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }
		std::string GetPath() const { return m_Path; }

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transform);

		void ProcessNodeMaterial(aiNode* node, const aiScene* scene, std::vector<Material>* materials, std::vector<std::string>* loadedMaterials);
		void ProcessMeshMaterial(aiMesh* mesh, const aiScene* scene, std::vector<Material>* materials, std::vector<std::string>* loadedMaterials);

	private:
		std::vector<Ref<Mesh>> m_Meshes;
		std::string m_Path;
		bool m_FlipTextures;
	};
}