#include "deferred.hpp"

namespace RedFox
{
	GBuffer::GBuffer(u32 _width, u32 _height)
	{
		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

		glGenTextures(3, m_textures);

		//Genera texture per le posizione
		glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_POSITION]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[GBUFFER_POSITION], 0);

		//=================================================================================================================

		//Genera texture per le normali
		glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_NORMAL]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_textures[GBUFFER_NORMAL], 0);

		//=================================================================================================================

		//Genera texture per i colori
		glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_ALBEDO]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_textures[GBUFFER_ALBEDO], 0);

		//=================================================================================================================

		//Setta su quali textures avviene il rendering
		u32 attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
		  
		//=================================================================================================================

		//Crea il depth e stencil render buffer object
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//Applica rbo al gbuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		//=================================================================================================================

		//Controlla che il framebuffer sia stato creato correttamente
		u32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			exit(-1);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::bind(u32 mode) const
	{
		glBindFramebuffer(mode, m_handle);
	}

	void GBuffer::unbind() const
	{
		 glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::bindTextures() const
	{
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_POSITION]);

		 glActiveTexture(GL_TEXTURE1);
		 glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_NORMAL]);

		 glActiveTexture(GL_TEXTURE2);
		 glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_ALBEDO]);
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
						.attach(Shader("final.vtx.glsl", GL_VERTEX_SHADER))
						.attach(Shader("final.frg.glsl", GL_FRAGMENT_SHADER))
						.link();

		 m_finalPass.enable();
		 m_finalPass.setUniform("fram", 0u);

		 //====================================================================================

		 m_skyboxPass.initialize()
					    .attach(Shader("skybox.vtx.glsl", GL_VERTEX_SHADER))
					    .attach(Shader("skybox.frg.glsl", GL_FRAGMENT_SHADER))
					    .link();

		 m_skyboxPass.enable();
		 m_skyboxPass.setUniform("skybox", 0u);
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
		static Shape cube = Shape::Cube();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		m_skyboxPass.enable();

		//Elimina traslazione
		mat4 view = mat4(mat3(_camera.view()));
		m_skyboxPass.setUniform("camera.view", view);
		m_skyboxPass.setUniform("camera.proj", _camera.proj());

		m_skybox->bind();
		cube.render();

		glDepthFunc(GL_LESS);
	}

	//Esegue tutti i comandi
	void DeferredRenderer::present(const Camera& _camera)
	{
		//Per qualche oscuro motivo il depth testing non funziona piu

		 //Quadrato che copre tutto lo schermo
		 static Shape screen = Shape::Screen();

		 //Attiva il depth test, renderizzare gli oggetti in ordine di profondità
		 glEnable(GL_DEPTH_TEST);

		 //Inizio rendering scena
		 m_gBuffer.bind(GL_DRAW_FRAMEBUFFER);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

		 //Fine rendering scena e pulisce backbuffer principale
		 m_gBuffer.unbind();

		 //Resetta vettore comandi
		 m_meshCommands.clear();

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

		 //Renderizza tutte le luci
		 m_gBuffer.bindTextures();
		 screen.render();

		 //Copia il depth buffer
		 m_gBuffer.bind(GL_READ_FRAMEBUFFER);
		 glBlitFramebuffer(0, 0, RFX_WINDOW_WIDTH, RFX_WINDOW_HEIGHT, 0, 0, RFX_WINDOW_WIDTH, RFX_WINDOW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		 //Renderizza lo skybox
		 renderSkybox(_camera);

		 //===================================================================================================================
		 //Visualizza textures del gbuffer per debug

		 m_gBuffer.bind(GL_READ_FRAMEBUFFER);
		 glBindFramebuffer(GL_DRAW_BUFFER, 0);

		 glReadBuffer(GL_COLOR_ATTACHMENT2);
		 glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.50f, 1600 * 0.25f, 900 * 0.75f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		 glReadBuffer(GL_COLOR_ATTACHMENT1);
		 glBlitFramebuffer(0, 0, 1600, 900, 0, 900 * 0.25f, 1600 * 0.25f, 900 * 0.5f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		 glReadBuffer(GL_COLOR_ATTACHMENT0);
		 glBlitFramebuffer(0, 0, 1600, 900, 0, 0, 1600 * 0.25f, 900 * 0.25f, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		 m_gBuffer.unbind(); 

		 //Breakpoint
		 glColor3f(0.0f, 0.0f, 0.0f);
	}
}