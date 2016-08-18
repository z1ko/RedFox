#include "core/engine.hpp"
#include "core/engine.hpp"
#include "utils\io.hpp"

#include "graphics/model.hpp"
#include "graphics/technique.hpp"
#include "graphics/camera.hpp"
#include "graphics/transform.hpp"
#include "graphics/shape.hpp"
#include "graphics/renderer.hpp"
#include "graphics/deferred.hpp"

#include "experimental/graphics.hpp"

using namespace RedFox;

class Sandbox : public Application
{
	public:
		void onInit()
		{
			 geometryPass.initialize()
						    .attach(Shader("deferred/geometry.vtx.glsl", GL_VERTEX_SHADER))
						    .attach(Shader("deferred/geometry.frg.glsl", GL_FRAGMENT_SHADER))
						    .link();

			 geometryPass.enable();

			 geometryPass.setUniform("material.albedo", 0u);

			 lightingPass.initialize()
						    .attach(Shader("deferred/lighting.vtx.glsl", GL_VERTEX_SHADER))
						    .attach(Shader("deferred/lighting.frg.glsl", GL_FRAGMENT_SHADER))
						    .link();

			 lightingPass.enable();

			 lightingPass.setUniform("gbuffer.position", 0u);
			 lightingPass.setUniform("gbuffer.normal",	1u);
			 lightingPass.setUniform("gbuffer.color",		2u);

			 skyboxPass.initialize()
						  .attach(Shader("skybox.vtx.glsl", GL_VERTEX_SHADER))
						  .attach(Shader("skybox.frg.glsl", GL_FRAGMENT_SHADER))
						  .link();

			 skyboxPass.enable();

			 gBuffer = new GBuffer(1600, 900);

			 camera = new Camera(glm::perspective(glm::radians(70.0f), 16.0f / 9.0f, 0.1f, 100.0f));
			 camera->position = vec3(0, 7, 14);

			 model = Exp::Model("nanosuit.obj");
		}

		void onUpdate()
		{
			 static float delta = 0.0f;

			 //Inizio rendering scena
			 gBuffer->bindForWriting();
			 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



			 geometryPass.enable();
			 geometryPass.setCamera(*camera);

			 //Rendering oggetti della scena
			 transform.position = vec3(0, 0, 0);
			 geometryPass.setTransform(transform);
			 model.render();

			 transform.position = vec3(-10, 0, -10);
			 geometryPass.setTransform(transform);
			 model.render();

			 transform.position = vec3(10, 0, -10);
			 geometryPass.setTransform(transform);
			 model.render();

			 //Fine rendering scena
			 gBuffer->unbind();
			 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			 //====================================================================

			 lightingPass.enable();
			 lightingPass.setUniform("camera.position", camera->position);

			 //Setta luci
			 lightingPass.setUniform("lights[0].position", vec3(sin(delta) * 10.f, 15, cos(delta) * 10.f));
			 lightingPass.setUniform("lights[0].color", vec3(1, 0, 0));

			 lightingPass.setUniform("lights[1].position", vec3(0.0f, sin(delta * 0.5f) * 15.f, cos(delta * 0.5f) * 10.f));
			 lightingPass.setUniform("lights[1].color", vec3(0, 0, 1));

			 //lightingPass.setUniform("lights[2].position", camera->position);
			 //lightingPass.setUniform("lights[2].color", vec3(1,1,1));

			 gBuffer->bindTextures();

			 static Shape screen = Shape::Screen();
			 
			 //glViewport(1600 * 0.25f, 0.0f, 1600 * 0.75f, 900.0f);
			 screen.render();

			 //====================================================================

			 gBuffer->bindForReading();

			 //Copia una texture del gbuffer sullo schermo, per debug
			 glReadBuffer(GL_COLOR_ATTACHMENT0);
			 glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.50f, 1600 * 0.25f, 900 * 0.75f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

			 glReadBuffer(GL_COLOR_ATTACHMENT1);
			 glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.25f, 1600 * 0.25f, 900 * 0.5f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

			 glReadBuffer(GL_COLOR_ATTACHMENT2);
			 glBlitFramebuffer(0, 0, 1600, 900, 0, 0, 1600 * 0.25f, 900 * 0.25f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

			 gBuffer->unbind();

			 delta += 0.1f;
		}

		void onShutdown()
		{
		}

	private:
		Camera* camera;
		GBuffer* gBuffer;
		Transform transform;
		Exp::Model model;
		Technique geometryPass, lightingPass, skyboxPass;
};

int main(int _argc, char** _argv)
{
	RedFox::Engine engine;

	engine.initialize(_argc, _argv);
	engine.execute(new Sandbox);

	return 0;
}