#pragma once

#include "../graphics/technique.hpp"
#include "../graphics/transform.hpp"

//Implementazione semplice, efficace e poco performate(MOLTO POCO)
namespace RedFox
{
	class Entity;

	class Component
	{
		public:
			inline void attachTo(Entity* _parent)
			{
				m_parent = _parent;
			}

			virtual void update() { };
			virtual void render() { };

		protected:
			Entity* m_parent;
	};
	
	class Entity
	{
		public:
			Entity();
			virtual ~Entity();

			void addComponent(Component* _component);

			void update();
			void render(StandardTechnique* _technique);

		public:
			Transform transform;

		private:
			vector<Component*> m_components;
	};
};