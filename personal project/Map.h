#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "ObjectPool.h"

class Player;
class TileMap;
class SpriteGo;

class Map : public Scene
{
protected:
	TileMap* tileMap = nullptr;
	SpriteGo* mapmap;
	sf::FloatRect wallBounds;
	sf::FloatRect tileSize;

	Player* player;
public:
	Map();
	virtual ~Map() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	void NextScene();
};

