// MyGame123.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Systems/PhysicsSystem.h"
#include "Game.h"
#include "MathHelperFunctions.h"

using namespace HelperFuncs;

void doColl(Entity* a, Entity* b)
{
	Physics* aPhys = &a->getComponent<Physics>(),
		*bPhys = &b->getComponent<Physics>();

	sf::Vector2f aVel = aPhys->velocity, bVel = bPhys->velocity,	// Velocity a, b
		aDir = normalize(aVel), bDir = normalize(bVel);					// Normalized velocity a, b
	float aMass = aPhys->mass, bMass = bPhys->mass;

	///	(1) Find directions of tangent |et| and normal |en|
	sf::Vector2f en = normalize(a->getPosition() - b->getPosition());
	sf::Vector2f et = sf::Vector2f(-en.x, en.y);

	/// (2) Express u1 and u2 in terms of en and et -> ui = (vi * en)*en + (vi * et)*et
	float v1en = multiplyVectors(aVel, en);
	float v1et = multiplyVectors(aVel, et);
	float v2en = multiplyVectors(bVel, en);
	float v2et = multiplyVectors(bVel, et);

	///	(3) Do 1D-collision in the direction en
	float v1 = ((aMass - bMass) * v1en + 2 * bMass * v2en) / (aMass + bMass);
	float v2 = ((bMass - aMass) * v2en + 2 * aMass * v1en) / (aMass + bMass);

	/// (4) Add up new velocity in the direction en with old velocity in direction et
	//sf::Vector2f aNewVel = scaleVector(v2, en) + scaleVector(v1et, et);
	//sf::Vector2f bNewVel = scaleVector(v1, en) + scaleVector(v2et, et);
	sf::Vector2f aNewVel = scaleVector(v2en, en) + scaleVector(v1et, et);
	sf::Vector2f bNewVel = scaleVector(v1en, en) + scaleVector(v2et, et);

	aPhys->velocity = aNewVel;
	bPhys->velocity = bNewVel;
}

int main(int argc, char *argv[])
{
	Entity a(1), b(2);
	a.setPosition(0, 0); b.setPosition(1, -1);

	Physics* aPhys = &a.addComponent<Physics>();
	Physics* bPhys = &b.addComponent<Physics>();

	aPhys->mass = 1, bPhys->mass = 1;
	aPhys->velocity = sf::Vector2f(3, 0);
	bPhys->velocity = sf::Vector2f(-1, 2);

	doColl(&a, &b);

	std::cout << "a velocity: " << aPhys->velocity.x << ", " << aPhys->velocity.y <<
		" --- b velocity: " << bPhys->velocity.x << ", " << bPhys->velocity.y << '\n';
	


	try
	{
		Game game;
		game.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}


	return 0;
}

