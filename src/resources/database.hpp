#pragma once

#include "../memory/pool.hpp"

namespace RedFox
{
	template<typename T>
	class Accessor
	{
		public:
			inline Accessor(u32 _index)
				: m_index(_index)
			{
			}

			inline T* operator ->()
			{
				return Database::retrive<T>(m_index);
			}

		private:
			u32 m_index;
	};

	template<typename T>
	class TDatabase
	{
		public:
			template<typename ... Ts>
			u32 emplace(const str& _name, Ts&& ... _args)
			{
				//Non crea la risorsa se esiste gia
				if (m_registry.find(_name) != m_registry.end())
					return m_registry.at(_name);

				//Crea risorsa e registra index
				u32 index = m_pool.emplace(std::forward<Ts>(_args)...);
				m_registry[_name] = index;

				return index;
			}

			template<typename ... Ts>
			u32 replace(const str& _name, Ts&& ... _args)
			{
				u32 index = m_registry[_name];
				m_pool[index] = T(std::forward<Ts>(_args)...);

				return index;
			}

			inline T* retrive(const str& _name)
			{
				return &m_pool[m_registry[_name]];
			}

			inline T* retrive(u32 _index)
			{
				return &m_pool[_index];
			}

		private:
			Pool<T, 64> m_pool;
			umap<str, u32> m_registry;
	};

	class Database
	{
		public:
			template<typename T, typename ... Ts>
			static u32 create(const str& _name, Ts&& ... _args)
			{
				return storage<T>().emplace(_name, _args...);
			}

			template<typename T>
			static T* retrive(const str& _name)
			{
				return storage<T>().retrive(_name);
			}

			template<typename T>
			static T* retrive(u32 _index)
			{
				return storage<T>().retrive(_index);
			}

		private:
			template<typename T>
			static auto& storage()
			{
				static TDatabase<T> result;
				return result;
			}
	};
}