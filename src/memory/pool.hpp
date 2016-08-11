#pragma once

#include "../common.hpp"

namespace RedFox
{
	//Contenitore che riusa la memoria allocata
	template<typename T, u32 S>
	class Pool
	{
		public:
			//Prepara indici
			Pool()
				: m_last(0)
			{
				for (u32 i = 0; i < S; ++i)
					m_indices[i] = i;
			}

			template<typename ... Ts>
			u32 emplace(Ts&& ... _args)
			{
				u32 index = m_indices[m_last++];
				m_elements[index] = T(std::forward<Ts>(_args)...);
				return index;
			}

			T& operator[] (u32 _index)
			{
				return m_elements[_index];
			}

			void remove(u32 _index)
			{
				for (u32 i = 0; i < m_last; ++i)
				{
					if (m_indices[i] == _index)
					{
						std::swap(m_indices[i], m_indices[--m_last]);
						return;
					}
				}
			}

		private:
			u32 m_last;
			u32 m_indices  [S];
			T	m_elements [S];
	};
}