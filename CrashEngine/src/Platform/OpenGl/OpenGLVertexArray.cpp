#include "cepch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace CrashEngine {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case CrashEngine::ShaderDataType::Float:    return GL_FLOAT;
		case CrashEngine::ShaderDataType::Float2:   return GL_FLOAT;
		case CrashEngine::ShaderDataType::Float3:   return GL_FLOAT;
		case CrashEngine::ShaderDataType::Float4:   return GL_FLOAT;
		case CrashEngine::ShaderDataType::Mat3:     return GL_FLOAT;
		case CrashEngine::ShaderDataType::Mat4:     return GL_FLOAT;
		case CrashEngine::ShaderDataType::Int:      return GL_INT;
		case CrashEngine::ShaderDataType::Int2:     return GL_INT;
		case CrashEngine::ShaderDataType::Int3:     return GL_INT;
		case CrashEngine::ShaderDataType::Int4:     return GL_INT;
		case CrashEngine::ShaderDataType::Bool:     return GL_BOOL;
		}

		CE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		CE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			if (element.Type == ShaderDataType::Mat4)
			{
				for (int i = 0; i < 4; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						4,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + i * 16));
					index++;
				}
			}
			else
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				index++;
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);

	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::AddInstancedVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, uint32_t index)
	{
		CE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t currentindex = index;

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			if (element.Type == ShaderDataType::Mat4)
			{
				for (int i = 0; i < 4; i++)
				{
					glEnableVertexAttribArray(currentindex);
					glVertexAttribPointer(currentindex,
						4,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + i * 16));
					currentindex++;
				}

				glVertexAttribDivisor(currentindex - 4, 1);
				glVertexAttribDivisor(currentindex - 3, 1);
				glVertexAttribDivisor(currentindex - 2, 1);
				glVertexAttribDivisor(currentindex - 1, 1);
			}
			else
			{
				glEnableVertexAttribArray(currentindex);
				glVertexAttribPointer(currentindex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);

				glVertexAttribDivisor(currentindex, 1);

				currentindex++;
			}		
		}

		m_VertexBuffers.push_back(vertexBuffer);

	}

}