#pragma once
#include "Scene.h"
#include "ObjectPool.h"

class TileMap;
class Player;
class Element;
class MapStructure;
class Monster;
class SpriteGo;
//class Bullet;

class SceneDev2 : public Scene
{
protected:
	ObjectPool<Monster> poolMonsters;
	sf::FloatRect tileSize;
	TileMap* tileMap = nullptr;
	Player* player;
	Element* element;
	bool nextScene;
	SpriteGo* mapmap;
	Monster* monster;
	sf::FloatRect wallBounds;
	SpriteGo* nextdoor;
	int monsterCount = 0;
public:
	SceneDev2();
	virtual ~SceneDev2() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

