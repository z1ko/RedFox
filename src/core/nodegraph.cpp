#include "nodegraph.hpp"

namespace RedFox
{
	 //Crea nodo fasullo
	 Node::Node()
		  : m_scene(nullptr)
	 {
	 }

	 //Solo la scena o un altro nodo puo costruire un nodo
	 Node::Node(Scene* _scene, const Key<NodeDescription>& _key)
		  : m_scene(_scene), m_key(_key)
	 {
	 }

	 //Operatori
	 bool Node::operator == (const Node& _other) const
	 {
		  return m_key.index == _other.m_key.index
				&& m_key.generation == _other.m_key.generation;
	 }

	 //Crea un figlio
	 Node Node::populate()
	 {
		  //Crea un nuovo nodo
		  Node child = m_scene->create();
		  this->attach(child);

		  return child;
	 }

	 //Ritorna il parente di questo nodo
	 Node& Node::parent()
	 {
		  return m_scene->m_descriptions[m_key]->parent;
	 }

	 //Ritorna il vettore contenete i figli di questo nodo
	 auto& Node::children()
	 {
		  return m_scene->m_descriptions[m_key]->children;
	 }

	 //Controlla se questo nodo è figlio (Non recursivo)
	 bool Node::isChildOf(const Node& _node)
	 {
		  return _node == this->parent();
	 }

	 //Controlla se esiste questo figlio (Non recursivo)
	 bool Node::isParentOf(const Node& _node)
	 {
		  for (const auto& child : children())
				if (child == _node) return true;

		  return false;
	 }

	 //Rimuove un figlio di prima generazione
	 void Node::detach(const Node& _child)
	 {
		  auto& children = this->children();
		  for(u32 i = 0; i < children.size(); i++)
		  {
				if (children[i] == _child)
					 children.erase(children.begin() + i);
		  }
	 }

	 //Aggiunge un figlio
	 void Node::attach(Node& _child)
	 {
		  _child.parent().detach(_child);

		  _child.parent() = *this;
		  children().push_back(_child);
	 }

	 //Distrugge il nodo e tutti i suoi figli
	 void Node::destroy()
	 {
		  this->parent().detach(*this);

		  for (auto& child : children())
				child.destroy();

		  m_scene->m_descriptions.remove(m_key);
	 }

	 //=============================================================================

	 Scene::Scene()
	 {
		  m_root = Node(this, m_descriptions.emplace());
	 }

	 //Crea un nuovo nodo figlio della root
	 Node Scene::create()
	 {
		  //Trova un nuovo descrittore libero e lo usa per questo nuovo nodo
		  Key<NodeDescription> key = m_descriptions.emplace();
		  Node result = Node(this, key);

		  //Registra il nodo
		  result.parent() = m_root;
		  m_root.children().push_back(result);

		  return Node(this, key);
	 }
}