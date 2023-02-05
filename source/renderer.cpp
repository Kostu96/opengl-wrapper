#include "glw/renderer.hpp"
#include "glw/shader.hpp"
#include "glw/vertex_array.hpp"
#include "glw/buffers.hpp"

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

		static constexpr uint16_t MaxPointVerticesPerBatch = 10000;
		static constexpr uint16_t MaxQuadVerticesPerBatch = 2000 * 4;
		static constexpr uint16_t MaxQuadIndicesPerBatch = 2000 * 6;
		static struct RendererData {
			PointVertex* pointVBOBase = nullptr;
			PointVertex* pointVBOPtr = nullptr;
			glw::Shader* pointShader = nullptr;
			glw::VertexArray* pointVAO = nullptr;
			glw::VertexBuffer* pointVBO = nullptr;

			QuadVertex* quadVBOBase = nullptr;
			QuadVertex* quadVBOPtr = nullptr;
			glw::Shader* quadShader = nullptr;
			glw::VertexArray* quadVAO = nullptr;
			glw::VertexBuffer* quadVBO = nullptr;
			glw::IndexBuffer* quadIBO = nullptr;
			uint16_t currentQuadIndexCount = 0;
			float quadVertexPositions[4][2];
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

			s_data.quadVertexPositions[0][0] = -0.5f;
			s_data.quadVertexPositions[0][1] = -0.5f;
			s_data.quadVertexPositions[1][0] =  0.5f;
			s_data.quadVertexPositions[1][1] = -0.5f;
			s_data.quadVertexPositions[2][0] =  0.5f;
			s_data.quadVertexPositions[2][1] =  0.5f;
			s_data.quadVertexPositions[3][0] = -0.5f;
			s_data.quadVertexPositions[3][1] =  0.5f;
		}

		void shutdown()
		{
			delete s_data.pointVAO;
			delete s_data.pointVBO;
			delete s_data.pointShader;
			delete[] s_data.pointVBOBase;

			delete s_data.quadVAO;
			delete s_data.quadVBO;
			delete s_data.quadShader;
			delete s_data.quadIBO;
			delete[] s_data.quadVBOBase;
		}

		void beginFrame()
		{
			beginPointBatch();
			beginQuadBatch();
		}

		void endFrame()
		{
			endPointBatch();
			endQuadBatch();
		}

		void renderPoint(uint16_t x, uint16_t y, uint32_t color)
		{
			if (((uintptr_t)s_data.pointVBOPtr - (uintptr_t)s_data.pointVBOBase) / sizeof(PointVertex)
				>= MaxPointVerticesPerBatch)
			{
				endPointBatch();
				beginPointBatch();
			}

			s_data.pointVBOPtr->x = (float)x / (128.0 * 0.5) - 1.0;   //
			s_data.pointVBOPtr->y = -((float)y / (192.0 * 0.5) -1.0); // TODO: generalize
			s_data.pointVBOPtr->color = color;
			s_data.pointVBOPtr++;
		}

		void renderTexture(uint16_t x, uint16_t y, uint16_t u0, uint16_t v0, uint16_t u1, uint16_t v1)
		{
			if (s_data.currentQuadIndexCount >= MaxQuadIndicesPerBatch)
			{
				endQuadBatch();
				beginQuadBatch();
			}

			s_data.quadVBOPtr->x = (float)(x) / (256.0 * 0.5) - 1.0;
			s_data.quadVBOPtr->y = -((float)(y + 8) / (256.0 * 0.5) - 1.0);
			s_data.quadVBOPtr->u = (float)u0 / 128.0;
			s_data.quadVBOPtr->v = 1.0 - (float)v1 / 192.0;
			s_data.quadVBOPtr++;

			s_data.quadVBOPtr->x = (float)(x + 8) / (256.0 * 0.5) - 1.0;
			s_data.quadVBOPtr->y = -((float)(y + 8) / (256.0 * 0.5) - 1.0);
			s_data.quadVBOPtr->u = (float)u1 / 128.0;
			s_data.quadVBOPtr->v = 1.0 - (float)v1 / 192.0;
			s_data.quadVBOPtr++;

			s_data.quadVBOPtr->x = (float)(x + 8) / (256.0 * 0.5) - 1.0;
			s_data.quadVBOPtr->y = -((float)(y) / (256.0 * 0.5) - 1.0);
			s_data.quadVBOPtr->u = (float)u1 / 128.0;
			s_data.quadVBOPtr->v = 1.0 - (float)v0 / 192.0;
			s_data.quadVBOPtr++;

			s_data.quadVBOPtr->x = (float)(x) / (256.0 * 0.5) - 1.0;
			s_data.quadVBOPtr->y = -((float)(y) / (256.0 * 0.5) - 1.0);
			s_data.quadVBOPtr->u = (float)u0 / 128.0;
			s_data.quadVBOPtr->v = 1.0 - (float)v0 / 192.0;
			s_data.quadVBOPtr++;

			s_data.currentQuadIndexCount += 6;
		}

	} // namespace Renderer

} // namespace glw
