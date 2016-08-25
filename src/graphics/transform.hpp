#pragma once

#include "../common.hpp"
#include "../core/nodegraph.hpp"

namespace RedFox
{
	class NodeComponent;

	//Classe che rappresenta una trasformazione e che crea la gerarchia della scena
	class Transf : public NodeComponent
	{
		 public:
			  Transf();

			  //Percorre l'albero e aggiorna le matrici se necessario
			  void bake(const mat4& _world, bool _dirty = true);

			  //Aggiunge un transform come figlio
			  void attachTransform(Transf* _transform);

			  //Rimuove un transform figlio
			  void detachTransform(Transf* _transform);

			  //Setters, richiedono il ricalcolo del world matrix al prossimo bake
			  void setPosition (const vec3& _position);
			  void setRotation (const vec3& _rotation);
			  void setScale	 (const vec3& _scale);

			  //Getters, non richiedono il ricalcolo del world matrix
			  inline const vec3& position() const { return m_position; }
			  inline const vec3& rotation() const { return m_rotation; }
			  inline const vec3& scale()    const { return m_scale;    }

			  //Ritorna la trasformazione salvata
			  inline mat4 world() const { return m_world; }

			  //Ritorna il transform iniziale
			  Transf* root();

			  //Ritorna parente di questo transform
			  inline Transf* parent() const { return m_parent; }

			  //Ritorna vettore coi figli di questo transform
			  inline auto& children() const { return m_children; }

		 private:
			  //Calcola la trasformazione locale
			  mat4 local() const;

		 private:
			  //Informazioni sulla trasformazione dell'oggetto
			  vec3 m_scale, m_position, m_rotation;

			  //Trasformazione salvata
			  mat4 m_world;

			  //Vera se m_world è deprecata
			  bool m_dirty;

			  //Transform che ha come figlio questo transform
			  Transf* m_parent;

			  //Transforms che dipendono da questo transform
			  vector<Transf*> m_children;
	};
};