#pragma once

#include "GL/glew.h"

#include "log.h"

class GLVertexBuffer
{
public:

	GLVertexBuffer(unsigned int size) :
		m_size(size)
	{
		GLCall(glGenBuffers(1, &m_rendererID));
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_DYNAMIC_DRAW));
	}

	~GLVertexBuffer()
	{
		Unbind();
		GLCall(glDeleteBuffers(1, &m_rendererID));
	}

	bool StreamData(const void* ptr, GLsizeiptr size)
	{
		if (size > m_size) return false;
		Bind();
		Flush();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, ptr));
		return true;
	}

	void Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	}

	void Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void Flush()
	{
		GLCall(glInvalidateBufferData(m_rendererID));
	}

private:
	
	GLuint m_size;
	GLuint m_rendererID;
};