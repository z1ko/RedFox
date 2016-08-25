#pragma once

#include "../common.hpp"
#include "../core/events.hpp"
#include "../memory/pool.hpp"

namespace RedFox
{
	 class Node;
	 class Transf;

	 //Descrive come si comporta un nodo
	 class NodeComponent
	 {
		  public:
				NodeComponent();

				//Setta nodo a cui appartiene il componente
				virtual void attachTo(Node* _owner);

				//Ritorna nodo a cui appartiene il componente
				inline Node* node() const { return m_owner; }

		  protected:
				//Nodo a cui appartiene il componente
				Node* m_owner;
	 };

	 //Componente specializzato per il rendering, 
	 //contiene un pointer al transform nel nodo a cui appartiene, per comodità
	 class GraphicComponent : public NodeComponent
	 {
		  public:
				//Setta owner e ottiene transform
				virtual void attachTo(Node* _owner) override;

		  public:
				Transf* transform;
	 };

	 class Node
	 {
		  public:
				//Aggiunge un transform di base
				Node();
				Node(const str& _name);

				//Ritorna nome del nodo, per identificarlo
				inline str name() const { return m_name; }

				//Assegna un componente al nodo e informa l'engine
				template<class T, class ... Ts>
				T* assign(Ts&& ... args)
				{
					 //Alloca componente
					 T* component = new T(std::forward<Ts>(args)...);
					 m_components[typeid(T)] = component;

					 //Setta proprietario del componente
					 component->attachTo(this);

					 //Informa tutta l'engine
					 Events::ComponentCreation(this, component);

					 return component;
				};

				//Ritorna un componente se esiste
				template<class T>
				T* retrive()
				{
					 if (m_components.find(typeid(T)) != m_components.end())
						  return (T*)m_components.at(typeid(T));

					 return nullptr;
				}

				//Dealloca un componente e informa l'engine
				template<class T>
				void remove()
				{
					 if (m_components.find(typeid(T)) == m_components.end())
					 {
						  //Il componente non esiste
						  return;
					 }
					
					 //Trova componente
					 T* component = m_components.at(typeid(T));
					 m_components.erase(typeid(T));

					 //Informa tutta l'engine
					 Events::ComponentDeletion(this, component);

					 //Dealloca componente
					 delete component;
				}

		  public:
				//Transform di base
				Transf* transform;

		  private:
				//Nome del nodo
				str m_name;

				//Mappa che permette di ottenere i componenti di questo nodo
				umap<type_index, NodeComponent*> m_components;
	 };
	 
	 //Sistema che processa determinati nodi
	 class NodeSystem
	 {
		  public:
				//Collega eventi globali
				NodeSystem();

				//Funzione invocata all'avvio dell'engine
				virtual void initialize() = 0;

				//Funzione invocata ogni frame
				virtual void update() = 0;

		  protected:
				//Callback invocato quando un componente viene assegnato ad un nodo
				virtual void onComponentCreation(Node*, NodeComponent*) = 0;

				//Callback invocato quando un componente viene eliminato da un nodo
				virtual void onComponentDeletion(Node*, NodeComponent*) = 0;
	 };

	 namespace Events
	 {
		  extern Event<Node*, NodeComponent*> ComponentCreation, ComponentDeletion;
	 }
}