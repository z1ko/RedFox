#pragma once

#include "../common.hpp"

namespace RedFox
{
	class Transform
	{
		public:
			//Crea trasformazione
			mat4 model() const;

			void attachTo(Transform* _parent);

		public:
			vec3 position;

		private:
			Transform* m_parent;
	};
};