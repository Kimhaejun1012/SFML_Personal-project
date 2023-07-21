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
		Bossbullet,
		Redbullet,
	};


protected:
	AnimationController monsterbullet;

	Types bullettype;
	int damage = 0;
	float speed = 0.f;
	float attackRate = 0.f;
	float attackTimer = 0.f;
	int bulletCount = 0;
	float tick;
	float range = 0.f;
	sf::Vector2f direction;
	sf::Vector2f attack = { 0.f,0.f };

	Player* player = nullptr;
	Monster* monster;


public:
	ObjectPool<MonsterBullet>* pool;

	MonsterBullet(const std::string textureId = "", const std::string n = "");
	virtual ~MonsterBullet() override;

	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir);

	void HitPlayer(float dt);

	void SetPlayer(Player* player);

	float GetAttacktimer();
	float GetattackRate() const;
	int GetBulletCount();

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
	void SetType(Types t);
};

