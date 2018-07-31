#pragma once
#include "stdafx.h"
#include "System.h"
#include <vector>
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include <memory>

enum class SystemEvent
{
	ENTITY_UPDATE,
};

//	Responsible to automatically add entities to the systems
class SystemManager
{
public:
	typedef std::unique_ptr<System> SysPtr;

public:
								SystemManager(/*sf::RenderWindow& window*/);

	void						init();
	void						begin();
	void						update(float dt);
	void						end();

	void						notify(Entity* entity, SystemEvent event);

public:
	template<typename T> T&	addSystem()
	{
		SysPtr newSys(new T());
		systems.push_back(std::move(newSys));
		return *((T*)systems.back().get());
	}

	template<typename T> T& getSystem() const
	{
		T* it = NULL;

		for (auto& system : systems)
		{
			it = dynamic_cast<T*>(system.get());
			if (it != NULL)
				break;
		}
		assert(it != NULL);

		return *it;
	}

private:
	//sf::RenderWindow& window;
	std::vector<std::unique_ptr<System>> systems;
};