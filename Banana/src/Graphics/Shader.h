#pragma once
#include <Core/Ref.h>
#include <string>
#include <glm/glm.hpp>

namespace Banana
{
	class Shader
	{
	public:
		static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);

		virtual void Bind() = 0;
		virtual void SetInt(int value, const std::string& name) = 0;
		virtual void SetFloat(float value, const std::string& name) = 0;
		virtual void SetVec2(const glm::vec2& value, const std::string& name) = 0;
		virtual void SetVec3(const glm::vec3& value, const std::string& name) = 0;
		virtual void SetVec4(const glm::vec4& value, const std::string& name) = 0;
		virtual void SetMat2(const glm::mat2& value, const std::string& name) = 0;
		virtual void SetMat3(const glm::mat3& value, const std::string& name) = 0;
		virtual void SetMat4(const glm::mat4& value, const std::string& name) = 0;

	protected:
		std::string m_VertexPath, m_FragmentPath;
	};
}