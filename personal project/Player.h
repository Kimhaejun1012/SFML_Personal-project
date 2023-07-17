#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class Bullet;
class Monster;
class UIButton;
class Player : public SpriteGo
{
protected:
	AnimationController animation;
	sf::Vector2f velocity;
	sf::Vector2f direction;
	std::list<Monster*> monsters;
	sf::Vector2f monsterlook = { 0,0 };
	Monster* monster;
	sf::FloatRect playerwall;
	int maxexp = 100;
	int exp = 0;
	UIButton* uibutton;
	int bulletCount;
	float accel = 500.f;
	float speed = 500.f;
	float tick = 0.5f;
	bool filpX = false;
	bool increaseDamage;
	int bulletDamage = 100;
	ObjectPool<Bullet> poolBullets;
	Bullet* bullet;
	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	int MaxHp = 100;
	int Hp = 0;

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
	void PlayerMove(float dt);
	void ClearBullet();
	//void GetMonsterList(Monster* monster);
	void SetMonster(Monster* monster);
	void SetMonsterList(const std::list<Monster*> list);
	void RemoveMonster(Monster* monster);
	void OnHitted(int damage);
	int GetHp() const;
	int ExpExp();
	void GetExp(int exp);
	int GetMaxExp();
	void LevelUp();
	bool isplaying = true;
	bool isAlive = true;
	void IncreaseBullet();
	void IncreaseAttack();
	void IncreaseSpeed();
};

