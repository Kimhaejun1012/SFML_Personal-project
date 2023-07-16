#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class Bullet;
class Monster;

class Player : public SpriteGo
{
protected:
	AnimationController animation;
	sf::Vector2f velocity;
	sf::Vector2f direction;

	sf::Vector2f monsterlook;
	Monster* monster;
	sf::FloatRect playerwall;

	int bulletCount;
	float accel = 500.f;
	float speed = 500.f;
	float tick = 0.5f;
	bool filpX = false;
	bool increaseDamage;
	int bulletDamage = 10;
	ObjectPool<Bullet> poolBullets;
	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;


public:
	Player(const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n) {}
	virtual ~Player() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Release() override;
	bool GetFlipX() const;
	void SetFlipX(bool filp);

	virtual void Draw(sf::RenderWindow& window) override;
	void SetWallBounds(const sf::FloatRect& bounds);

	sf::Vector2f GetDirection();
	void Shoot();
	void LookMonster();
	void MoveStop();
	void PlayerMove(float dt);
	void ClearBullet();
};
