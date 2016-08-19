#pragma once

#include "../common.hpp"
#include "../memory/pool.hpp"

namespace RedFox
{
	 class NodeDescription;

	 //Descrive come si comporta un nodo
	 class NodeComponent
	 {
	 };

	 //Rappresenta una singola unità logica (wrapper)
	 class Node
	 {
		  friend class Scene;

		  public:
				//Crea nodo fasullo
				Node();

				//Distrugge il nodo e tutti i suoi figli
				void destroy();

		  private:
				//Solo la scena o un altro nodo puo costruire un nodo
				Node(Scene* _scene, const Key<NodeDescription>& _key);

		  public:
				//Crea un figlio
				Node populate();

				//Ritorna il parente di questo nodo
				Node& parent();

				//Ritorna il vettore contenete i figli di questo nodo
				auto& children();

				//Controlla se questo nodo è figlio (Non recursivo)
				bool isChildOf(const Node& _node);

				//Controlla se esiste questo figlio (Non recursivo)
				bool isParentOf(const Node& _node);

				//Operatori
				bool operator == (const Node& _other) const;

				//Assegna un componente
				template<class T, class ... Ts>
				void assign(Ts&& ... args)
				{
				}

				//Rimuove un componente
				template<class T>
				void remove()
				{
				}

		  private:
				//Aggiunge un figlio
				void attach(Node& _child);

				//Rimuove un figlio di prima generazione
				void detach(const Node& _child);

		  private:
				//Oggetto che dirigge questo nodo
				Scene* m_scene;

				//Premette di accedere alla descrizione di questo nodo
				Key<NodeDescription> m_key;
	 };

	 //Descrive una determinata unità logica
	 struct NodeDescription
	 {
		  //Nodo di cui questo nodo è figlio
		  Node parent;

		  //Figli di questo nodo
		  vector<Node> children;
	 };

	 //Classe che crea, dirigge e distrugge i nodi
	 class Scene
	 {
		  friend class Node;

		  public:
				Scene();

				//Crea un nuovo nodo figlio della root
				Node create();

		  private:
				//Nodo di base a cui sono collegati tutti i nuovi oggetti
				Node m_root;

				//Descrive tutti i nodi
				Pool<NodeDescription> m_descriptions;
	 };
}