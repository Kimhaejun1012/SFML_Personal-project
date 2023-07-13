#pragma once
#include "Scene.h"
class TileMap;
class Player;
class Element;

class SceneDev1 : public Scene
{
protected:

	TileMap* tileMap = nullptr;
	Player* player;
	Element* element;

public:
	SceneDev1();
	virtual ~SceneDev1() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

