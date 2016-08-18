#pragma once

#include "../common.hpp"
#include "../resources/resources.hpp"

#include "../graphics/frame.hpp"
#include "../graphics/model.hpp"
#include "../graphics/camera.hpp"
#include "../graphics/cubemap.hpp"
#include "../graphics/postprocess.hpp"

namespace RedFox
{
	class RenderCommand
	{
		public:
			//TODO(?): Sposta il transform
			RenderCommand(Key<Technique> _technique, Key<Material> _material, Key<Model> _model, Transform* _transform);
			RenderCommand();

			//Permette di organizzare i comandi in modo da limitare gli switch nel contesto
			bool operator <(const RenderCommand& _other) const;

		public:
			 Key<Technique> technique;
			 Key<Material> material;
			 Key<Model> model;

			 Transform* transform;
	}; 

	class Renderer
	{
		public:
			Renderer();

			//Aggiunge un post process
			//TODO: Permetti di usare diverse stack di tecniche
			void setPostProcess(PostProcess* _process);

			//Setta quale cubemap verrà usata per la skymap
			void setSkyBox(CubeMap* _cubeMap);

			//Aggiunge un commando che verrà eseguito all'invocazione di flush
			void submitCommand(Key<Technique> _technique, Key<Material> _material, Key<Model> _model, Transform* _transform);
			void submitCommand(const RenderCommand& _command);

			//Sorta ed esegue tutti i comandi
			//TODO(?): Inoltra camera con questo metodo 
			void present(const Camera& _camera);

		private:
			void renderSkyBox(const Camera& _camera);

		private:
			vector<RenderCommand> m_commands;
			unique<PostProcess> m_postProcess;
			unique<CubeMap> m_skyBox;
	};
}