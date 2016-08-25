#pragma once

#include "../core/nodegraph.hpp"
#include "../graphics/model.hpp"
#include "../graphics/deferred.hpp"

namespace RedFox
{
	 //Rappresenta un instanza di una mesh
	 class MeshComponent : public GraphicComponent
	 {
		  public:
				MeshComponent()
					 : mesh(nullptr)
				{
				}

				MeshComponent(Mesh* _mesh)
					 : mesh(_mesh)
				{
				}

		  public:
				Mesh* mesh;
	 };

	 class LightComponent : public GraphicComponent
	 {
		  public:
				LightComponent()
					 : color(0,0,0)
				{
				}

				LightComponent(const vec3& _color)
					 : color(_color)
				{
				}

		  public:
				vec3 color;
	 };

	 //Class dedicata a renderizzare solo le mesh
	 class MeshRenderer : public NodeSystem
	 {
		  public:
				//Funzione invocata all'avvio dell'engine
				void initialize();

				//Funzione invocata ogni frame
				void update();

				//Setta la camera con cui verrà renderizzata la scena
				void setCamera(Camera* _camera);

		  protected:
				//Callback invocato quando un componente viene assegnato ad un nodo
				void onComponentCreation(Node*, NodeComponent*);

				//Callback invocato quando un componente viene eliminato da un nodo
				void onComponentDeletion(Node*, NodeComponent*);

		  private:
				GBuffer* m_gBuffer;
				Technique m_geometryPass, m_lightingPass, m_finalPass;
				Frame* m_finalFrame;
				CubeMap* m_skybox;
				Camera* m_camera;

				vector<MeshComponent*> m_components;
				vector<LightComponent*> m_lights;
	 };
}