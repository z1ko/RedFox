#include "core/engine.hpp"
#include "core/engine.hpp"
#include "utils\io.hpp"

#include "graphics/model.hpp"
#include "graphics/technique.hpp"
#include "graphics/camera.hpp"
#include "graphics/transform.hpp"
#include "graphics/deferred.hpp"
#include "graphics/scene.hpp"

#include "core/nodegraph.hpp"

using namespace RedFox;

const array<str, 6> faces =
{
	 "skyboxes/right.jpg",
	 "skyboxes/left.jpg",
	 "skyboxes/bottom.jpg",
	 "skyboxes/top.jpg",
	 "skyboxes/back.jpg",
	 "skyboxes/front.jpg",
};

class Sandbox : public Application
{
	 RootNode root;

	public:
		void onInit()
		{
			 static DeferredRenderer& renderer = DeferredRenderer::instance();

			 skybox = new CubeMap(faces);
			 renderer.setSkybox(skybox);

			 camera = new Camera(glm::perspective(glm::radians(70.0f), 16.0f / 9.0f, 0.1f, 100.0f));
			 camera->position = vec3(0, 7, 14);

			 root.addChild(new ModelNode("nanosuit.obj"));
			 root[0].setPosition(vec3(0, 0, 0));

			 light1 = new LightNode(vec3(1.f, 1.f, 1.f));
			 root.addChild(light1);

			 //light2 = new LightNode(vec3(0.f, 0.f, 1.f));
			 //root.addChild(light2);
		}

		void onUpdate()
		{
			 static float delta = 0.0f;

			 root.update(mat4(), false, 0);

			 static DeferredRenderer& renderer = DeferredRenderer::instance();
			 renderer.present(*camera);

			 light1->setPosition(vec3(sin(delta * 1.5f) * 10.f, 10.f, cos(delta * 1.5f) * 10.f));
			 //light2->setPosition(vec3(10.f, cos(delta * 2.5f) * 10.f, sin(delta * 2.5f) * 10.f));

			 delta += 0.1f;
		}

		void onShutdown()
		{
		}

	private:
		Camera* camera;
		CubeMap* skybox;
		LightNode *light1, *light2;
};

int main(int _argc, char** _argv)
{
	RedFox::Engine engine;

	RedFox::Scene scene;
	Node world = scene.create();

	Node player = world.populate();
	player.assign<u32>();

	Node enemy = world.populate();
	enemy.assign<u16>();

	bool value = player.isParentOf(enemy);

	engine.initialize(_argc, _argv);
	engine.execute(new Sandbox);

	return 0;
}