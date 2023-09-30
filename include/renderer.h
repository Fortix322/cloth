#pragma once

#include <string>

#include "glm.hpp"
#include "glShaderProgram.h"
#include "glVertexBuffer.h"
#include "glIndexBuffer.h"
#include "glVertexArray.h"

struct RendererConfigurations
{
	unsigned int MaxQuads;
	std::string ShaderSourceFilename;
	glm::uvec2 Resolution;
};

class Renderer
{
public:

	Renderer(const RendererConfigurations& config) :
		m_MaxQuads(config.MaxQuads), m_resolution(config.Resolution)
	{
		m_Storage = new RendererStorage(config.MaxQuads, config.ShaderSourceFilename);

		unsigned int* indicies = new unsigned int[config.MaxQuads * 6];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < config.MaxQuads * 6; i += 6)
		{
			indicies[0 + i] = 0 + offset;
			indicies[1 + i] = 1 + offset;
			indicies[2 + i] = 2 + offset;

			indicies[3 + i] = 1 + offset;
			indicies[4 + i] = 2 + offset;
			indicies[5 + i] = 3 + offset;

			offset += 4;
		}

		m_Storage->IndexBuffer->StreamData(indicies, config.MaxQuads * 6);

		delete[] indicies;
	}

	~Renderer() 
	{ 
		delete m_Storage;
	}

	void DrawCall()
	{
		if (m_Storage->Data.size() == 0) return;

		m_Storage->VertexArray->Bind();
		m_Storage->VertexBuffer->Bind();
		m_Storage->IndexBuffer->Bind();
		m_Storage->ShaderProgram->Bind();
		m_Storage->ShaderProgram->SetUniformVec2("u_resolution", m_resolution);

		m_Storage->VertexBuffer->StreamData((void*)(m_Storage->Data.data()), m_Storage->Data.size() * sizeof(Vertex));

		GLCall(glDrawElements(GL_TRIANGLES, (m_Storage->Data.size() / 4) * 6, GL_UNSIGNED_INT, nullptr));
	}

	void DrawQuad(glm::fvec3 pos, glm::fvec4 color, float size) 
	{
		if ((m_Storage->Data.size() / 4) == m_MaxQuads)
		{
			DrawCall();
			Flush();
		}
		m_Storage->Data.push_back({ glm::vec3(pos.x - size / 2.0f, pos.y - size / 2.0f, 0.0f), color });
		m_Storage->Data.push_back({ glm::vec3(pos.x + size / 2.0f, pos.y - size / 2.0f, 0.0f), color });
		m_Storage->Data.push_back({ glm::vec3(pos.x - size / 2.0f, pos.y + size / 2.0f, 0.0f), color });
		m_Storage->Data.push_back({ glm::vec3(pos.x + size / 2.0f, pos.y + size / 2.0f, 0.0f), color });
	}

	void DrawLine(glm::fvec3 lPoint, glm::fvec3 rPoint, glm::fvec4 color, float thickness)
	{
		if ((m_Storage->Data.size() / 4) == m_MaxQuads)
		{
			DrawCall();
			Flush();
		}

		m_Storage->Data.push_back({ glm::vec3(lPoint.x - thickness / 2.0f, lPoint.y - thickness / 2.0f, 0.0f), color });
		m_Storage->Data.push_back({ glm::vec3(rPoint.x + thickness / 2.0f, rPoint.y - thickness / 2.0f, 0.0f), color });
		m_Storage->Data.push_back({ glm::vec3(lPoint.x - thickness / 2.0f, lPoint.y + thickness / 2.0f, 0.0f), color });
		m_Storage->Data.push_back({ glm::vec3(rPoint.x + thickness / 2.0f, rPoint.y + thickness / 2.0f, 0.0f), color });
	}

	void Flush()
	{
		m_Storage->VertexBuffer->Flush();
		m_Storage->Data.clear();
	}

private:

	struct Vertex
	{
		glm::fvec3 Position;
		glm::fvec4 Color;

		static GLVertexArrayLayout GetLayout()
		{
			GLVertexArrayLayout layout;

			layout.Push<float>(3);
			layout.Push<float>(4);

			return layout;
		}
	};

	struct RendererStorage
	{
		RendererStorage(unsigned int MaxQuads, const std::string& shaderFilename)
		{
			VertexBuffer = new GLVertexBuffer(MaxQuads * sizeof(Vertex) * 4);
			IndexBuffer = new GLIndexBuffer();
			VertexArray = new GLVertexArray();
			ShaderProgram = new GLShaderProgram(shaderFilename);

			VertexArray->AddBuffer(*VertexBuffer, Vertex::GetLayout());
		}

		~RendererStorage()
		{
			delete ShaderProgram;
			delete VertexArray;
			delete VertexBuffer;
			delete IndexBuffer;
		}

		std::vector<Vertex> Data;

		GLVertexBuffer* VertexBuffer;
		GLIndexBuffer* IndexBuffer;
		GLVertexArray* VertexArray;
		GLShaderProgram* ShaderProgram;
	};

private:

	glm::uvec2 m_resolution;
	const unsigned int m_MaxQuads;
	Renderer::RendererStorage* m_Storage;
};