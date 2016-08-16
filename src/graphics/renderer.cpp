#include "renderer.hpp"

#include "shape.hpp"

namespace RedFox
{
	//===============================================================================================================

	RenderCommand::RenderCommand() 
		: technique(0), material(0), model(0), transform(nullptr)
	{
	}

	//TODO(?): Sposta il transform
	RenderCommand::RenderCommand(u32 _technique, u32 _material, u32 _model, Transform* _transform)
		: technique(_technique), material(_material), model(_model), transform(_transform)
	{
	}

	//Permette di organizzare i comandi in modo da limitare gli switch nel contesto
	bool RenderCommand::operator < (const RenderCommand& _other) const
	{
		return (technique < _other.technique || material < _other.material || model < _other.model);
	}

	//===============================================================================================================

	Renderer::Renderer()
	{
	}

	//Aggiunge un post process
	//TODO: Permetti di usare diverse stack di tecniche
	void Renderer::setPostProcess(PostProcess* _process)
	{
		m_postProcess.reset(_process);
	}

	//Setta quale cubemap verrà usata per la skymap
	void Renderer::setSkyBox(CubeMap* _cubeMap)
	{
		m_skyBox.reset(_cubeMap);
	}

	//Aggiunge un commando che verrà eseguito all'invocazione di flush
	void Renderer::submitCommand(u32 _technique, u32 _material, u32 _model, Transform* _transform)
	{
		//TODO(?): Usa batches 
		m_commands.emplace_back(_technique, _material, _model, _transform);
	}

	void Renderer::submitCommand(const RenderCommand& _command)
	{
		//TODO(?): Usa batches 
		m_commands.push_back(_command);
	}

	void Renderer::renderSkyBox(const Camera& _camera)
	{
		static SkyBoxTechnique technique;
		static Shape quad = Shape::Quad();

		//Toglie la traslazione dalla matrice della telecamera
		mat4 view = mat4(mat3(_camera.view()));

		//Senza depth mask lo skybox viene renderizzato dietro tutti gli oggetti
		glDepthMask(GL_FALSE);

		technique.enable();
		technique.setUniform("camera.view", view);
		technique.setUniform("camera.proj", _camera.proj());

		m_skyBox->bind();
		quad.render();

		glDepthMask(GL_TRUE);
	}

	//Sorta ed esegue tutti i comandi
	void Renderer::present(const Camera& _camera)
	{
		static Frame renderingFrame(1600, 900); //TODO: Permetti di cambiare dimensioni schermo
		static FrameTechnique finalTechnique;

		//Ultima tecnica usata, questo pointer serve a settare le uniform
		static Technique* lastTechnique = nullptr;

		//Tecnica <- Materiale <- Modello (Dal più al meno importante)
		std::sort(m_commands.begin(), m_commands.end());

		renderingFrame.enable();

		//Renderizza lo skybox sul fondo della scena
		//if(m_skyBox) renderSkyBox(_camera);

		for(const auto& command : m_commands)
		{
			//Cambia la tecnica solo se necessario
			static u32 lastTechniqueIndex = UINT_MAX;
			if (command.technique != lastTechniqueIndex || lastTechnique == nullptr)
			{
				//Trova tecnica usando l'index specificato nel comando
				lastTechnique = Database::retrive<Technique>(command.technique);
				lastTechnique->enable();

				//Setta camera
				lastTechnique->setCamera(_camera);

				lastTechniqueIndex = command.technique;
			}

			//Cambia materiale solo se necessario
			//static u32 lastMaterialIndex = UINT_MAX;
			//if(command.material != lastMaterialIndex)
			//{
			//	//Trova materiale usando l'index specificato nel comando
			//	Material* material = Database::retrive<Material>(command.material);

			//	//TODO: Trova un modo piu generico di settare i materiali
			//	material->albedo.bind(0);

			//	lastMaterialIndex = command.material;
			//}

			//TODO(?): Se il modello è uguale nei comandi successivi allora renderizza ad istanze
			lastTechnique->setTransform(*command.transform);

			//Trova modello usando l'index specificato nel comando
			Model* model = Database::retrive<Model>(command.model);
			model->render();
		}
		renderingFrame.disable();

		//Quadrato 1x1 che copre tutto lo schermo
		static const Shape screen = Shape::Screen();

		//Esegue tutti gli effetti post produzione
		static Frame postProcessFrame(1600, 900);

		if(m_postProcess)
		{
			postProcessFrame.enable();
			{
				m_postProcess->enable();
				renderingFrame.bind();
				screen.render();
			}
			postProcessFrame.disable();

			//Renderizza frame finale
			finalTechnique.enable();
			postProcessFrame.bind();
			screen.render();
		}
		else
		{
			//Renderizza frame finale
			finalTechnique.enable();
			renderingFrame.bind();
			screen.render();
		}

		//Resetta shader altrimenti può capitare che la scena venga renderizzata con la tecnica finale
		lastTechnique = nullptr;

		//Resetta comandi
		m_commands.clear();
	}
}