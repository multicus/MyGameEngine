#pragma once
#include "stdafx.h"
#include <memory>
#include <algorithm>
#include "Entity.h"
#include "Entity Factories/EntityFactory.h"

typedef std::unique_ptr<Entity> EntPtr;

//////////////////////	
//
//	TODO: Add a "pendingUpdate()" which checks if a new m_entity has been created
//		  or that an m_entity has gotten a new component. This is to reduce calls
//		  to the "getEntWithComps()" for each update();

class EntityManager
{
public:
	EntityManager();

	// Create a custom m_entity and add to list
	Entity* createEntity(sf::Vector2f position = sf::Vector2f(0, 0));
	//	Create a premade m_entity (preferred)
	Entity* createEntity(std::string ID, sf::Vector2f position = sf::Vector2f(0, 0));
	//EntPtr& addEntity(Entity* ent);			//	Add the m_entity from parameter to list
	bool removeEntity(unsigned int entID);		//	Delete the m_entity from the list
	Entity* getEntity(unsigned int entID);		//	Get m_entity by ID

	template<class T> std::vector<Entity*>	getEntWithComp();
	template<class T, class... params> std::vector<Entity*> getEntWithComps();
	std::vector<Entity*>						getAllEntities();

private:
	static std::vector<EntPtr> entities;		//	Only true holder of entities
	EntityFactory entityFactory;
	static unsigned int uniqueID;
};

template<class T> std::vector<Entity*> EntityManager::getEntWithComp()
{
	std::vector<Entity*> ents;

	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i]->hasComponent<T>())
		{
			//ent = std::make_shared<Entity>(entities[i].get());
			ents.push_back(entities[i].get());
		}
	}
	return ents;
}
template<class T, class... params> std::vector<Entity*> EntityManager::getEntWithComps()
{
	std::vector<Entity*> ents;

	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i]->hasComponents<T, params...>())
		{
			//ent = std::make_shared<Entity>(*entities[i]);
			ents.push_back(entities[i].get());
		}
	}
	return ents;
}

//	TODO:::


//	Disable entities (Useful for chunk loading!!)
//		You can then move the disabled enetities into the unloaded chunk class and load 
//		them fast when the chunk is loaded again