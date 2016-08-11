#pragma once

#include "../common.hpp"
#include "../meta/singleton.hpp"
#include "../memory/pool.hpp"

namespace RedFox {

	//Facilità l'accesso alle risorse
	template<typename T>
	class Asset
	{
		friend class AssetsDatabase;

		public:
			Asset()
				: m_counter(nullptr), m_handle(0), m_database(nullptr)
			{
			}

			Asset(u32 _handle, AssetsDatabase* _database, u32 __first = 1)
				: m_handle(_handle), m_database(_database)
			{
				//Alloca contatore
				m_counter = new u32(__first);
			}

			Asset(const Asset<T>& _other)
				: m_counter(_other.m_counter), m_handle(_other.m_handle), m_database(_other.m_database)
			{
				//Aumenta contatore
				(*m_counter)++;
			}

			~Asset()
			{
				//Decrementa contatore ed elimina la risorsa se non ci sono altri handle
				if (--(*m_counter) == 0)
				{
					m_database->remove<T>(m_handle);
					delete m_counter;
				}
			}

			const T& get()
			{
				return m_database->retrive<T>(m_handle);
			}

		private:
			u32 m_handle;
			u32* m_counter;
			AssetsDatabase* m_database;
	};

	//Contenitore di tutte le risorse
	class AssetsDatabase
	{
		public:
			//Crea una risorsa con un nome
			template<typename T, typename ... Ts>
			Asset<T> create(const string& _name, Ts&& ... _args)
			{
				u32 index = getTypePool<T>().emplace(std::forward<Ts>(_args)...);
				return Asset<T>(index, this);
			}

		public:
			template<typename T>
			T& retrive(u32 _index)
			{
				return getTypePool<T>()[_index];
			}

			template<typename T>
			void remove(u32 _index)
			{
				getTypePool<T>().remove(_index);
			}

		private:
			template<typename T>
			auto& getTypePool()
			{
				static Pool<T, 1024> pool;
				return pool;
			}
	};
}