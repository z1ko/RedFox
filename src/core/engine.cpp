#include "engine.hpp"

namespace RedFox
{
	//Punto di ingresso
	void Engine::initialize(int _argc, char** _argv)
	{
		//Crea finestringinga
		m_window = new Window("RedFox Engine", 1660, 900);

		//TODO: Crea sistemi
	}

	//Esegue un applicazione
	void Engine::execute(Application* _application)
	{
		//Setta componenti applicazione
		this->connect(_application);

		//Crea risorse
		_application->onInit();

		while(m_window->update()) 
		{
			//Aggiorna scena
			_application->onUpdate();

			//TODO: renderizza scena
		}

		//Libera risorse
		_application->onShutdown();
	}

	//Connette tutti i sistemi all'applicazione
	void Engine::connect(Application* _application)
	{
		_application->Window = m_window;
	}
}