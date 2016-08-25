#include "renderer.hpp"

namespace RedFox
{
	 //Funzione invocata all'avvio dell'engine
	 void MeshRenderer::initialize()
	 {
		  m_gBuffer = new GBuffer(RFX_WINDOW_WIDTH, RFX_WINDOW_HEIGHT);
		  m_finalFrame = new Frame(RFX_WINDOW_WIDTH, RFX_WINDOW_HEIGHT);

		  m_geometryPass.initialize()
				.attach(Shader("deferred/geometry.vtx.glsl", GL_VERTEX_SHADER))
				.attach(Shader("deferred/geometry.frg.glsl", GL_FRAGMENT_SHADER))
				.link();

		  m_geometryPass.enable();
		  m_geometryPass.setUniform("material.albedo", 0u);

		  //====================================================================================

		  m_lightingPass.initialize()
				.attach(Shader("deferred/lighting.vtx.glsl", GL_VERTEX_SHADER))
				.attach(Shader("deferred/lighting.frg.glsl", GL_FRAGMENT_SHADER))
				.link();

		  m_lightingPass.enable();
		  m_lightingPass.setUniform("gbuffer.position", 0u);
		  m_lightingPass.setUniform("gbuffer.normal", 1u);
		  m_lightingPass.setUniform("gbuffer.color", 2u);

		  //====================================================================================

		  m_finalPass.initialize()
				.attach(Shader("skybox.vtx.glsl", GL_VERTEX_SHADER))
				.attach(Shader("skybox.frg.glsl", GL_FRAGMENT_SHADER))
				.link();

		  m_finalPass.enable();
		  m_finalPass.setUniform("skybox", 0u);
	 }

	 //Funzione invocata ogni frame
	 void MeshRenderer::update()
	 {
		  //Quadrato che copre tutto lo schermo
		  static Shape screen = Shape::Screen();

		  //Inizio rendering scena
		  m_gBuffer->bind(GL_DRAW_FRAMEBUFFER);
		  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		  m_geometryPass.enable();
		  m_geometryPass.setCamera(*m_camera);

		  for (const auto& component : m_components)
		  {
				m_geometryPass.setUniform("instance.model", component->transform->world());

				mat3 normalizer = glm::transpose(glm::inverse(mat3(component->transform->world())));
				m_geometryPass.setUniform("instance.normalizer", normalizer);

				component->mesh->render();
		  }

		  //Fine rendering scena
		  m_gBuffer->unbind();

		  m_lightingPass.enable();
		  m_lightingPass.setUniform("camera.position", m_camera->position);

		  //Invia ogni luce alla GPU
		  for (u32 i = 0; i < m_lights.size(); i++)
		  {
				char buffer[64];

				sprintf(buffer, "lights[%i].position", i);
				m_lightingPass.setUniform(buffer, m_lights[i]->transform->position());

				sprintf(buffer, "lights[%i].color", i);
				m_lightingPass.setUniform(buffer, m_lights[i]->color);
		  }

		  //Binda textures del gbuffer per il lighting pass e renderizza sul frame
		  m_gBuffer->bindTextures();
		  screen.render();

		  //===================================================================================================================
		  //Visualizza textures del gbuffer per debug

		  m_gBuffer->bind(GL_DRAW_FRAMEBUFFER);

		  //glReadBuffer(GL_COLOR_ATTACHMENT3);
		  //glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.75f, 1600 * 0.25f, 900, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		  glReadBuffer(GL_COLOR_ATTACHMENT2);
		  glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.50f, 1600 * 0.25f, 900 * 0.75f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		  glReadBuffer(GL_COLOR_ATTACHMENT1);
		  glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.25f, 1600 * 0.25f, 900 * 0.5f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		  glReadBuffer(GL_COLOR_ATTACHMENT0);
		  glBlitFramebuffer(0, 0, 1600, 900, 0, 0, 1600 * 0.25f, 900 * 0.25f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		  m_gBuffer->unbind();
	 }

	 //Setta la camera con cui verrà renderizzata la scena
	 void MeshRenderer::setCamera(Camera* _camera)
	 {
		  m_camera = _camera;
	 }

	 //==============================================================================
	 //Callbacks

	 //Callback invocato quando un componente viene assegnato ad un nodo
	 void MeshRenderer::onComponentCreation(Node* _node, NodeComponent* _component)
	 {
		  MeshComponent* mesh = dynamic_cast<MeshComponent*>(_component);
		  if(mesh != nullptr)
		  {
				//Aggiunge componente
				m_components.push_back(mesh);
				return;
		  }

		  LightComponent* light = dynamic_cast<LightComponent*>(_component);
		  if (light != nullptr)
		  {
				//Aggiunge componente
				m_lights.push_back(light);
				return;
		  }
	 }

	 //Callback invocato quando un componente viene eliminato da un nodo
	 void MeshRenderer::onComponentDeletion(Node* _node, NodeComponent* _component)
	 {
		  MeshComponent* mesh = dynamic_cast<MeshComponent*>(_component);
		  if (mesh != nullptr)
		  {
				//Rimuove componente
				std::remove(m_components.begin(), m_components.end(), mesh);
		  }

		  LightComponent* light = dynamic_cast<LightComponent*>(_component);
		  if (light != nullptr)
		  {
				//Rimuove componente
				std::remove(m_lights.begin(), m_lights.end(), light);
				return;
		  }
	 }
}