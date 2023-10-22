#pragma once
#include <Scene/Scene.h>
#include <nlohmann/json.hpp>

namespace Banana
{
	/**
	 * STILL IN DEVELOPMENT
	 */
	class Parser
	{
	public:
		void ExportToJSON(const std::string& path, Scene* scene);
		void ImportFromJSON(const std::string& path, Scene* scene);

	private:
		void ExportEntity(Entity ent, nlohmann::json& entityJson);
		void ExportVec3(glm::vec3 vec, nlohmann::json& vecJson);
	};
}