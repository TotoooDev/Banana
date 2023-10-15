#include <Graphics/Log.h>
#include <Graphics/OpenGL/OpenGLVertexObjects.h>

namespace Banana
{
	OpenGLVAO::OpenGLVAO(const VertexLayout& layout, unsigned int numVertices)
	{
		m_Layout = layout;
		m_NumVertices = numVertices;

		// Generate the array and the buffers
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		// No need to create an instance VBO if we don't use instancing
		if (m_Layout.GetInstanceSize() > 0)
			glGenBuffers(1, &m_InstanceVBO);

		glBindVertexArray(m_VAO);

		// Allocate memory on the GPU
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Layout.GetVertexSize() * m_NumVertices, nullptr, GL_STATIC_DRAW);

		if (m_InstanceVBO != 0)
		{
			// Here we assume there will be one instance of the data per vertex
			glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
			glBufferData(GL_ARRAY_BUFFER, m_Layout.GetInstanceSize() * m_NumVertices, nullptr, GL_STATIC_DRAW);
		}

		// Set the attributes layout
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		unsigned int offset = 0;
		for (auto& attribute : m_Layout.GetAttributes())
		{
			glEnableVertexAttribArray(attribute.AttributeIndex);
			glVertexAttribPointer(attribute.AttributeIndex, attribute.NumData, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			switch (attribute.DataType)
			{
			case Type::Float: offset += sizeof(float) * 1 * m_NumVertices; break;
			case Type::Vec2: offset += sizeof(float) * 2 * m_NumVertices; break;
			case Type::Vec3: offset += sizeof(float) * 3 * m_NumVertices; break;
			case Type::Vec4: offset += sizeof(float) * 4 * m_NumVertices; break;
			case Type::Mat4: offset += sizeof(float) * 16 * m_NumVertices; break;
			}
		}

		if (m_InstanceVBO != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
			offset = 0;
			for (auto& attribute : m_Layout.GetAttributesInstance())
			{
				glEnableVertexAttribArray(attribute.AttributeIndex);
				glVertexAttribPointer(attribute.AttributeIndex, attribute.NumData, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				switch (attribute.DataType)
				{
				case Type::Float: offset += sizeof(float) * 1 * m_NumVertices; break;
				case Type::Vec2: offset += sizeof(float) * 2 * m_NumVertices; break;
				case Type::Vec3: offset += sizeof(float) * 3 * m_NumVertices; break;
				case Type::Vec4: offset += sizeof(float) * 4 * m_NumVertices; break;
				case Type::Mat4: offset += sizeof(float) * 16 * m_NumVertices; break;
				}

				// Set divisor
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glVertexAttribDivisor(attribute.AttributeIndex, 1);
			}
		}
	}

	OpenGLVAO::~OpenGLVAO()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		if (m_InstanceVBO != 0)
			glDeleteBuffers(1, &m_InstanceVBO);
	}

	void OpenGLVAO::SetData(unsigned int attributeIndex, std::vector<float> data)
	{
		Attribute attribute = m_Layout.GetAttributes()[attributeIndex];
		if (attribute.DataType != Type::Float)
			BANANA_RENDERER_WARN("Trying to set data for wrong type!");
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		unsigned int offset = 0;
		for (unsigned int i = 0; i < attributeIndex; i++)
			offset += m_Layout.GetAttributes()[i].Size;
		glBufferSubData(GL_ARRAY_BUFFER, offset * m_NumVertices, data.size() * attribute.Size, &data[0]);
	}
	void OpenGLVAO::SetData(unsigned int attributeIndex, std::vector<glm::vec2> data)
	{
		Attribute attribute = m_Layout.GetAttributes()[attributeIndex];
		if (attribute.DataType != Type::Vec2)
			BANANA_RENDERER_WARN("Trying to set data for wrong type!");
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		unsigned int offset = 0;
		for (unsigned int i = 0; i < attributeIndex; i++)
			offset += m_Layout.GetAttributes()[i].Size;
		glBufferSubData(GL_ARRAY_BUFFER, offset * m_NumVertices, data.size() * attribute.Size, &data[0]);
	}
	void OpenGLVAO::SetData(unsigned int attributeIndex, std::vector<glm::vec3> data)
	{
		Attribute attribute = m_Layout.GetAttributes()[attributeIndex];
		if (attribute.DataType != Type::Vec3)
			BANANA_RENDERER_WARN("Trying to set data for wrong type!");
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		unsigned int offset = 0;
		for (unsigned int i = 0; i < attributeIndex; i++)
			offset += m_Layout.GetAttributes()[i].Size;
		glBufferSubData(GL_ARRAY_BUFFER, offset * m_NumVertices, data.size() * attribute.Size, &data[0]);
	}
	void OpenGLVAO::SetData(unsigned int attributeIndex, std::vector<glm::vec4> data)
	{
		Attribute attribute = m_Layout.GetAttributes()[attributeIndex];
		if (attribute.DataType != Type::Vec4)
			BANANA_RENDERER_WARN("Trying to set data for wrong type!");
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		unsigned int offset = 0;
		for (unsigned int i = 0; i < attributeIndex; i++)
			offset += m_Layout.GetAttributes()[i].Size;
		glBufferSubData(GL_ARRAY_BUFFER, offset * m_NumVertices, data.size() * attribute.Size, &data[0]);
	}
	void OpenGLVAO::SetData(unsigned int attributeIndex, std::vector<glm::mat4> data)
	{
		Attribute attribute = m_Layout.GetAttributes()[attributeIndex];
		if (attribute.DataType != Type::Mat4)
			BANANA_RENDERER_WARN("Trying to set data for wrong type!");
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		unsigned int offset = 0;
		for (unsigned int i = 0; i < attributeIndex; i++)
			offset += m_Layout.GetAttributes()[i].Size;
		glBufferSubData(GL_ARRAY_BUFFER, offset * m_NumVertices, data.size() * attribute.Size, &data[0]);
	}

	void OpenGLVAO::SetDataInstance(unsigned int attributeIndex, std::vector<float> data)
	{
		BANANA_RENDERER_WARN("I'm too lazy to implement this yet");
	}
	void OpenGLVAO::SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec2> data)
	{
		BANANA_RENDERER_WARN("I'm too lazy to implement this yet");
	}
	void OpenGLVAO::SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec3> data)
	{
		BANANA_RENDERER_WARN("I'm too lazy to implement this yet");
	}
	void OpenGLVAO::SetDataInstance(unsigned int attributeIndex, std::vector<glm::vec4> data)
	{
		BANANA_RENDERER_WARN("I'm too lazy to implement this yet");
	}
	void OpenGLVAO::SetDataInstance(unsigned int attributeIndex, std::vector<glm::mat4> data)
	{
		BANANA_RENDERER_WARN("I'm too lazy to implement this yet");
	}

	void OpenGLVAO::Bind()
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		if (m_InstanceVBO != 0)
			glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
	}

	OpenGLEBO::OpenGLEBO()
	{
		glGenBuffers(1, &m_ID);
	}

	OpenGLEBO::~OpenGLEBO()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void OpenGLEBO::SetData(const std::vector<unsigned int>& indices)
	{
		m_Count = (unsigned int)indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	void OpenGLEBO::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
}