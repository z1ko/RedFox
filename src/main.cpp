#include "core/engine.hpp"

class Sandbox : public RedFox::Application
{
	public:
		void update()
		{

		}
};

int main(int _argc, char** _argv)
{
	RedFox::Engine engine;

	engine.initialize(_argc, _argv);
	engine.execute(new Sandbox);

	return 0;
}