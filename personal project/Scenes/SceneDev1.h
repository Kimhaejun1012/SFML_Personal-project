#pragma once
#include "Scene.h"
class TileMap;
class Player;
class Element;
class MapStructure;


class SceneDev1 : public Scene
{
protected:

	sf::FloatRect tileSize;
	TileMap* tileMap = nullptr;
	Player* player;
	Element* element;

	MapStructure* mapmap;

	sf::FloatRect wallBounds;

public:
	SceneDev1();
	virtual ~SceneDev1() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void AddStructure();
	void IsStructure();
	void MapFR();
};

