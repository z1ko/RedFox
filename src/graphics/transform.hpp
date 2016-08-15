#pragma once

#include "../common.hpp"

namespace RedFox
{
	class Transform
	{
		public:
			Transform();

			//Crea trasformazione
			mat4 model() const;

		public:
			vec3 scale;
			vec3 position;
			vec3 orientation;
	};
};