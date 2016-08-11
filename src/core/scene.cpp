#include "scene.hpp"

namespace RedFox
{
	//Contatore statico che da un ID unico ad ogni entità
	static u32 lastNodeID = 0;

	Node::Node(const string& _name)
		: m_name(_name), m_id(lastNodeID++)
	{
	}

	//Ricerca ricorsiva
	Node* Node::findChild(const string& _name)
	{
		//Prima controlla tutti i figli diretti
		for(auto& child : m_children)
		{
			if (child.m_name == _name)
				return &child;
		}

		//Cerca in tutto l'albero
		for(auto& child : m_children)
		{
			Node* result = child.findChild(_name);
			if (result != nullptr) return result;
		}

		//Ritorna null se non esiste un nodo con quel nome fra i figli
		return nullptr;
	}
}