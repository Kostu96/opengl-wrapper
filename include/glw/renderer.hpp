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

	} // namespace Renderer

} // namespace glw
