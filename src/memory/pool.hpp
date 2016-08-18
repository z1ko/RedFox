#pragma once

#include "../common.hpp"

namespace RedFox
{
	 template<typename T>
	 struct Key
	 {
			Key()
				 : index(UINT_MAX), generation(UINT_MAX)
			{
			}

			Key(u32 _index, u32 _generation)
				 : index(_index), generation(_generation)
			{
			}

			u32 index;
			u32 generation;
	 };

	//Contenitore che riusa la memoria allocata
	template<typename T>
	class Pool
	{
		 public:
			  Pool()
					: m_last(0)
			  {
					for (u32 i = 0; i < RFX_POOL_CAPACITY; ++i)
					{
						 m_indices[i] = i;
						 m_generations[i] = 0;
					}
			  }

			  template<typename ... Ts>
			  Key<T> emplace(Ts&& ... _args)
			  {
					u32 index = m_indices[m_last++];
					m_elements[index] = { std::forward<Ts>(_args)... };

					RFX_MESSAGE("Creazione risorsa completata.");
					return Key<T>(index, m_generations[index]);
			  }

			  T* operator[] (Key<T> _key)
			  {
					if (m_generations[_key.index] == _key.generation)
						 return &m_elements[_key.index];
					else
					{
						 RFX_ERROR("Tentativo di accesso ad una risorsa eliminata.");
						 return nullptr;
					}
			  }

			  void remove(Key<T> _key)
			  {
					for (u32 i = 0; i < m_last; ++i)
					{
						 if (m_indices[i] == _key.index)
						 {
							  if(m_generations[_key.index] != _key.generation)
							  {
									RFX_WARNING("Tentativo di eliminazione di una risorsa eliminata.");
									return;
							  }
							  else
							  {
									//Invoca distruttore e resetta risorsa 
									m_elements[_key.index] = T();
									RFX_MESSAGE("Eliminazione risorsa completata.");
							  }

							  m_generations[_key.index]++;
							  std::swap(m_indices[i], m_indices[--m_last]);
							  return;
						 }
					}
			  }

		 private:
			  u32 m_last;
			  u32 m_indices[RFX_POOL_CAPACITY];
			  u32 m_generations[RFX_POOL_CAPACITY];

			  T m_elements[RFX_POOL_CAPACITY];
	};
}