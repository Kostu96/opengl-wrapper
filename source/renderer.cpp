#include "glw/renderer.hpp"
#include "glw/shader.hpp"
#include "glw/vertex_array.hpp"
#include "glw/buffers.hpp"
#include "glw/framebuffer.hpp"

#include <glad/gl.h>

namespace glw {

	namespace Renderer
	{

		static const char* pointVertSource =
			R"(
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;

layout(location = 0) out vec4 v_Color;

void main()
{
    v_Color = a_Color;
    gl_Position = vec4(a_Position, 0.0, 1.0);
}
)";

		static const char* pointFragSource =
			R"(
#version 450 core

layout(location = 0) in vec4 v_Color;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = v_Color;
}
)";

		static const char* quadVertSource =
			R"(
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position, 0.0, 1.0);
}
)";

		static const char* quadFragSource =
			R"(
#version 450 core

layout(location = 0) in vec2 v_TexCoord;

layout(location = 0) out vec4 fragColor;

uniform sampler2D u_Texture;

void main()
{
    fragColor = texture(u_Texture, v_TexCoord);
}
)";

		struct PointVertex {
			float x, y;
			uint32_t color;
		};
		static_assert(sizeof(PointVertex) == 12);

		struct QuadVertex {
			float x, y;
			float u, v;
		};
		static_assert(sizeof(QuadVertex) == 16);

		static constexpr uint16_t MaxPointVerticesPerBatch = 4000;
		static constexpr uint16_t MaxLineVerticesPerBatch = 2000;
		static constexpr uint16_t MaxQuadVerticesPerBatch = 1000 * 4;
		static constexpr uint16_t MaxQuadIndicesPerBatch = 1000 * 6;
		static struct RendererData {
			const glw::Framebuffer* fbo = nullptr;

			PointVertex* pointVBOBase = nullptr;
			PointVertex* pointVBOPtr = nullptr;
			glw::VertexArray* pointVAO = nullptr;
			glw::VertexBuffer* pointVBO = nullptr;
			glw::Shader* pointShader = nullptr;

			PointVertex* lineVBOBase = nullptr;
			PointVertex* lineVBOPtr = nullptr;
			glw::VertexArray* lineVAO = nullptr;
			glw::VertexBuffer* lineVBO = nullptr;

			QuadVertex* quadVBOBase = nullptr;
			QuadVertex* quadVBOPtr = nullptr;
			glw::VertexArray* quadVAO = nullptr;
			glw::VertexBuffer* quadVBO = nullptr;
			glw::IndexBuffer* quadIBO = nullptr;
			glw::Shader* quadShader = nullptr;
			uint16_t currentQuadIndexCount = 0;
		}
		s_data;

		static void beginPointBatch()
		{
			s_data.pointVBOPtr = s_data.pointVBOBase;
		}

		static void endPointBatch()
		{
			size_t dataSize = (uintptr_t)s_data.pointVBOPtr - (uintptr_t)s_data.pointVBOBase;
			if (dataSize > 0)
			{
				s_data.pointVBO->setData(s_data.pointVBOBase, dataSize);
				s_data.pointVAO->bind();
				s_data.pointShader->bind();
				glDrawArrays(GL_POINTS, 0, (GLsizei)(dataSize / sizeof(PointVertex)));
			}
		}

		static void beginLineBatch()
		{
			s_data.lineVBOPtr = s_data.lineVBOBase;
		}

		static void endLineBatch()
		{
			size_t dataSize = (uintptr_t)s_data.lineVBOPtr - (uintptr_t)s_data.lineVBOBase;
			if (dataSize > 0)
			{
				s_data.lineVBO->setData(s_data.lineVBOBase, dataSize);
				s_data.lineVAO->bind();
				s_data.pointShader->bind();
				glDrawArrays(GL_LINES, 0, (GLsizei)(dataSize / sizeof(PointVertex)));
			}
		}

		static void beginQuadBatch()
		{
			s_data.currentQuadIndexCount = 0;
			s_data.quadVBOPtr = s_data.quadVBOBase;
		}

		static void endQuadBatch()
		{
			size_t dataSize = (uintptr_t)s_data.quadVBOPtr - (uintptr_t)s_data.quadVBOBase;
			if (dataSize > 0)
			{
				s_data.quadVBO->setData(s_data.quadVBOBase, dataSize);
				s_data.quadVAO->bind();
				s_data.quadShader->bind();
				glDrawElements(GL_TRIANGLES, s_data.currentQuadIndexCount, GL_UNSIGNED_SHORT, nullptr);
			}
		}

		void init()
		{
			glActiveTexture(GL_TEXTURE0);

			s_data.pointShader = new glw::Shader{};
			s_data.pointShader->createFromSource(pointVertSource, pointFragSource);
			s_data.pointVBOBase = new PointVertex[MaxPointVerticesPerBatch];
			s_data.pointVBO = new glw::VertexBuffer{ MaxPointVerticesPerBatch * sizeof(PointVertex) };
			glw::VertexLayout pointLayout{ {
				{ glw::LayoutElement::DataType::F32_2, false },
				{ glw::LayoutElement::DataType::U8_4, true, true }
			} };
			s_data.pointVAO = new glw::VertexArray{ *s_data.pointVBO, pointLayout };

			s_data.lineVBOBase = new PointVertex[MaxLineVerticesPerBatch];
			s_data.lineVBO = new glw::VertexBuffer{ MaxLineVerticesPerBatch * sizeof(PointVertex) };
			s_data.lineVAO = new glw::VertexArray{ *s_data.lineVBO, pointLayout };

			s_data.quadShader = new glw::Shader{};
			s_data.quadShader->createFromSource(quadVertSource, quadFragSource);
			s_data.quadVBOBase = new QuadVertex[MaxQuadVerticesPerBatch];
			s_data.quadVBO = new glw::VertexBuffer{ MaxQuadVerticesPerBatch * sizeof(QuadVertex) };
			glw::VertexLayout quadLayout{ {
				{ glw::LayoutElement::DataType::F32_2, false },
				{ glw::LayoutElement::DataType::F32_2, false }
			} };
			s_data.quadVAO = new glw::VertexArray{ *s_data.quadVBO, quadLayout };
			uint16_t* quadIndices = new uint16_t[MaxQuadIndicesPerBatch];
			for (uint16_t i = 0, offset = 0; i < MaxQuadIndicesPerBatch; i += 6, offset += 4) {
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;
				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;
			}
			s_data.quadIBO = new IndexBuffer{ quadIndices, MaxQuadIndicesPerBatch, glw::IndexBuffer::IndexType::U16 };
			delete[] quadIndices;
			s_data.quadVAO->setIndexBuffer(*s_data.quadIBO);
		}

		void shutdown()
		{
			delete s_data.pointVAO;
			delete s_data.pointVBO;
			delete[] s_data.pointVBOBase;
			delete s_data.pointShader;

			delete s_data.lineVAO;
			delete s_data.lineVBO;
			delete[] s_data.lineVBOBase;

			delete s_data.quadVAO;
			delete s_data.quadVBO;
			delete s_data.quadIBO;
			delete s_data.quadShader;
			delete[] s_data.quadVBOBase;
		}

		void beginFrame(const Framebuffer* framebuffer)
		{
			s_data.fbo = framebuffer;
			if (s_data.fbo)
			{
				s_data.fbo->bind();
				glViewport(0, 0, s_data.fbo->getProperties().width, s_data.fbo->getProperties().height);
			}

			beginPointBatch();
			beginLineBatch();
			beginQuadBatch();
		}

		void endFrame()
		{
			endPointBatch();
			endLineBatch();
			endQuadBatch();

			if (s_data.fbo)
			{
				s_data.fbo->unbind();
			}
		}

		void renderPoint(uint16_t x, uint16_t y, uint32_t color)
		{
			if (((uintptr_t)s_data.pointVBOPtr - (uintptr_t)s_data.pointVBOBase) / sizeof(PointVertex)
				>= MaxPointVerticesPerBatch)
			{
				endPointBatch();
				beginPointBatch();
			}

			s_data.pointVBOPtr->x = (float)x / ((128.0 + 15.0 + 2.0) * 0.5) - 1.0;   //
			s_data.pointVBOPtr->y = -((float)y / ((192.0 + 23.0 + 2.0) * 0.5) -1.0); // TODO: generalize
			s_data.pointVBOPtr->color = color;
			s_data.pointVBOPtr++;
		}

		void renderLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color)
		{
			if (((uintptr_t)s_data.lineVBOPtr - (uintptr_t)s_data.lineVBOBase) / sizeof(PointVertex)
				>= MaxLineVerticesPerBatch)
			{
				endLineBatch();
				beginLineBatch();
			}

			s_data.lineVBOPtr->x = (float)x0 / (256.0 * 0.5) - 1.0;    //
			s_data.lineVBOPtr->y = -((float)y0 / (256.0 * 0.5) - 1.0); // TODO: generalize
			s_data.lineVBOPtr->color = color;
			s_data.lineVBOPtr++;

			s_data.lineVBOPtr->x = (float)x1 / (256.0 * 0.5) - 1.0;    //
			s_data.lineVBOPtr->y = -((float)y1 / (256.0 * 0.5) - 1.0); // TODO: generalize
			s_data.lineVBOPtr->color = color;
			s_data.lineVBOPtr++;
		}

		void renderTexture(float left, float top, float right, float bottom, float u0, float v0, float u1, float v1)
		{
			if (s_data.currentQuadIndexCount >= MaxQuadIndicesPerBatch)
			{
				endQuadBatch();
				beginQuadBatch();
			}

			s_data.quadVBOPtr->x = left;
			s_data.quadVBOPtr->y = bottom;
			s_data.quadVBOPtr->u = u0;
			s_data.quadVBOPtr->v = v1;
			s_data.quadVBOPtr++;

			s_data.quadVBOPtr->x = right;
			s_data.quadVBOPtr->y = bottom;
			s_data.quadVBOPtr->u = u1;
			s_data.quadVBOPtr->v = v1;
			s_data.quadVBOPtr++;

			s_data.quadVBOPtr->x = right;
			s_data.quadVBOPtr->y = top;
			s_data.quadVBOPtr->u = u1;
			s_data.quadVBOPtr->v = v0;
			s_data.quadVBOPtr++;

			s_data.quadVBOPtr->x = left;
			s_data.quadVBOPtr->y = top;
			s_data.quadVBOPtr->u = u0;
			s_data.quadVBOPtr->v = v0;
			s_data.quadVBOPtr++;

			s_data.currentQuadIndexCount += 6;
		}

	} // namespace Renderer

} // namespace glw
