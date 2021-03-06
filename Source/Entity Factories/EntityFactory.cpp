#include "stdafx.h"
#include "EntityFactory.h"
#include "EntityManager.h"
//#include "World.h"

using namespace Settings;

EntityFactory::EntityFactory()
	: textureHolder{}
{
	entities = getAllFilesFromFolder<std::string>("\Entities");		//	Automatically load all entities from folder

	loadTextures();
}

EntPtr EntityFactory::spawnEntity(int uniqueID, std::string ID, sf::Vector2f position)
{
	///	Find path from list
	assert(!entities.empty());			//	If there are no entities to spawn at all
	auto it = entities.find(ID);
	assert(it != entities.end());		//	If the m_entity was not found in the list
	std::string& path = it->second;

	///	Read contents from file
	std::ifstream file(path);
	std::unique_ptr<Entity> m_entity;

	if (file)
	{
		std::string line;
		m_entity = std::unique_ptr<Entity>(new Entity(uniqueID));

		//	Mandatory component
		//m_entity->addComponent<Transform>();
		m_entity->setPosition(position);

		while (std::getline(file, line, '\n'))
		{
			std::istringstream iss(line);
			std::vector<std::string> words{
				std::istream_iterator<std::string>(iss),{}
			};
			if (words.empty())		//	Blank line detected, skip
				continue;

			///	Spaghetti code. Improve it. Please.
			if (words[0] == "Component")
			{
				if (words[1] == "Physics")
					m_entity->addComponent<Physics>();
				else if (words[1] == "Collider")
					m_entity->addComponent<Collider>();
				else if (words[1] == "Controller")
					m_entity->addComponent<Controller>();
				else if (words[1] == "Movement")
					m_entity->addComponent<Movement>();
				else if (words[1] == "Sprite2D")
					m_entity->addComponent<Sprite2D>();
				else if (words[1] == "Player")
					m_entity->addComponent<Player>();
				else if (words[1] == "Anim")
					m_entity->addComponent<Anim>();
				else if (words[1] == "LivingThing")
					m_entity->addComponent<LivingThing>();
				else if (words[1] == "Grabbable")
					m_entity->addComponent<Grabbable>();
			}
			else if (words[0] == "Texture")
			{
				Sprite2D* spriteComp = &m_entity->getComponent<Sprite2D>();
				//Transform* globalTransform = &m_entity->getComponent<Transform>();
				spriteComp->texture = textureHolder.get(words[1]);	//	Get texture from world
				spriteComp->sprite = sf::Sprite(spriteComp->texture);
				spriteComp->sprite.setPosition(m_entity->getPosition());
			}
			else if (words[0] == "SpriteSheet")			//	REMEMBER TO SET SCALE!!
			{
				///	Get all relevant info from file
				Anim* anim = &m_entity->getComponent<Anim>();
				Animation newAnimRight(sf::milliseconds(std::stoi(words[5])));		//	New animation with frametime specified in file
				Animation newAnimLeft(sf::milliseconds(std::stoi(words[5])));		//	New animation with frametime specified in file

				//	Convert integer from file to Action type
				Anim::AnimAction action = static_cast<Anim::AnimAction>(std::stoi(words[4]));

				anim->textures.insert(std::make_pair(action, sf::Texture(spriteSheetHolder.get(words[1]))));

				sf::Texture* sprite = &anim->textures.find(action)->second;

				newAnimRight.setSpriteSheet(*sprite);
				newAnimLeft.setSpriteSheet(*sprite);
				sf::Vector2u size = newAnimRight.getSpriteSheet()->getSize();		
				int rows = std::stoi(words[2]);			//	Rows in sprite sheet
				int columns = std::stoi(words[3]);		//	Columns in sprite sheet

				std::cout << sf::milliseconds(std::stoi(words[5])).asMilliseconds() << '\n';
				
				///	Get all frames in the spritesheet and add them to the animataion object
				for(int y = 0; y < size.y; y += size.y / rows)	
					for (int x = 0; x < size.x; x += size.x / columns)
					{
						newAnimRight.addFrame(sf::IntRect(x, y, size.x / columns, size.y / rows));
						newAnimLeft.addFrame(sf::IntRect(x + (size.x / columns), y, -1.0f * (size.x / columns), size.y / rows));
					}

				///	Insert into map and add
				anim->rightAnimations.insert(std::make_pair(action, newAnimRight));
				anim->leftAnimations.insert(std::make_pair(action, newAnimLeft));

				anim->activeAnim.setAnimation(anim->rightAnimations.find(action)->second);

				///	Set config of spriteAnimation object
				anim->activeAnim.setLooped(true);
				anim->activeAnim.stop();
			}
			else if (words[0] == "ColliderBox")
			{
				Collider* collider = &m_entity->getComponent<Collider>();

				if (words[1] == "default")
				{
					if (m_entity->hasComponent<Anim>())
					{
						Anim* anim = &m_entity->getComponent<Anim>();
						collider->colliderBox = anim->activeAnim.getGlobalBounds();
					}
					else if (m_entity->hasComponent<Sprite2D>())
					{
						Sprite2D* sprite2D = &m_entity->getComponent<Sprite2D>();
						collider->colliderBox = sprite2D->sprite.getGlobalBounds();
					}
				}
				//	For custom collider boxes (NOT TESTED YET!!! NOR SHOULD IT BE USED)
				else if (std::stoi(words[1]) || std::stoi(words[1]) == 0)
				{
					int width = std::stoi(words[1]), height = std::stoi(words[2]);

					collider->colliderBox.width = width;
					collider->colliderBox.height = height;
				}
			}
		}
	}
	return std::move(m_entity);
}

void EntityFactory::loadTextures()	//	Load all textures and sprite sheets from folders
{
	std::map<std::string, std::string> files = getAllFilesFromFolder<std::string>("Media/Textures");

	for (const auto& file : files)
		textureHolder.load(file.first, file.second);

	files = getAllFilesFromFolder<std::string>("Media/SpriteSheets");

	for (const auto& file : files)
		spriteSheetHolder.load(file.first, file.second);
}
