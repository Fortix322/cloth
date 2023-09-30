#pragma once

#include "GL/glew.h"

#include "glVertexBuffer.h"
#include "glVertexArrayLayout.h"

#include "log.h"

class GLVertexArray
{
public:

	GLVertexArray()
	{
		GLCall(glCreateVertexArrays(1, &m_rendererID));
		Bind();
	}

	~GLVertexArray()
	{
		Unbind();
		GLCall(glDeleteVertexArrays(1, &m_rendererID));
	}

	void AddBuffer(const GLVertexBuffer& buffer, const GLVertexArrayLayout& layout)
	{
		buffer.Bind();
		const auto& elements = layout.GetElements();
		uint64_t offset = 0;
		for (uint32_t i = 0; i < elements.size(); i++)
		{
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (const void*)offset));

			offset += elements[i].count * elements[i].GetSizeOfType(elements[i].type);
		}
	}

	void Bind() const
	{
		GLCall(glBindVertexArray(m_rendererID));
	}

	void Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

private:
	unsigned int m_rendererID;
};