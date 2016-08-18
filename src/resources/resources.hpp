#pragma once

#include "../memory/pool.hpp"

namespace RedFox
{
	 class RfCnt
	 {
		  public:
				inline RfCnt()
					 : m_counter(0)
				{
				}

				inline void increase()
				{
					 m_counter++;
				}

				inline auto decrease()
				{
					 return --m_counter;
				}

		  private:
				u32 m_counter;
	 };

	 template<typename T>
	 class SmartRes
	 {
		  public:
				SmartRes()
					 : m_counter(nullptr)
				{
				}

				SmartRes(Key<T> _key)
					 : m_key(_key), m_counter(nullptr)
				{
					 m_counter = new RfCnt;
					 m_counter->increase();
				}

				SmartRes(const SmartRes<T>& _other)
					 : m_key(_other.m_key), m_counter(_other.m_counter)
				{
					 m_counter->increase();
				}

				SmartRes<T>& operator = (const SmartRes<T>& _other)
				{
					 if(this != &_other)
					 {
						  //Elimina la risorsa se non ci sono piu handle a mantenerla
						  if (m_counter != nullptr && m_counter->decrease() == 0)
						  {
								Resources::remove<T>(m_key);
								delete m_counter;
						  }

						  m_key = _other.m_key;

						  m_counter = _other.m_counter;
						  m_counter->increase();
					 }
					 return *this;
				}

				~SmartRes()
				{
					 //Elimina la risorsa se non ci sono piu handle a mantenerla
					 if (m_counter != nullptr && m_counter->decrease() == 0)
					 {
						  Resources::remove<T>(m_key);
						  delete m_counter;
					 }
				}

				T* operator ->()
				{
					 return Resources::retrive<T>(m_key);
				}

		  private:
				Key<T> m_key;
				RfCnt* m_counter;
	 };

	 class Resources
	 {
		  public:
				template<typename T, typename ... Ts>
				static auto create(Ts&& ... _args)
				{
					 Pool<T>& pool = getTypePool<T>();
					 return pool.emplace(std::forward<Ts>(_args)...);
				}

				template<typename T>
				static auto retrive(Key<T> _key)
				{
					 Pool<T>& pool = getTypePool<T>();
					 return pool[_key];
				}

				template<typename T>
				static void remove(Key<T> _key)
				{
					 Pool<T>& pool = getTypePool<T>();
					 pool.remove(_key);
				}

		  private:
				template<typename T>
				static Pool<T>& getTypePool()
				{
					 static Pool<T> result;
					 return result;
				}
	 };
}