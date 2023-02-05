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

		struct PointVertex {
			float x, y;
			uint32_t color;
		};
		static_assert(sizeof(PointVertex) == 12);

		static struct RendererData
		{
			uint16_t maxPointVerticesPerBatch = 10000;
			PointVertex* pointVBOBase = nullptr;
			PointVertex* pointVBOPtr = nullptr;
			glw::Shader* pointShader = nullptr;
			glw::VertexArray* pointVAO = nullptr;
			glw::VertexBuffer* pointVBO = nullptr;
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

		void init()
		{
			s_data.pointShader = new glw::Shader{};
			s_data.pointShader->createFromSource(pointVertSource, pointFragSource);
			s_data.pointVBOBase = new PointVertex[s_data.maxPointVerticesPerBatch];
			s_data.pointVBO = new glw::VertexBuffer{ s_data.maxPointVerticesPerBatch * sizeof(PointVertex) };
			glw::VertexLayout pointLayout{ {
				{ glw::LayoutElement::DataType::F32_2, false },
				{ glw::LayoutElement::DataType::U8_4, true, true }
			} };
			s_data.pointVAO = new glw::VertexArray{ *s_data.pointVBO, pointLayout };
		}

		void shutdown()
		{
			delete s_data.pointVAO;
			delete s_data.pointVBO;
			delete s_data.pointShader;
			delete[] s_data.pointVBOBase;
		}

		void beginFrame()
		{
			beginPointBatch();
		}

		void endFrame()
		{
			endPointBatch();
		}

		void renderPoint(uint16_t x, uint16_t y, uint32_t color)
		{
			if (((uintptr_t)s_data.pointVBOPtr - (uintptr_t)s_data.pointVBOBase) / sizeof(PointVertex)
				>= s_data.maxPointVerticesPerBatch)
			{
				endPointBatch();
				beginPointBatch();
			}

			s_data.pointVBOPtr->x = (float)x / (128.0 * 0.5) - 1.0; // float(a_Position.x) / (128.0 * 0.5) - 1.0;
			s_data.pointVBOPtr->y = -((float)y / (192.0 * 0.5) -1.0); // -(float(a_Position.y) / (192.0 * 0.5) - 1.0);
			s_data.pointVBOPtr->color = color;
			s_data.pointVBOPtr++;
		}

	} // namespace Renderer

} // namespace glw
