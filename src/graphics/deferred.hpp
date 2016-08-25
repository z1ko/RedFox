#pragma once

#include "../common.hpp"
#include "../meta/singleton.hpp"
#include "../graphics/model.hpp"
#include "../graphics/frame.hpp"
#include "../graphics/cubemap.hpp"

namespace RedFox
{
	class GBuffer
	{
		public:
			 enum GBUFFER_TEXTURES
			 {
				  GBUFFER_POSITION, 
				  GBUFFER_NORMAL,
				  GBUFFER_ALBEDO
			 };

		public:
			GBuffer(u32 _width, u32 _height);

			void bind(u32 mode) const;
			void unbind() const;

			void bindTextures() const;

		private:
			u32 m_handle, m_rbo;
			u32 m_textures[3];
	};

	class MeshRenderCommand
	{
		 public:
			  MeshRenderCommand(Mesh* _mesh, mat4 _transformation);

		 public:
			  mat4  transformation;
			  Mesh* mesh;
	};

	struct Light
	{
		 vec3 position;
		 vec3 color;
	};

	//Deferred Mesh Renderer
	class DeferredRenderer : public Singleton<DeferredRenderer>
	{
		public:
			//Carica tecnica
			DeferredRenderer();

			//Crea ed aggiunge comando per renderizzare una mesh
			void submitMeshCommand(Mesh* _mesh, mat4 _transformation);

			//Crea ed aggiunge comando per una luce
			void submitLightCommand(const Light& _light);
		  
			//Setta lo skybox della scena
			void setSkybox(CubeMap* _skybox);

			//Esegue tutti i comandi
			void present(const Camera& _camera);

		private:
			 //Renderizza lo skybox sul fondo della scena
			 void renderSkybox(const Camera& _camera);

		private:
			GBuffer m_gBuffer;
			Technique m_geometryPass, m_lightingPass, m_finalPass, m_skyboxPass;

			Frame m_finalFrame;
			RenderTarget finalTarget_;

			CubeMap* m_skybox;

			vector<MeshRenderCommand> m_meshCommands;
			vector<Light> m_lights;
	};
}