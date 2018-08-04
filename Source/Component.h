#pragma once
#include "stdafx.h"
#include <cassert>

struct BaseComponent
{
	//	Need a virtual function in base class to be able to use dynamic_cast
	virtual ~BaseComponent() {};
};

struct Transform : BaseComponent	//	Position, rotation, scale
{
	sf::Transformable transform;
};

struct Physics : BaseComponent		//	Making an object fall
{
	float gravity = 200.0f;			//	Default gravity (Disable for e.g. bullets)
	sf::Vector2f velocity;

	const float maxFallingSpeed = 200.0f;
};

struct Collider : BaseComponent		//	Collides with other objects
{
	sf::Rect<float> colliderBox;	//	Simply a box around the entity sprite
	bool isTrigger = false;			//	If true, this collider works as a trigger instead
};

///	Requires : MovementComponent
struct Controller : BaseComponent	
{
	
};

///	Requires : PhysicsComponent
struct Movement : BaseComponent		//	Can be applied to NPC's as well
{
	float horizontalSpeed = 30;
	//float runSpeed = 50;
	float jumpForce = 200;
};

struct Sprite2D : BaseComponent
{
	enum Layer
	{
		LAYER_BACK,
		LAYER_FRONT,
		LAYER_SIZE
	};

	//Sprite2D() {}
	//Sprite2D(sf::Texture tex, sf::Sprite spr, Layer lay = LAYER_FRONT)
	//	: texture{ tex }, sprite{ spr }, layer{ lay }
	//{
	//}

	Layer layer;
	sf::Texture texture;
	sf::Sprite sprite;
};