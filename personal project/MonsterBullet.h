#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class Monster;
class Player;
class SceneDev1;

class MonsterBullet : public SpriteGo
{
public:

	enum class Types
	{
		FireBall,
		Pattern31,
		Pattern32,
		Pattern4,
		Hammer,
	};


protected:

	sf::Vector2f pattern31BulletPos;
	AnimationController monsterbullet;
	Types bullettype;
	SceneDev1* scenedev1;
	int damage = 0;
	float speed = 0.f;
	float attackRate = 0.f;
	float attackTimer = 0.f;
	int bulletCount = 0;
	float tick;
	float range = 0.f;

	bool isone = true;
	sf::Vector2f direction;
	sf::Vector2f attack = { 0.f,0.f };

	Player* player = nullptr;
	Monster* monster;

	

	sf::FloatRect mapsize;

public:
	ObjectPool<MonsterBullet>* pool;
	MonsterBullet(const std::string textureId = "", const std::string n = "");
	virtual ~MonsterBullet() override;

	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir);

	void HitPlayer(float dt);

	void SetPlayer(Player* player);
	void GetType(Types info);


	void SetMapSize(sf::FloatRect& mapsize);
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	sf::Vector2f Get31Pos();
	MonsterBullet::Types ReturnType();

	void Move(float dt);
	void SetDamage(int number);
	int GetDamage()
	{
		return damage;
	}
	void SetType(Types t);
};

