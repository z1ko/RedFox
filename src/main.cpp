#include "core/engine.hpp"

#include "graphics/mesh.hpp"
#include "graphics/technique.hpp"

using namespace RedFox;

const vector<Vertex> vertices = 
{
	{{ -0.5f, -0.5f, 0.0f }},
	{{  0.5f, -0.5f, 0.0f }},
	{{  0.0f,  0.5f, 0.0f }}
};

const vector<u32> indices = 
{
	0,1,2
};

class Sandbox : public Application
{
	public:
		void onInit()
		{
			m_shape = new Shape(vertices, indices);
		}

		void onUpdate()
		{
			m_technique->enable();
			m_shape->render();
		}

		void onShutdown()
		{
		}

	private:
		GenericTechnique* m_technique;
		Shape* m_shape;
};

int main(int _argc, char** _argv)
{
	RedFox::Engine engine;

	engine.initialize(_argc, _argv);
	engine.execute(new Sandbox);

	return 0;
}