#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class Monster;
class Player;

class MonsterBullet : public SpriteGo
{
public:

	enum class Types
	{
		Boss,
		Monster,
	};


protected:

	AnimationController monsterbul;
	sf::Vector2f direction;
	sf::Vector2f attack = { 0.f,0.f };
	float speed = 0.f;
	float range = 2000.f;
	int damage = 10;
	Player* player;


public:
	ObjectPool<MonsterBullet>* pool;

	MonsterBullet(const std::string textureId = "", const std::string n = "");
	virtual ~MonsterBullet() override;

	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void Move(float dt);

	void SetDamage(int number);
	int GetDamage()
	{
		return damage;
	}
};

