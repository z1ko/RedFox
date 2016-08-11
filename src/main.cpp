#include "core/engine.hpp"
#include "utils\io.hpp"

#include "graphics/mesh.hpp"
#include "graphics/technique.hpp"
#include "graphics/camera.hpp"
#include "graphics/transform.hpp"

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
			camera = new Camera(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f));
			camera->position = vec3(0, 0, 2);

			triangle = new Mesh(vertices, indices);
			triangle->transform.position = vec3(0, 0, 0);
		}

		void onUpdate()
		{
			Renderer->submit(triangle);
			Renderer->flush(*camera);
		}

		void onShutdown()
		{
		}

	private:
		Camera* camera;
		Mesh* triangle;
};

int main(int _argc, char** _argv)
{
	RedFox::Engine engine;

	engine.initialize(_argc, _argv);
	engine.execute(new Sandbox);

	return 0;
}