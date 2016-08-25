#include "transform.hpp"

namespace RedFox
{

	Transf::Transf()
		 : m_dirty(true), m_scale(1,1,1)
	{
	}

	//Calcola la trasformazione locale
	mat4 Transf::local() const
	{
		 mat4 result;

		 result  = glm::translate(result, m_position);
		 result *= glm::orientate4(m_rotation);
		 result  = glm::scale(result, m_scale);

		 return result;
	}

	//Percorre l'albero e aggiorna le matrici
	void Transf::bake(const mat4& _world, bool _dirty)
	{
		 //Aggiorna solo se necessario
		 _dirty != m_dirty;
		 if(m_dirty)
		 {
			  m_world = local() * _world;
			  m_dirty = false;
		 }

		 //Propaga funzione a tutti i figli
		 for (auto child : m_children) 
			  child->bake(m_world, m_dirty);
	}

	//Aggiunge un transform come figlio
	void Transf::attachTransform(Transf* _transform)
	{
		 m_children.push_back(_transform);
		 _transform->m_parent = this;
	}

	//Rimuove un transform figlio
	void Transf::detachTransform(Transf* _transform)
	{
		 if (std::remove(m_children.begin(), m_children.end(), _transform) != m_children.end())
			  _transform->m_parent = nullptr;
	}

	//Ritorna il transform iniziale
	Transf* Transf::root()
	{
		 if (m_parent == nullptr)
			  return this;
		 else
			  return m_parent->root();
	}

	//Getters and setters, richiedono il ricalcolo del world matrix al prossimo bake
	void Transf::setPosition(const vec3& _position)
	{
		 m_position = _position;
		 m_dirty = true;
	}

	void Transf::setRotation(const vec3& _rotation)
	{
		 m_rotation = _rotation;
		 m_dirty = true;
	}

	void Transf::setScale(const vec3& _scale)
	{
		 m_scale = _scale;
		 m_dirty = true;
	}
}