#include "nodegraph.hpp"

#include "../core/engine.hpp"
#include "../graphics/transform.hpp"

namespace RedFox
{
	 NodeComponent::NodeComponent()
		  : m_owner(nullptr)
	 {
	 }

	 //Setta nodo a cui appartiene il componente
	 void NodeComponent::attachTo(Node* _owner)
	 {
		  m_owner = _owner;
	 }

	 //==========================================================================

	 //Setta owner e ottiene transform
	 void GraphicComponent::attachTo(Node* _owner)
	 {
		  m_owner = _owner;
		  transform = _owner->transform;
	 }

	 //==========================================================================

	 Node::Node()
	 {
		  //Aggiunge un transform di base
		  transform = this->assign<Transf>();
	 }

	 Node::Node(const str& _name)
		  : m_name(_name)
	 {
		  //Aggiunge un transform di base
		  transform = this->assign<Transf>();
	 }

	 //==========================================================================

	 NodeSystem::NodeSystem()
	 {
		  RFX_CONNECT(NodeSystem, onComponentCreation, Events::ComponentCreation);
		  RFX_CONNECT(NodeSystem, onComponentDeletion, Events::ComponentDeletion);
	 }

	 //==========================================================================

	 namespace Events
	 {
		  Event<Node*, NodeComponent*> ComponentCreation, ComponentDeletion;
	 }
}