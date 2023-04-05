#pragma once

#include "Colors.h"

namespace RAVEN::Graphics
{
	struct Material
	{
		Color ambient = Colors::White;
		Color diffuse = Colors::White;
		Color specular = Colors::White;
		Color emissive = Colors::Black;
		
		float power = 1.0f;
		float padding[3] = { 0.0f }; // Memory padding to keep structure 16 byte aligned for VRAM. 

	};
}