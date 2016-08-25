#include "engine.hpp"

#include "../graphics/transform.hpp"

namespace RedFox
{
	namespace Globals
	{
		namespace Directories
		{
			str Executable, Models, Shaders, Textures;
		};
	};

	//Punto di ingresso
	void Engine::initialize(int _argc, char** _argv)
	{
		//Crea finestringinga
		m_window = new Window("RedFox Engine", RFX_WINDOW_WIDTH, RFX_WINDOW_HEIGHT);

		//Crea root nodi
		m_root = new Node("root");

		//Setta percorsi
		Globals::Directories::Textures = "C:/Development/RedFox/RedFox/res/textures/";
		Globals::Directories::Shaders = "C:/Development/RedFox/RedFox/res/shaders/";
		Globals::Directories::Models = "C:/Development/RedFox/RedFox/res/models/";

		//TODO: Crea sistemi
	}

	//Esegue un applicazione
	void Engine::execute(Application* _application)
	{
		//Setta componenti applicazione
		this->connect(_application);

		//Inizializza sistemi
		for (NodeSystem* system : m_systems)
			 system->initialize();

		//Crea risorse
		_application->onInit();

		while(m_window->update()) 
		{
			//Aggiorna scena
			_application->onUpdate();

			//Aggiorna transforms
			m_root->transform->bake(mat4());

			//Aggiorna sistemi
			for (NodeSystem* system : m_systems)
				 system->update();
		}

		//Libera risorse
		_application->onShutdown();
	}

	//Connette tutti i sistemi all'applicazione
	void Engine::connect(Application* _application)
	{
		_application->root = m_root;
	}
}