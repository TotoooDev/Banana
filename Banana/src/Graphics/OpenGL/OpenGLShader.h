#pragma once
#include <Graphics/Shader.h>

namespace Banana
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);

		void Bind();
		void SetInt(int value, const std::string& name);
		void SetFloat(float value, const std::string& name);
		void SetVec2(const glm::vec2& value, const std::string& name);
		void SetVec3(const glm::vec3& value, const std::string& name);
		void SetVec4(const glm::vec4& value, const std::string& name);
		void SetMat2(const glm::mat2& value, const std::string& name);
		void SetMat3(const glm::mat3& value, const std::string& name);
		void SetMat4(const glm::mat4& value, const std::string& name);
		void SetTexture(Ref<Texture> value, const std::string& name);

	private:
		void CreateFromString(const std::string& vertexSource, const std::string& fragmentSource);

	private:
		unsigned int m_ID;
	};
}