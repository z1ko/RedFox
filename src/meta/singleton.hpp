#pragma once

#include "../common.hpp"

namespace RedFox
{
	template<typename T>
	class Singleton
	{
		public:
			static T& get() noexcept
			{
				static T instance;
				return instance;
			}
	};
}