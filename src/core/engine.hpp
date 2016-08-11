#pragma once

#include "../common.hpp"
#include "../graphics/window.hpp"
#include "../assets/database.hpp"
#include "../graphics/renderer.hpp"

namespace RedFox
{
	class Application
	{
		public:
			virtual void onInit() = 0;
			virtual void onUpdate() = 0;
			virtual void onShutdown() = 0;

		public:
			//Finestra
			Window* Window;
			//Classe che renderizza le mesh nella scena
			MeshRenderer* Renderer;
			//Contenitore di tutte le risorse
			AssetsDatabase* Assets;
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
			//Finestra
			Window* m_window;
			//Classe che renderizza le mesh nella scena
			MeshRenderer* m_renderer;
			//Contenitore di tutte le risorse
			AssetsDatabase* m_assets;
	};
}