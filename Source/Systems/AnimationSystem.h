#pragma once
#include "stdafx.h"
#include "System.h"
#include <map>
#include <memory>

class AnimationSystem : public System
{
	//	Structure/TODO:
	//	- Holds all animations (Load them from folder) possible
	//	- Tag them by string name or enum? (Use map to bind them?)
	//	- 



public:
								AnimationSystem();

	void						configure(EventManager& events) override;
	void						update(float dt, EventManager& events) override;

	void						receiver(Action* action);
	//	Get notified from subscriptions
	void						onNotify(int entity, EventID event) override;

private:
	//Animation*					getAnimationFromTrigger()
	void						flipTexture(sf::Texture& texture);

private:
	EntityManager				entMan;

};
