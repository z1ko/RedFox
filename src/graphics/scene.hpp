#pragma once

#include "transform.hpp"
#include "../graphics/deferred.hpp"

namespace RedFox
{
	 class SceneNode
	 {
		  public:
				SceneNode();
				virtual ~SceneNode();

				//Aggiunge un figlio a questo nodo
				void addChild(SceneNode* _node);

				//Registra questo nodo e i suoi figli nei sistema addetti al loro rendering,
				//aggiorna la trasformazione se necessario
				void update(const mat4& _world, bool _dirty, u32 _depth);

				//Ottiene un figlio tramite index
				SceneNode& operator[] (u32 _index);

				//Ritorna il numero di figli di questo nodo
				inline u32 size() const { return m_children.size(); }

				//Modifica il transform corrente
				void setTransform(const Transf& _transform);

				//Ritorna il transform corrente
				inline auto getTransform() const { return m_transform; }

				//Ritorna il matrix per la trasformazione
				inline mat4 getWorld() const { return m_world; }

				//Muove il nodo
				void translate(const vec3& _vector);
				void setPosition(const vec3& _vector);

				//Ruota il nodo
				void rotate(const vec3& _vector);
				void setRotation(const vec3& _vector);

				//Scala il nodo
				void scale(const vec3& _vector);
				void setScale(const vec3& _vector);

		  protected:
				//Registra oggetto nel renderer adeguato
				virtual void submit() = 0;

		  protected:
			    Transf m_transform;

				//transform del nodo padre
				mat4 m_world;
				bool m_dirty;

				vector<SceneNode*> m_children;
	 };

	 //Nodo che non si registra ad un sistema
	 class RootNode : public SceneNode
	 {
		  protected:
				void submit() { /*Vuoto*/ };
	 };

	 //Nodo vuoto, usato solo per raggruppare logicamente delle mesh
	 class ModelNode : public RootNode
	 {
		  public:
				//Carica modello da file
				ModelNode(const str& _filename);
	 };

	 class MeshNode : public SceneNode
	 {
		  public:
				//Crea mesh tramite assimp
				MeshNode(const aiMesh* _mesh);
				MeshNode(const aiMesh* _mesh, const aiMaterial* _material);
				virtual ~MeshNode();

		  protected:
				//Registra oggetto nel renderer adeguato
				void submit();

		  private:
				Mesh m_mesh;
	 };

	 class LightNode : public SceneNode
	 {
		  public:
				LightNode(const vec3& _color);

		  protected:
				//Registra oggetto nel renderer adeguato
				void submit();

		  public:
				vec3 color;
	 };
}