#pragma once

#include "../common.hpp"
#include "../core/events.hpp"
#include "../resources/resources.hpp"

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
			//Finestringinga
			Window* Window;
	};

	//Classe principale
	class Engine
	{
		public:
			//Punto di ingresso
			void initialize(int _argc, char** _argv);

			//Esegue un applicazione, blocca esecuzione
			void execute(Application* _application);

		private:
			//Connette tutti i sistemi all'applicazione
			void connect(Application* _application);

		private:
			//Finestringinga
			Window* m_window;
	};
}