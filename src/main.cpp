#include "core/engine.hpp"
#include "core/engine.hpp"
#include "utils\io.hpp"

#include "graphics/model.hpp"
#include "graphics/technique.hpp"
#include "graphics/camera.hpp"
#include "graphics/transform.hpp"
#include "graphics/shape.hpp"
#include "graphics/renderer.hpp"

using namespace RedFox;

const vector<Sampler> samplers = 
{
	{ 0, "albedo" }
};

Renderer renderer;

class Sandbox : public Application
{
	RenderCommand command;
	public:
		void onInit()
		{
			command.transform = &transform;

			command.technique = Database::create<Technique>("standard", StandardTechnique());
			command.model = Database::create<Model>("nanosuit", "C:/Development/RedFox/RedFox/res/models/nanosuit.obj");

			camera = Camera(glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f));
			camera.position = vec3(0, 7, 14);
		}

		void onUpdate()
		{
			transform.orientation.z += 0.01f;
			renderer.submitCommand(command);

			renderer.present(camera);
		}

		void onShutdown()
		{
		}

	private:
		Camera camera;
		Transform transform;
};

int main(int _argc, char** _argv)
{
	RedFox::Engine engine;

	engine.initialize(_argc, _argv);
	engine.execute(new Sandbox);

	return 0;
}