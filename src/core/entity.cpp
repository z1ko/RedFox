#include "entity.hpp"

namespace RedFox
{
	Entity::Entity()
	{
	}

	Entity::~Entity()
	{
		for (Component* component : m_components)
			delete component;
	}

	void Entity::addComponent(Component* _component)
	{
		m_components.push_back(_component);
	}

	void Entity::update()
	{
		for (Component* component : m_components)
			component->update();
	}

	void Entity::render(StandardTechnique* _technique)
	{
		_technique->setTransform(transform);
		for (Component* component : m_components)
			component->render();
	}
}