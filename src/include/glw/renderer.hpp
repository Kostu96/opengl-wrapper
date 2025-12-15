#pragma once
#include <cstdint>

namespace glw {

	class Framebuffer;

	namespace Renderer
	{

		void init();
		void shutdown();

		void beginFrame(const Framebuffer* framebuffer = nullptr);
		void endFrame();

		void renderPoint(uint16_t x, uint16_t y, uint32_t color);
		void renderLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
		void renderTexture(float left, float top, float right, float bottom, float u0, float v0, float u1, float v1);

	} // namespace Renderer

} // namespace glw
