#pragma once

#include "../common.hpp"
#include "../core/events.hpp"
#include "../core/nodegraph.hpp"

#include "../graphics/window.hpp"

namespace RedFox
{
	namespace Globals
	{
		namespace Directories
		{
			extern str Executable, Models, Shaders, Textures;
		};
	};

	class Application
	{
		public:
			virtual void onInit() = 0;
			virtual void onUpdate() = 0;
			virtual void onShutdown() = 0;

		public:
			//Root di tutti i nodi dell'applicazione
			Node* root;
	};

	//Classe principale
	class Engine
	{
		public:
			//Punto di ingresso
			void initialize(int _argc, char** _argv);

			//Installa un sistema
			template<typename T>
			T* install()
			{
				 m_systems.push_back(new T);
				 return (T*)m_systems.back();
			}

			//Esegue un applicazione, blocca esecuzione
			void execute(Application* _application);

		private:
			//Connette tutti i sistemi all'applicazione
			void connect(Application* _application);

		private:
			//Finestra
			Window* m_window;

			//Root di tutti i nodi dell'applicazione
			Node* m_root;

			//Systemi usato dall'engine
			vector<NodeSystem*> m_systems;
	};
}