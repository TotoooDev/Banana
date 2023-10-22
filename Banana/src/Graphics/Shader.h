#pragma once
#include <Core/Ref.h>
#include <Graphics/Texture.h>
#include <string>
#include <glm/glm.hpp>

namespace Banana
{
	/**
	 * Provides an API for shaders.
	 */
	class Shader
	{
	public:
		/**
		 * Creates a new shader from files.
		 * @returns A reference to the newly created shader.
		 * @param vertexPath - The file path of the vertex shader code.
		 * @param fragmentPath - The file path of the fragment shader code.
		 */
		static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);

		/**
		 * Binds the shader for future uses.
		 */
		virtual void Bind() = 0;

	// Getters
	public:
		/**
		 * @returns The vertex shader file path.
		 */
		std::string GetVertexPath() { return m_VertexPath; }

		/**
		 * @returns The fragment shader file path.
		 */
		std::string GetFragmentPath() { return m_FragmentPath; }

	// Setters
	public:
		/**
		 * Sets an int uniform associated with a name.
		 * @param value - The value to load to the uniform.
		 * @param name - The name of the uniform.
		 */
		virtual void SetInt(int value, const std::string& name) = 0;
		/**
		 * Sets a float uniform associated with a name.
		 * @param value - The value to load to the uniform.
		 * @param name - The name of the uniform.
		 */
		virtual void SetFloat(float value, const std::string& name) = 0;
		/**
		 * Sets a Vec2 uniform associated with a name.
		 * @param value - The value to load to the uniform.
		 * @param name - The name of the uniform.
		 */
		virtual void SetVec2(const glm::vec2& value, const std::string& name) = 0;
		/**
		 * Sets a Vec3 uniform associated with a name.
		 * @param value - The value to load to the uniform.
		 * @param name - The name of the uniform.
		 */
		virtual void SetVec3(const glm::vec3& value, const std::string& name) = 0;
		/**
		 * Sets a Vec4 uniform associated with a name.
		 * @param value - The value to load to the uniform.
		 * @param name - The name of the uniform.
		 */
		virtual void SetVec4(const glm::vec4& value, const std::string& name) = 0;
		/**
		 * Sets a 2x2 matrix uniform associated with a name.
		 * @param value - The value to load to the uniform.
		 * @param name - The name of the uniform.
		 */
		virtual void SetMat2(const glm::mat2& value, const std::string& name) = 0;
		/**
		 * Sets a 3x3 matrix uniform associated with a name.
		 * @param value - The value to load to the uniform.
		 * @param name - The name of the uniform.
		 */
		virtual void SetMat3(const glm::mat3& value, const std::string& name) = 0;
		/**
		 * Sets a 4x4 matrix uniform associated with a name.
		 * @param value - The value to load to the uniform.
		 * @param name - The name of the uniform.
		 */
		virtual void SetMat4(const glm::mat4& value, const std::string& name) = 0;
		/**
		 * Sets a sampler2D uniform associated with a name.
		 * @param value - The value to load to the uniform.
		 * @param name - The name of the uniform.
		 */
		virtual void SetTexture(Ref<Texture> value, const std::string& name) = 0;

	protected:
		std::string m_VertexPath, m_FragmentPath;
	};
}