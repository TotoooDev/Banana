#pragma once
#include <Core/Ref.h>
#include <Graphics/Shader.h>
#include <Graphics/Framebuffer.h>
#include <Graphics/Model.h>
#include <Graphics/Camera.h>
#include <Graphics/Light.h>
#include <glm/glm.hpp>

namespace Banana
{
	enum class RendererAPIName
	{
		None,
		OpenGL,
		Vulkan
	};

	/**
	 * @brief An API for the renderer. You can define the renderer specification in the Core/Config.h file. Note that Vulkan is not supported yet.
	 *		  A RendererAPI is created automatically at the application creation.
	 *        You can create a RendererAPI using the RendererAPI::Create method but it's kinda useless.
	 */
	class RendererAPI
	{
	public:
		/**
		 * @brief Creates a RendererAPI using the API passed in parameter.
		 * @param api - The renderer specification to be used with the API.
		 */
		static Ref<RendererAPI> Create(RendererAPIName api);

		/**
		 * @brief Initializes the logging pattern for the renderer.
		 */
		static void InitLog();

		/**
		 * @returns The rendererspecification in use.
		 */
		static RendererAPIName GetAPI() { return m_API; }

	// Platform independent methods
	public:
		/**
		 * @brief Sets the projection matrix for the renderer.
		 * @param fov - The vertical FOV.
		 * @param aspectRation - The aspect ratio of the projection (usually width / height).
		 * @param nearPlane - The distance of the near clipping plane.
		 * @param farPlane - The distance of the far clipping plane.
		 */
		void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

		/**
		 * @brief Sets the framebuffer target for the next draw calls. All the following draw calls will be done to this framebuffer.
		 * @param target - The framebuffer to draw to.
		 */
		void SetTarget(Ref<Framebuffer> target);

		/**
		 * @brief Prepares for the rendering of a scene.
		 * @param cam - The camera to use when drawing.
		 * @param camPos - The position of the camera.
		 */
		void BeginScene(Ref<Camera> cam, glm::vec3 camPos);

		/**
		 * @brief Adds a directional light to the scene that is begin drawn.
		 * @param light - The directional light to use.
		 * @param direction - The direction of the light.
		 */
		void AddDirectionalLight(const DirectionalLight& light, glm::vec3 direction);

		/**
		 * @brief Adds a point light to the scene that is begin drawn.
		 * @param light - The point light to use.
		 * @param pos - The position of the light.
		 */
		void AddPointLight(const PointLight& light, glm::vec3 pos);

		/**
		 * @brief Adds a spot light to the scene that is begin drawn.
		 * @param light - The spot light to use.
		 * @param pos - The position of the light.
		 * @param direction - The direction of the light.
		 */
		void AddSpotLight(const SpotLight& light, glm::vec3 pos, glm::vec3 direction);

		/**
		 * @brief Adds a vertex object to the draw list.
		 * @param vao - The vertex array object to draw.
		 * @param ebo - The element buffer object to use.
		 * @param material - The material to use.
		 * @param transform - The model matrix used to transform the object in space.
		 */
		void Draw(Ref<VAO> vao, Ref<EBO> ebo, Material material, glm::mat4 transform);

		/**
		 * @brief Adds a mesh to the draw list.
		 * @param mesh - The mesh to draw.
		 * @param material - The material to use.
		 * @param transform - The model matrix used to transform the mesh in space.
		 */
		void Draw(Ref<Mesh> mesh, Material material, glm::mat4 transform);

		/**
		 * @brief Adds a model to the draw list.
		 * @param model - The model to draw.
		 * @param materials - A material vector used to draw the model. The vector needs to have enough materials for the model.
		 * @param transform - The model matrix used to transform the object in space.
		 */
		void Draw(Ref<Model> model, std::vector<Material> materials, glm::mat4 transform);

	// Platform specific methods
	public:
		/**
		 * @brief Sets the viewport of the renderer.
		 * @param x - The x offset.
		 * @param y - The y offset.
		 * @param width - The width of the viewport.
		 * @param height - The height of the viewport.
		 */
		virtual void SetViewport(int x, int y, int width, int height) = 0;

		/**
		 * @brief Clears the viewport with a given color.
		 * @param r - The red component of the clear color.
		 * @param g - The green component of the clear color.
		 * @param b - The blue component of the clear color.
		 * @param a - The alpha component of the clear color.
		 */
		virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) = 0;

		/**
		 * @brief Renders the draw list to the screen.
		 */
		virtual void RenderScene() = 0;

	protected:
		glm::mat4 m_CurrentView = glm::mat4(1.0f);
		glm::mat4 m_CurrentProjection = glm::mat4(1.0f);

		Ref<Shader> m_CurrentShader = nullptr;
		Ref<Shader> m_CustomShader = nullptr;
		Ref<Shader> m_ShaderColor;
		Ref<Shader> m_ShaderTexture;

		Ref<Framebuffer> m_Target = nullptr;

		struct DrawableObject
		{
			Ref<Banana::VAO> VAO;
			Ref<Banana::EBO> EBO;
			Material Mat;
			glm::mat4 Transform;
		};
		std::vector<DrawableObject> m_ObjectsToDraw;
		std::vector<std::pair<DirectionalLight, glm::vec3>> m_DirectionalLights;
		std::vector<PointLight> m_PointLights;
		std::vector<SpotLight> m_SpotLights;

	private:
		void AddDirectionalLight(Ref<Shader> shader, const DirectionalLight& light, glm::vec3 direction);
		void AddPointLight(Ref<Shader> shader, const PointLight& light, glm::vec3 pos);
		void AddSpotLight(Ref<Shader> shader, const SpotLight& light, glm::vec3 pos, glm::vec3 direction);

	private:
		static RendererAPIName m_API;

		unsigned int m_NumLights;
	};
}