#pragma once
#include <Graphics/VertexObjects.h>

namespace Banana
{
	class OpenGLVAO : public VAO
	{
	public:
		OpenGLVAO(const VertexLayout& layout, unsigned int numVertices);
		~OpenGLVAO();

		virtual void SetData(unsigned int attributeIndex, std::vector<float> data) override;
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::vec2> data) override;
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::vec3> data) override;
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::vec4> data) override;
		virtual void SetData(unsigned int attributeIndex, std::vector<glm::mat4> data) override;

		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<float> data) override;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec2> data) override;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec3> data) override;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec4> data) override;
		virtual void SetDataInstance(unsigned int attributeIndex, std::vector<glm::mat4> data) override;

		virtual void Bind() override;

	private:
		unsigned int m_VAO = 0;
		unsigned int m_VBO = 0;
		unsigned int m_InstanceVBO = 0;
	};

	class OpenGLEBO : public EBO
	{
	public:
		OpenGLEBO();
		~OpenGLEBO();

		void SetData(const std::vector<unsigned int>& indices);
		void Bind();

	private:
		unsigned int m_ID;
	};
}