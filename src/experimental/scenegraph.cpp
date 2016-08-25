#include "scenegraph.hpp"

namespace RedFox { namespace Exp {

	Transform::Transform()
		: parent_(nullptr), scale_(1.0f)
	{
	}

	//Fa dipendere la trasformazione dal parente
	void Transform::attachTo(Transform* parent)
	{
		if (parent_ != nullptr)
		{
			//Rimuove figlio dal parente precedente
			auto& children = parent_->children_;
			children.erase(std::remove(children.begin(), children.end(), this), children.end());
		}

		//Aggiunge figlio
		if (parent != nullptr)
			parent->children_.push_back(this);

		parent_ = parent;
	}

	//Aggiorna la trasformazione di tutti i figli in relazione a quella di questo transform
	void Transform::bake(const mat4& world, bool dirty)
	{
		//Aggiorna il transform solo se necessario
		if ((dirty_ |= dirty) == true)
		{
			dirty_ = false;
		}

		//Propaga a l'operazione tutti i figli
		for (auto child : children_)
			child->bake(world_, dirty);
	}

#pragma region getters_setters

	//==================================================================================================
	//Getters e setters vari

	//Setta nodo che possiede questo transform
	void Transform::setOwner(Node* node)
	{
		node_ = node;
	}

	//Aggiunge traslazione
	void Transform::translate(const vec3& position)
	{
		position_ += position;
		dirty_ = true;
	}

	void Transform::setPosition(const vec3& position)
	{
		position_ = position;
		dirty_ = true;
	}

	vec3 Transform::getPosition() const
	{
		return position_;
	}

	//Aggiunge rotazione
	void Transform::rotate(const vec3& rotation)
	{
		rotation_ += rotation;
		dirty_ = true;
	}

	void Transform::setRotation(const vec3& rotation)
	{
		rotation_ = rotation;
		dirty_ = true;
	}

	vec3 Transform::getRotation() const
	{
		return rotation_;
	}

	//Aggiunge scaling
	void Transform::scale(const vec3& scale)
	{
		scale_ += scale;
		dirty_ = true;
	}

	void Transform::setScale(const vec3& scale)
	{
		scale_ = scale;
		dirty_ = true;
	}

	vec3 Transform::getScale() const
	{
		return scale_;
	}

#pragma endregion

	//==================================================================================================
	//Scena

	Node::Node()
	{
		transform_.setOwner(this);

		for (u32 i = 0; i < 64; ++i)
			components_[i] = nullptr;
	}

	NodePtr::NodePtr()
		: pool_(nullptr), key_(UINT_MAX, UINT_MAX)
	{
	}

	NodePtr::NodePtr(Pool<Node, 1024>* pool, Key<Node> key)
		: pool_(pool), key_(key)
	{
	}

	//Crea un nuovo nodo vuoto
	NodePtr Scene::instantiate()
	{
		Key<Node> key = nodes_.emplace();
		return NodePtr(&nodes_, key);
	}

	//Elimina un nodo, se ha come componente un transform elimina anche tutti i nodi figli
	void Scene::kill(const NodePtr& node)
	{
		//TODO
		nodes_.remove(node.key_);
	}

} }