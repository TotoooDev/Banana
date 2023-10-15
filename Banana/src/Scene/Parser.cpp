#include <Scene/Parser.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Core/Log.h>
#include <fstream>

namespace Banana
{
	void Parser::ExportToJSON(const std::string& path, Scene* scene)
	{
		BANANA_INFO("Exporting scene to {}...", path);

		nlohmann::json json;
		nlohmann::json& entityJson = json["entities"];

		int entityNum = 0;
		scene->m_Registry.each([&](auto entity)
			{
				Entity ent(entity, scene);
				if (!ent.IsValid())
					return;

				ExportEntity(ent, entityJson[entityNum]);

				entityNum++;
			});

		std::ofstream file(path);
		BANANA_ASSERT(file.is_open(), "Failed to open file {}!", path);

		file << std::setw(4) << json << std::endl;

		BANANA_INFO("Finished exporting scene!");
	}

	void Parser::ImportFromJSON(const std::string& path, Scene* scene)
	{
		BANANA_INFO("Importing scene to {}...", path);

		std::ifstream file(path);
		BANANA_ASSERT(file.is_open(), "Failed to open file {}!", path);

		nlohmann::json json = nlohmann::json::parse(file);

		BANANA_INFO("Finished importing scene!");
	}

	void Parser::ExportEntity(Entity ent, nlohmann::json& entityJson)
	{
		if (ent.HasComponent<TagComponent>())
			entityJson["tag"] = ent.GetComponent<TagComponent>().Tag;

		if (ent.HasComponent<TransformComponent>())
		{
			auto& comp = ent.GetComponent<TransformComponent>();
			ExportVec3(comp.Translation, entityJson["transform"]["translation"]);
			ExportVec3(comp.Rotation, entityJson["transform"]["rotation"]);
			ExportVec3(comp.Scale, entityJson["transform"]["scale"]);
		}

		// Unfinished
		if (ent.HasComponent<PhysicsComponent>())
		{
			auto& comp = ent.GetComponent<PhysicsComponent>();
			entityJson["physics"]["type"] = comp.RigidBody.GetType();
		}

		if (ent.HasComponent<DirectionalLightComponent>())
		{
			auto& comp = ent.GetComponent<DirectionalLightComponent>();
			entityJson["directional_light"]["emit"] = comp.Emit;
			ExportVec3(comp.Light.Ambient, entityJson["directional_light"]["ambient"]);
			ExportVec3(comp.Light.Diffuse, entityJson["directional_light"]["diffuse"]);
			ExportVec3(comp.Light.Specular, entityJson["directional_light"]["specular"]);
		}

		if (ent.HasComponent<PointLightComponent>())
		{
			auto& comp = ent.GetComponent<PointLightComponent>();
			entityJson["point_light"]["emit"] = comp.Emit;
			ExportVec3(comp.Light.Ambient, entityJson["point_light"]["ambient"]);
			ExportVec3(comp.Light.Diffuse, entityJson["point_light"]["diffuse"]);
			ExportVec3(comp.Light.Specular, entityJson["point_light"]["specular"]);
			entityJson["point_light"]["constant"] = comp.Light.Constant;
			entityJson["point_light"]["linear"] = comp.Light.Linear;
			entityJson["point_light"]["quadratic"] = comp.Light.Quadratic;
		}

		if (ent.HasComponent<SpotLightComponent>())
		{
			auto& comp = ent.GetComponent<SpotLightComponent>();
			entityJson["spot_light"]["emit"] = comp.Emit;
			ExportVec3(comp.Light.Ambient, entityJson["spot_light"]["ambient"]);
			ExportVec3(comp.Light.Diffuse, entityJson["spot_light"]["diffuse"]);
			ExportVec3(comp.Light.Specular, entityJson["spot_light"]["specular"]);
			entityJson["spot_light"]["constant"] = comp.Light.Constant;
			entityJson["spot_light"]["linear"] = comp.Light.Linear;
			entityJson["spot_light"]["quadratic"] = comp.Light.Quadratic;
			entityJson["spot_light"]["cutoff"] = comp.Light.CutOff;
			entityJson["spot_light"]["outer_cutoff"] = comp.Light.OuterCutOff;
		}

		if (ent.HasComponent<ModelComponent>())
		{
			auto& comp = ent.GetComponent<ModelComponent>();
			entityJson["model"]["path"] = comp.Model->GetPath();
		}

		if (ent.HasComponent<ShaderComponent>())
		{
			auto& comp = ent.GetComponent<ShaderComponent>();
			entityJson["shader"]["vertex_path"] = comp.Shader->GetVertexPath();
			entityJson["shader"]["fragment_path"] = comp.Shader->GetFragmentPath();
		}
	}

	void Parser::ExportVec3(glm::vec3 vec, nlohmann::json& vecJson)
	{
		vecJson["x"] = vec.x;
		vecJson["y"] = vec.y;
		vecJson["z"] = vec.z;
	}
}