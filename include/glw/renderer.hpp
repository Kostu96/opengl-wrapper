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
		void renderTexture(uint16_t x, uint16_t y, uint16_t u0, uint16_t v0, uint16_t u1, uint16_t v1);

	} // namespace Renderer

} // namespace glw
