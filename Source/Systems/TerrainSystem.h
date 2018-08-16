#pragma once
#include <random>
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include "System.h"
#include "Chunk.h"

using namespace Settings;
const int TABLE_SIZE = 256;

////////////////////////////////////////////////////////////
/// Components used: Transform, Player, 
///	The system manages chunks
////////////////////////////////////////////////////////////
class TerrainSystem : public System
{
public:
	typedef std::unique_ptr<Chunk> ChunkPtr;

public:
	TerrainSystem();			//	TODO: Add ability to add custom seed

	void						init() override;
	void						begin() override;
	void						update(float dt) override;
	void						onEntityUpdate(const Entity* entity) override;

private:
	void						updateChunks();
	ChunkPtr						createChunk(int ID);
	void						loadTextures();

	////////////////////////////////////////////////////////////
	/// Methods for Perlin noise
	///	
	////////////////////////////////////////////////////////////

	float						octaveNoise(float x, int octaves, float persistence,
		float frequency, float amplitude, float offset, bool makeSigned = false);
	float						noise(float x, bool makeSigned = false);
	float						lerp(float a, float b, float t);
	float						fade(float t);

	void						generateTable();	//	Generate gradients (0.0  -  1.0) for this seed

private:
	std::vector<ChunkPtr> availableChunks;						
	TextureHolder<std::string> textureHolder;

	int seed;
	float table[TABLE_SIZE];
	float d2_counter;
	//unsigned int uniqueChunkID;		//	Only positive IDs

	//const World* mWorld;			//	To get the textures 



private:
	//struct EntComponents
	//{
	//	Transform* transformComp;		//	Required
	//	Player* playerComp;			//	Required 

	//	EntComponents(Transform* trans, Player* play)
	//		: transformComp{ trans }, playerComp{ play }
	//	{
	//	}
	//};
	//std::map<int, std::unique_ptr<EntComponents>> entities;		//	Only player here :(

	Transform* transformComp;
	Player* playerComp;
};