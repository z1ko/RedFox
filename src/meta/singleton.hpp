#pragma once

#include "../common.hpp"

namespace RedFox
{
	template<typename T>
	class Singleton
	{
		public:
			static T& instance() noexcept
			{
				static T instance;
				return instance;
			}
	};
}