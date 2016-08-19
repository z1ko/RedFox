#include "deferred.hpp"

namespace RedFox
{
	GBuffer::GBuffer(u32 _width, u32 _height)
	{
		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);

		//Genera texture per le posizioni
		glGenTextures(1, &m_position);
		glBindTexture(GL_TEXTURE_2D, m_position);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_position, 0);

		//Genera texture per le normali
		glGenTextures(1, &m_normal);
		glBindTexture(GL_TEXTURE_2D, m_normal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normal, 0);

		//Genera texture per i colori
		glGenTextures(1, &m_color);
		glBindTexture(GL_TEXTURE_2D, m_color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_color, 0);

		//Genera texture per la profondità
		glGenTextures(1, &m_depth);
		glBindTexture(GL_TEXTURE_2D, m_depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_depth, 0);

		//Setta su quali textures avviene il rendering
		u32 attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);

		//Crea render buffer per la depth map e la stencil map
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//Assegna il renderbuffer al framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		//Controlla che il framebuffer sia stato creato correttamente
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			exit(-1);
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void GBuffer::bindForWriting() const
	{
		 glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);
	}

	void GBuffer::bindForReading() const
	{
		 glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
	}

	void GBuffer::unbind() const
	{
		 glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::bindTextures() const
	{
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, m_position);

		 glActiveTexture(GL_TEXTURE1);
		 glBindTexture(GL_TEXTURE_2D, m_normal);

		 glActiveTexture(GL_TEXTURE2);
		 glBindTexture(GL_TEXTURE_2D, m_color);

		 glActiveTexture(GL_TEXTURE3);
		 glBindTexture(GL_TEXTURE_2D, m_depth);
	}

	//====================================================================================================================

	MeshRenderCommand::MeshRenderCommand(Mesh* _mesh, mat4 _transformation)
		 : mesh(_mesh), transformation(_transformation)
	{
	}

	//====================================================================================================================

	//Carica tecnica
	DeferredRenderer::DeferredRenderer()
		 : m_gBuffer(RFX_WINDOW_WIDTH, RFX_WINDOW_HEIGHT), m_finalFrame(RFX_WINDOW_WIDTH, RFX_WINDOW_HEIGHT)
	{
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

	//Crea ed aggiunge comando per renderizzare una mesh
	void DeferredRenderer::submitMeshCommand(Mesh* _mesh, mat4 _transformation)
	{
		 m_meshCommands.emplace_back(_mesh, _transformation);
	}

	//Crea ed aggiunge comando per una luce
	void DeferredRenderer::submitLightCommand(const Light& _light)
	{
		 m_lights.emplace_back(_light);
	}

	//Setta lo skybox della scena
	void DeferredRenderer::setSkybox(CubeMap* _skybox)
	{
		 m_skybox = _skybox;
	}

	//Renderizza lo skybox sul fondo della scena
	void DeferredRenderer::renderSkybox(const Camera& _camera)
	{
		 //Cambia funzione del depth test, per non sovrascrivere l'immagine già presente nel frame
		 glDepthFunc(GL_LEQUAL);
		 glDisable(GL_CULL_FACE);

		 m_finalPass.enable();

		 //Elimina la traslazione e setta gli uniforms
		 m_finalPass.setUniform("camera.view", mat4(mat3(_camera.view())));
		 m_finalPass.setUniform("camera.proj", _camera.proj());

		 //Usa lo skybox
		 m_skybox->bind();

		 //Cubo dello skybox
		 static Shape cube = Shape::Cube();
		 cube.render();

		 //Ritorna alla funzione standard
		 glDepthFunc(GL_LESS);
		 glEnable(GL_CULL_FACE);
	}

	//Esegue tutti i comandi
	void DeferredRenderer::present(const Camera& _camera)
	{
		 //Quadrato che copre tutto lo schermo
		 static Shape screen = Shape::Screen();

		 //Inizio rendering scena
		 m_gBuffer.bindForWriting();
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		 m_geometryPass.enable();
		 m_geometryPass.setCamera(_camera);
		 
		 //Ordina tutti i comandi per minimizzare i cambi di contesto
		 //std::sort(m_meshCommands.begin(), m_meshCommands.end());

		 for(const auto& command : m_meshCommands)
		 {
			  m_geometryPass.setUniform("instance.model", command.transformation);

			  mat3 normalizer = glm::transpose(glm::inverse(mat3(command.transformation)));
			  m_geometryPass.setUniform("instance.normalizer", normalizer);

			  command.mesh->render();
		 }

		 //Fine rendering scena
		 m_gBuffer.unbind();

		 //Resetta vettore comandi
		 m_meshCommands.clear();

		 //Renderizza luci sul frame finale
		 //m_finalFrame.enable();
		 //glClear(GL_COLOR_BUFFER_BIT);

		 m_lightingPass.enable();
		 m_lightingPass.setUniform("camera.position", _camera.position);

		 //Invia ogni luce alla GPU
		 for(u32 i = 0; i < m_lights.size(); i++)
		 {
			  char buffer[64];

			  sprintf(buffer, "lights[%i].position", i);
			  m_lightingPass.setUniform(buffer, m_lights[i].position);

			  sprintf(buffer, "lights[%i].color", i);
			  m_lightingPass.setUniform(buffer, m_lights[i].color);
		 }

		 //Resetta vettore luci
		 m_lights.clear();

		 //Binda textures del gbuffer per il lighting pass e renderizza sul frame
		 m_gBuffer.bindTextures();
		 screen.render();

		 //Finisce di renderizzare luci
		 //m_finalFrame.disable();
		 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		 //Renderizza sul backbuffer

		 //Cambia funzione del depth test, per non sovrascrivere l'immagine già presente nel frame
		 //glDepthFunc(GL_LEQUAL);
		 //glDisable(GL_CULL_FACE);

		 //m_finalPass.enable();

		 //Elimina la traslazione e setta gli uniforms
		 //m_finalPass.setUniform("camera.view", mat4(mat3(_camera.view())));
		 //m_finalPass.setUniform("camera.proj", _camera.proj());

		 //Usa lo skybox
		 //m_skybox->bind();

		 //Cubo dello skybox
		 //static Shape cube = Shape::Cube();
		 //cube.render();

		 //Ritorna alla funzione standard
		 //glDepthFunc(GL_LESS);
		 //glEnable(GL_CULL_FACE);

		 //===================================================================================================================
		 //Visualizza textures del gbuffer per debug

		 m_gBuffer.bindForReading();

		 //glReadBuffer(GL_COLOR_ATTACHMENT3);
		 //glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.75f, 1600 * 0.25f, 900, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		 glReadBuffer(GL_COLOR_ATTACHMENT2);
		 glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.50f, 1600 * 0.25f, 900 * 0.75f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		 glReadBuffer(GL_COLOR_ATTACHMENT1);
		 glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.25f, 1600 * 0.25f, 900 * 0.5f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		 glReadBuffer(GL_COLOR_ATTACHMENT0);
		 glBlitFramebuffer(0, 0, 1600, 900, 0, 0, 1600 * 0.25f, 900 * 0.25f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		 m_gBuffer.unbind();
	}
}