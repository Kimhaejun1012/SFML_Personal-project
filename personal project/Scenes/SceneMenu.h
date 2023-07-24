#pragma once
#include "Scene.h"

class SpriteGo;
class UIButton;
class SceneMenu : public Scene
{
protected:

	SpriteGo* main;
	UIButton* startbutton;
	SpriteGo* targetScale;
	bool isbutton = true;
	float tick = 0.4;
public:

	SceneMenu();
	virtual ~SceneMenu() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

