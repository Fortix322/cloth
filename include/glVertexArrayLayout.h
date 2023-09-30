#pragma once

#include <vector>

#include "GL/glew.h"

struct GLVertexArrayElement
{
	unsigned int type;
	unsigned int count;
	bool normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		__debugbreak();
		return 0;
	}
};

class GLVertexArrayLayout
{
public:

	GLVertexArrayLayout()
		: m_stride(0), m_elements()
	{ }

	~GLVertexArrayLayout() { }

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(true);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, false });
		m_stride += count * GLVertexArrayElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, false });
		m_stride += count * GLVertexArrayElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, true });
		m_stride += count * GLVertexArrayElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	const std::vector<GLVertexArrayElement>& GetElements() const { return m_elements; }

	const unsigned int& GetStride() const { return m_stride; }

private:

	std::vector<GLVertexArrayElement> m_elements;
	unsigned int m_stride;
};