#pragma once
#include <cstdint>

namespace glw {

	namespace Renderer
	{

		void init();
		void shutdown();

		void beginFrame();
		void endFrame();

		void renderPoint(uint16_t x, uint16_t y, uint32_t color);
		void renderTexture(float left, float top, float right, float bottom, float u0, float v0, float u1, float v1);

	} // namespace Renderer

} // namespace glw
