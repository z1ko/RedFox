#pragma once

#include "../common.hpp"

#define RFX_CONNECT(Class, Method, Event) Event.connect<Class, &Class::Method>(this)
#define RFX_DISCONNECT(Class, Event) Event.disconnect<Class>(this)

namespace RedFox
{
	//Magia
	template<typename ... Ts>
	class Delegate
	{
		typedef void* Inst;
		typedef void(*Func)(Inst, Ts...);

		public:
			template<typename T, void(T::*F)(Ts...)>
			void connect(T* _instance)
			{
				m_instance = _instance;
				m_function = &bridge<T, F>;
			}

			void operator() (Ts... _args)
			{
				m_function(m_instance, _args...);
			}

			bool operator == (Inst _instance)
			{
				return m_instance == _instance;
			}

		private:
			template<typename T, void(T::*F)(Ts...)>
			static void bridge(Inst _instance, Ts... _args)
			{
				(static_cast<T*>(_instance)->*F)(_args...);
			}

		private:
			Inst m_instance;
			Func m_function;
	};

	template<typename ... Ts>
	class Event
	{
		public:
			template<typename T, void(T::*F)(Ts...)>
			void connect(T* _instance)
			{
				Delegate<Ts...> tmp;
				tmp.connect<T, F>(_instance);

				m_delegates.push_back(tmp);
			}

			template<typename T>
			void disconnect(T* _instance)
			{
				for(u32 i = 0; i < m_delegates.size(); i++)
				{
					if (m_delegates[i] == _instance)
						m_delegates.erase(m_delegates.begin() + i);
				}
			}

			void operator() (Ts... _args)
			{
				for (auto& delegate : m_delegates)
					delegate(_args...);
			}

		private:
			vector<Delegate<Ts...>> m_delegates;
	};

	namespace Events
	{
		namespace Input
		{
			extern Event<u32> KeyDown, KeyUp;
		}
	}
}