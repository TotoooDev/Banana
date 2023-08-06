#include <Core/Log.h>
#include <Graphics/Mesh.h>
#include <Graphics/Material.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Banana
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, glm::mat4 transform, unsigned int materialIndex)
		: m_Transform(transform), m_MaterialIndex(materialIndex)
	{
		m_VAO = CreateRef<VAO>();
		m_EBO = CreateRef<EBO>();

		VertexLayout layout;
		layout.AddAttribute(Type::Vec3); // Position
		layout.AddAttribute(Type::Vec3); // Normal
		layout.AddAttribute(Type::Vec2); // Texture coordinates

		m_VAO->SetData(vertices, layout);
		m_EBO->SetData(indices);
	}

	Mesh::Mesh(Ref<VAO> vao, Ref<EBO> ebo, glm::mat4 transform, unsigned int materialIndex)
		: m_VAO(vao), m_EBO(ebo), m_Transform(transform), m_MaterialIndex(materialIndex)
	{}

	Model::Model(const std::string& path, bool flipUVs)
	{
		CreateModelFromFile(path, flipUVs);
	}

	void Model::CreateModelFromFile(const std::string& path, bool flipUVs)
	{
		BANANA_INFO("Loading model {}", path);

		Assimp::Importer importer;
		aiPostProcessSteps flags = aiProcess_Triangulate;

		const aiScene* scene = importer.ReadFile(path, flags);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			BANANA_ERROR("Failed to load model at {}!", path);
			return;
		}

		ProcessNode(scene->mRootNode, scene, flipUVs);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, bool flipUVs)
	{
		std::vector<Mesh> meshes;
		// Process all the meshes if any
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			auto& assimpTransform = node->mTransformation;
			glm::mat4 transform;
			transform[0][0] = assimpTransform.a1; transform[1][0] = assimpTransform.a2; transform[2][0] = assimpTransform.a3; transform[3][0] = assimpTransform.a4;
			transform[0][1] = assimpTransform.b1; transform[1][1] = assimpTransform.b2; transform[2][1] = assimpTransform.b3; transform[3][1] = assimpTransform.b4;
			transform[0][2] = assimpTransform.c1; transform[1][2] = assimpTransform.c2; transform[2][2] = assimpTransform.c3; transform[3][2] = assimpTransform.c4;
			transform[0][3] = assimpTransform.d1; transform[1][3] = assimpTransform.d2; transform[2][3] = assimpTransform.d3; transform[3][3] = assimpTransform.d4;
			m_Meshes.push_back(ProcessMesh(mesh, scene, transform, flipUVs));
		}
		// Process other nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, flipUVs);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, glm::mat4 transform, bool flipUVs)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		// Process vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			glm::vec3 pos;
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			vertex.Pos = pos;

			if (mesh->mNormals)
			{
				glm::vec3 normal;
				normal.x = mesh->mNormals[i].x;
				normal.y = mesh->mNormals[i].y;
				normal.z = mesh->mNormals[i].z;
				vertex.Normal = normal;
			}
			else
			{
				vertex.Normal = glm::vec3(0.0f);
			}

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 texCoords;
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;
				vertex.TextureCoords = texCoords;
			}
			else
			{
				vertex.TextureCoords = glm::vec2(0.0f);
			}

			vertices.push_back(vertex);
		}

		// Process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			ProcessTextures(mat, scene, flipUVs);
		}

		return Mesh(vertices, indices, transform, mesh->mMaterialIndex);
	}

	void Model::ProcessTextures(aiMaterial* mat, const aiScene* scene, bool flipUVs)
	{
		Material material;

		// Don't process the material if it is already loaded
		for (unsigned int j = 0; j < m_LoadedMaterials.size(); j++)
		{
			if (m_LoadedMaterials[j].Name == mat->GetName().C_Str())
				return;
		}

		auto diffuse = ProcessTexture(mat, TextureType::Diffuse, flipUVs);
		auto specular = ProcessTexture(mat, TextureType::Specular, flipUVs);
		auto normal = ProcessTexture(mat, TextureType::Normal, flipUVs);

		if (!diffuse && !specular && !normal)
			return;

		material.Diffuse = diffuse;
		material.Specular = specular;
		material.Normal = normal;
		material.Name = mat->GetName().C_Str();
		aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &material.Shininess);
		m_LoadedMaterials.push_back(material); // add to loaded materials

		aiColor3D ambientColor;
		aiColor3D diffuseColor;
		aiColor3D specularColor;
		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
		material.ColorAmbient = glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b);
		material.ColorDiffuse = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
		material.ColorSpecular = glm::vec3(specularColor.r, specularColor.g, specularColor.b);

		m_Materials.push_back(material);
	}

	Ref<Texture> Model::ProcessTexture(aiMaterial* mat, TextureType type, bool flipUVs)
	{
		// Diffuse texture
		aiString str;
		aiTextureType assimpType;
		switch (type)
		{
		case TextureType::Specular:
			assimpType = aiTextureType_SPECULAR;
			break;

		case TextureType::Normal:
			assimpType = aiTextureType_HEIGHT;
			break;

		case TextureType::Diffuse:
		default:
			assimpType = aiTextureType_DIFFUSE;
			break;
		}
		mat->GetTexture(assimpType, 0, &str);

		if (str.length == 0)
			return {};

		Ref<Texture> texture = CreateRef<Texture>(str.C_Str(), flipUVs);

		return texture;
	}
}