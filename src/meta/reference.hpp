#pragma once

#include "../common.hpp"

namespace RedFox
{
	template<typename T>
	class Shared
	{
		public:
			//Alloca contatore
			Shared(T* _value)
				: m_count(nullptr), m_value(_value)
			{
				m_count = new u32(1);
			}

			//Aumenta contatore
			Shared(const Shared<T>& _other)
				: m_count(_other.m_count)
			{
				(*m_count)++;
			}

			//Decrementa il contatore e se raggiunge zero elimina la risorsa
			~Shared()
			{
				if (--(*m_count) == 0) {
					delete m_value, m_count;
				}
			}

		private:
			u32* m_count;
			T*   m_value;
	};
}