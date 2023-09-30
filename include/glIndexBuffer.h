#pragma once

#include "GL/glew.h"

#include "log.h"

class GLIndexBuffer
{
public:

	GLIndexBuffer()
	{
		GLCall(glGenBuffers(1, &m_rendererID));
	}

	~GLIndexBuffer()
	{
		Unbind();
		GLCall(glDeleteBuffers(1, &m_rendererID));
	}

	void StreamData(const unsigned int* ptr, GLsizeiptr count, bool dynamicDraw = false)
	{
		GLenum draw = dynamicDraw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
		Bind();
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), ptr, draw));
	}

	void Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

private:

	GLuint m_rendererID;
};