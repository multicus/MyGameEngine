#include "stdafx.h"
#include "Entity.h"
#include "Components/Sprite2D.h"
#include "Components/Physics/Physics.h"
#include "Components/Controller.h"
#include "Systems/SystemManager.h"
#include "Entity Factories/EntityFactory.h"

World::World(sf::RenderWindow& window)
	: mWindow{ window }
	//, mSceneGraph{}
	//, mSceneLayers{}
	, textureHolder{}
	, uniqueEntID{ 0 }
	, systemManager{ new SystemManager(/*window*/) }
{
	loadTextures();
	buildScene();
}

void World::addEntity(std::unique_ptr<Entity> entity)
{
	//std::cout << "World 1: " << entity.get()->getComponent<Transform>().transform.getPosition().y << '\n';

	//	Notify the system about changes
	systemManager->notify(entity.get(), SystemEvent::ENTITY_UPDATE);

	//std::cout << "World 2: " << entity.get()->getComponent<Transform>().transform.getPosition().y << '\n';

	//	Add to list
	entities.push_back(std::move(entity));
}

int World::getUniqueID()
{
	return uniqueEntID++;
}

void World::update(float dt)
{
	systemManager->update(dt);
	//	Rather update individually in systems
	//mSceneGraph.update(dt);	//	Make systems responsible for update 
}

void World::draw()
{
	//	Temporary
	systemManager->getSystem<RenderSystem>().draw(mWindow);
}

sf::Texture World::getTexture(std::string name) const
{
	return textureHolder.get(name);
}

void World::loadTextures()
{
	textureHolder.load("Raptor", "Media/Textures/Raptor.png");
	textureHolder.load("Ground", "Media/Textures/Ground.png");
}

void World::buildScene()
{
	EntityFactory factory(this);

	factory.createPlayer();
	factory.createGround();

	//for (int i = 0; i < Layer::LayerSize; i++)
	//{
	//	Entity::EntPtr layer(new Entity());
	//	mSceneLayers[i] = layer.get();
	//	mSceneGraph.attachChild(std::move(layer));
	//}

	//	Read all entities from file
		//	If entity.component contains Controller component
		//	player = entity;
	//sf::Texture texture = textureHolder.get("Raptor");
	//std::unique_ptr<Entity> player = EntFactory::Player::create(texture);

	//player->setPosition(300, 300);
	//mSceneLayers[Front]->attachChild(std::move(player));
}