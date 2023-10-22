#pragma once
#include <Graphics/Mesh.h>
#include <assimp/scene.h>
#include <vector>

namespace Banana
{
	/**
	 * A class to represent a 3D model and its materials.
	 * The model geometry is loaded separatly from its materials, to get the materials, you can call the LoadMaterials method
	 */
	class Model
	{
	public:
		/**
		 * Loads a 3D model from a file.
		 * You can find all supported formats at https://github.com/assimp/assimp/blob/master/doc/Fileformats.md.
		 * @param path - The file path of the model.
		 * @param flipTextures - If set to true, all loaded textures will be flipped vertically.
		 */
		Model(const std::string& path, bool flipTextures = false);

		/**
		 * Loads the model's material.
		 * @returns A vector of all the materials.
		 */
		std::vector<Material> LoadMaterials();

	// Getters
	public:
		/**
		 * @returns A vector of the model's meshes.
		 */
		std::vector<Ref<Mesh>> GetMeshes() const { return m_Meshes; }

		/**
		 * @returns The file path of the model. 
		 */
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