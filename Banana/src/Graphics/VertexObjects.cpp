#include <Graphics/VertexObjects.h>
#include <Core/Log.h>

namespace Banana
{
	void VertexLayout::AddAttribute(Type type)
	{
		int offset = m_Stride;
		switch (type)
		{
		case Type::Float:
			m_Stride += sizeof(float);
			m_TotalCount += 1;
			break;
		case Type::Vec2:
			m_Stride += sizeof(float) * 2;
			m_TotalCount += 2;
			break;
		case Type::Vec3:
			m_Stride += sizeof(float) * 3;
			m_TotalCount += 3;
			break;
		case Type::Vec4:
			m_Stride += sizeof(float) * 4;
			m_TotalCount += 4;
			break;
		case Type::Mat4:
			m_Stride += sizeof(float) * 16;
			m_TotalCount += 16;
			break;
		default:
			BANANA_WARN("Use of unsupported type attribute!");
			break;
		}
		m_Attributes.push_back({ type, offset });
	}

	VAO::VAO()
	{
		glGenVertexArrays(1, &m_ID);
		glGenBuffers(1, &m_VBO);
	}
	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &m_ID);
		glDeleteBuffers(1, &m_VBO);
	}
	void VAO::SetData(void* data, int size, const VertexLayout& layout)
	{
		glBindVertexArray(m_ID);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		for (auto& attribute : layout.GetAttributes())
		{
			switch (attribute.Type)
			{
			case Type::Float:
				glEnableVertexAttribArray(m_VertexAttribLocation);
				glVertexAttribPointer(m_VertexAttribLocation, 1, GL_FLOAT, GL_FALSE, layout.GetStride(), (void*)attribute.Offset);
				break;
			case Type::Vec2:
				glEnableVertexAttribArray(m_VertexAttribLocation);
				glVertexAttribPointer(m_VertexAttribLocation, 2, GL_FLOAT, GL_FALSE, layout.GetStride(), (void*)attribute.Offset);
				break;
			case Type::Vec3:
				glEnableVertexAttribArray(m_VertexAttribLocation);
				glVertexAttribPointer(m_VertexAttribLocation, 3, GL_FLOAT, GL_FALSE, layout.GetStride(), (void*)attribute.Offset);
				break;
			case Type::Vec4:
				glEnableVertexAttribArray(m_VertexAttribLocation);
				glVertexAttribPointer(m_VertexAttribLocation, 4, GL_FLOAT, GL_FALSE, layout.GetStride(), (void*)attribute.Offset);
				break;
			case Type::Mat4:
				// A mat4 is basically 4 vec4 so we need to take 4 locations
				for (unsigned int i = 0; i < 4; i++)
				{
					glEnableVertexAttribArray(m_VertexAttribLocation);
					glVertexAttribPointer(m_VertexAttribLocation, 4, GL_FLOAT, GL_FALSE, layout.GetStride(), (void*)attribute.Offset);
					m_VertexAttribLocation++;
				}
				break;
			default:
				BANANA_ERROR("Trying to load unsupported type to OpenGL!");
				break;
			}
			m_VertexAttribLocation++;
		}
	}

	void VAO::Bind() const
	{
		glBindVertexArray(m_ID);
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