#include <Core/Log.h>
#include <Graphics/Model.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Banana
{
	Model::Model(const std::string& path, bool flipTextures)
		: m_Path(path), m_FlipTextures(flipTextures)
	{
		BANANA_INFO("Loading model {}", m_Path);
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(m_Path, aiProcess_Triangulate | aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			BANANA_ERROR("Failed to load {0}! Assimp error: {1}", m_Path, importer.GetErrorString());
			return;
		}

		ProcessNode(scene->mRootNode, scene);
	}

	std::vector<Material> Model::LoadMaterials()
	{
		BANANA_INFO("Loading materials of model {}", m_Path);
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(m_Path, m_FlipTextures ? aiProcess_FlipUVs : 0);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			BANANA_ERROR("Failed to load {0}! Assimp error: {1}", m_Path, importer.GetErrorString());
			return {};
		}

		std::vector<Material> materials;
		std::vector<std::string> loadedMaterials;
		ProcessNodeMaterial(scene->mRootNode, scene, &materials, &loadedMaterials);

		return materials;
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// Process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			aiMatrix4x4 assimpTransform = node->mTransformation;
			glm::mat4 transform;
			// The a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
			transform[0][0] = assimpTransform.a1; transform[1][0] = assimpTransform.a2; transform[2][0] = assimpTransform.a3; transform[3][0] = assimpTransform.a4;
			transform[0][1] = assimpTransform.b1; transform[1][1] = assimpTransform.b2; transform[2][1] = assimpTransform.b3; transform[3][1] = assimpTransform.b4;
			transform[0][2] = assimpTransform.c1; transform[1][2] = assimpTransform.c2; transform[2][2] = assimpTransform.c3; transform[3][2] = assimpTransform.c4;
			transform[0][3] = assimpTransform.d1; transform[1][3] = assimpTransform.d2; transform[2][3] = assimpTransform.d3; transform[3][3] = assimpTransform.d4;
			m_Meshes.push_back(ProcessMesh(mesh, scene, transform));
		}
		// Then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Ref<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transform)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<unsigned int> indices;

		// Loop through every vertex in the mesh
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			glm::vec3 pos;
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			positions.push_back(pos);

			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			normals.push_back(normal);

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 texCoord;
				texCoord.x = mesh->mTextureCoords[0][i].x;
				texCoord.y = mesh->mTextureCoords[0][i].y;
				texCoords.push_back(texCoord);
			}
		}

		// Process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return CreateRef<Mesh>(mesh->mNumVertices, positions, normals, texCoords, indices, mesh->mMaterialIndex, transform);
	}

	void Model::ProcessNodeMaterial(aiNode* node, const aiScene* scene, std::vector<Material>* materials, std::vector<std::string>* loadedMaterials)
	{
		// Process all the node's materials (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMeshMaterial(mesh, scene, materials, loadedMaterials);
		}
		// Then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNodeMaterial(node->mChildren[i], scene, materials, loadedMaterials);
		}
	}

	void Model::ProcessMeshMaterial(aiMesh* mesh, const aiScene* scene, std::vector<Material>* materials, std::vector<std::string>* loadedMaterials)
	{
		aiMaterial* assimpMat = scene->mMaterials[mesh->mMaterialIndex];
		for (auto& materialName : *loadedMaterials)
		{
			if (materialName == assimpMat->GetName().C_Str())
				return;
		}

		// Process material
		Material mat;

		aiColor3D color;
		assimpMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mat.ColorDiffuse = glm::vec3(color.r, color.g, color.b);
		assimpMat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mat.ColorSpecular = glm::vec3(color.r, color.g, color.b);

		// Textures
		TextureSpecs specs(TextureWrapping::ClampToEdge, TextureFiltering::Linear);
		aiString str;
		assimpMat->GetTexture(aiTextureType_DIFFUSE, mesh->mMaterialIndex, &str);
		if (str.length > 0)
			mat.TextureDiffuse = Texture::Create(specs, str.C_Str());
		assimpMat->GetTexture(aiTextureType_SPECULAR, mesh->mMaterialIndex, &str);
		if (str.length > 0)
			mat.TextureSpecular = Texture::Create(specs, str.C_Str());

		if (!mat.TextureDiffuse)
			mat.UseColors = true;

		loadedMaterials->push_back(assimpMat->GetName().C_Str());
		materials->push_back(mat);
	}
}