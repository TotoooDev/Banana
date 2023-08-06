#pragma once
#include <Core/Ref.h>
#include <Graphics/Mesh.h>
#include <Graphics/Camera.h>
#include <Graphics/Mesh.h>
#include <Graphics/Light.h>
#include <Graphics/Cubemap.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <functional>

namespace Banana
{
	// Essentials
	struct TransformComponent
	{
		glm::vec3 Translation;
		glm::vec3 Rotation;
		glm::vec3 Scale;

		TransformComponent(const glm::vec3& translation = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
			: Translation(translation), Rotation(rotation), Scale(scale) {}
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransfrom()
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(1.0f, 0.0f, 1.0f));

			return glm::translate(glm::mat4(1.0f), Translation) *
				rotation *
				glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	// Graphics
	struct DirectionalLightComponent
	{
		DirectionalLight Light;
		bool Emit = true;

		DirectionalLightComponent() {}
		DirectionalLightComponent(DirectionalLight light)
			: Light(light) {}
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
	};
	struct PointLightComponent
	{
		PointLight Light;
		bool Emit = true;

		PointLightComponent() {}
		PointLightComponent(PointLight light)
			: Light(light) {}
		PointLightComponent(const PointLightComponent&) = default;
	};
	struct SpotLightComponent
	{
		SpotLight Light;
		bool Emit = true;

		SpotLightComponent() {}
		SpotLightComponent(SpotLight light)
			: Light(light) {}
		SpotLightComponent(const SpotLightComponent&) = default;
	};

	struct MeshComponent
	{
		Ref<Banana::Model> Model;
		bool Draw = true;

		MeshComponent(Ref<Banana::Model> model)
			: Model(model) {}
		MeshComponent(const MeshComponent&) = default;
	};

	struct ShaderComponent
	{
		Banana::Shader Shader;

		ShaderComponent(Banana::Shader shader)
			: Shader(shader) {}
		ShaderComponent(const ShaderComponent&) = default;
	};

	struct SkyboxComponent
	{
		Ref<Cubemap> Skybox;
		bool Draw = true;

		SkyboxComponent(Ref<Cubemap> skybox)
			: Skybox(skybox) {}
		SkyboxComponent(const SkyboxComponent&) = default;
	};

	struct CameraComponent
	{
		bool Primary;
		Ref<Camera> Cam;

		CameraComponent(Ref<Camera> cam, bool primary = false)
			: Cam(cam), Primary(primary) {}
		CameraComponent(const CameraComponent&) = default;
	};

	// Scripts
	struct ScriptableComponent
	{
		std::function<void(Entity)> OnStart;
		std::function<void(Entity, double)> OnUpdate;
		std::function<void(Entity)> OnEnd;

		bool HasStarted = false;

		ScriptableComponent() {}
		ScriptableComponent(const ScriptableComponent&) = default;
	};

	struct ImGuiComponent
	{
		bool Draw = false;
		std::function<void(Entity, bool*, double)> OnDraw;

		ImGuiComponent() {}
		ImGuiComponent(const ImGuiComponent&) = default;
	};
}