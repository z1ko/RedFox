#pragma once

#include "../common.hpp"

namespace RedFox
{
	class Application
	{
		public:
			virtual void update() = 0;
	};

	//Classe principale
	class Engine
	{
		public:
			//Punto di ingresso
			void initialize(int _argc, char** _argv);

			//Esegue un applicazione, blocca esecuzione
			void execute(Application* _application);
	};
}