#include "engine.hpp"

namespace RedFox
{
	//Punto di ingresso
	void Engine::initialize(int _argc, char** _argv)
	{
		//TODO: Crea finestra
		//TODO: Crea sistemi
	}

	//Esegue un applicazione
	void Engine::execute(Application* _application)
	{
		while(true)
		{
			_application->update();
		}
	}
}