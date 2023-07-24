#pragma once
#include "SpriteGo.h"
class Player;

class UIButton : public SpriteGo
{
protected:
	bool isHover = false;
	Player* player;
public:
	UIButton(const std::string& textureId = "", const std::string& n = "");
	virtual ~UIButton() override;
	float tick = 1.0;
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	sf::Text text;
	void IncreaseBullet();
	void IncreaseAttact();
	void IncreaseSpeed();
	void SetPlayer(Player* player);
	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;
	std::function<void()> OnMouse;
};

