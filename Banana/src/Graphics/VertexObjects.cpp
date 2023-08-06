#include <Graphics/VertexObjects.h>

namespace Banana
{
	void VertexLayout::AddAttribute(Type type)
	{
		unsigned int size = 0;
		unsigned int numData = 0;
		switch (type)
		{
		case Type::Float: size = sizeof(float); numData = 1; break;
		case Type::Vec2: size = sizeof(float) * 2; numData = 2; break;
		case Type::Vec3: size = sizeof(float) * 3; numData = 3; break;
		case Type::Vec4: size = sizeof(float) * 4; numData = 4; break;
		case Type::Mat4: size = sizeof(float) * 16; numData = 16; break;
		}

		Attribute attribute = { type, m_AttributeIndex, size, numData };
		m_Attributes.push_back(attribute);

		m_VertexSize += size;

		m_AttributeIndex++;
		// If the attribute is a mat4 (which is basically 4 vec4s) we need to increment the attribute index by 4
		if (type == Type::Mat4)
			m_AttributeIndex += 3;
	}

	void VertexLayout::AddAttributeInstance(Type type, unsigned int num)
	{
		unsigned int size;
		switch (type)
		{
		case Type::Float: size = sizeof(float); break;
		case Type::Vec2: size = sizeof(float) * 2; break;
		case Type::Vec3: size = sizeof(float) * 3; break;
		case Type::Vec4: size = sizeof(float) * 4; break;
		case Type::Mat4: size = sizeof(float) * 16; break;
		}

		Attribute attribute = { type, m_AttributeIndex, size };
		m_AttributesInstance.push_back(attribute);

		m_InstanceSize += size;

		m_AttributeIndex++;
		// If the attribute is a mat4 (which is basically 4 vec4s) we need to increment the attribute index by 4
		if (type == Type::Mat4)
			m_AttributeIndex += 3;
	}

	VAO::VAO(const VertexLayout& layout, unsigned int numVertices)
		: m_NumVertices(numVertices), m_Layout(layout)
	{
		// Generate the array and the buffers
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		// No need to create an instance VBO if we don't use instancing
		if (m_Layout.GetInstanceSize() >= 0)
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
		for (auto& attribute : m_Layout.GetAttributes())
		{
			unsigned int offset = 0;
			glVertexAttribPointer(attribute.AttributeIndex, attribute.NumData, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			switch (attribute.DataType)
			{
			case Type::Float: offset += 1 * m_NumVertices; break;
			case Type::Vec2: offset += 2 * m_NumVertices; break;
			case Type::Vec3: offset += 3 * m_NumVertices; break;
			case Type::Vec4: offset += 4 * m_NumVertices; break;
			case Type::Mat4: offset += 16 * m_NumVertices; break;
			}
		}

		if (m_InstanceVBO != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
			for (auto& attribute : m_Layout.GetAttributesInstance())
			{
				unsigned int offset = 0;
				glVertexAttribPointer(attribute.AttributeIndex, attribute.NumData, GL_FLOAT, GL_FALSE, 0, (void*)offset);
				switch (attribute.DataType)
				{
				case Type::Float: offset += 1 * m_NumVertices; break;
				case Type::Vec2: offset += 2 * m_NumVertices; break;
				case Type::Vec3: offset += 3 * m_NumVertices; break;
				case Type::Vec4: offset += 4 * m_NumVertices; break;
				case Type::Mat4: offset += 16 * m_NumVertices; break;
				}
			}
		}
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		if (m_InstanceVBO != 0)
			glDeleteBuffers(1, &m_InstanceVBO);
	}

	void VAO::Bind()
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		if (m_InstanceVBO != 0)
			glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
	}

	EBO::EBO()
	{
		glGenBuffers(1, &m_ID);
	}

	EBO::~EBO()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void EBO::SetData(const std::vector<unsigned int>& indices)
	{
		m_Count = (unsigned int)indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	void EBO::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
}