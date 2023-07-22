#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"
#include "Bullet.h"

class Monster;
class UIButton;
class SpriteGo;

class Player : public SpriteGo
{

public:

	struct ClipInfo
	{
		std::string idle;
		std::string move;
		std::string attack;
		bool flipX = false;
		sf::Vector2f point;
	};

protected:
	AnimationController animation;

	sf::Vector2f windowsize;
	sf::Vector2f velocity;
	sf::Vector2f direction;

	std::list<Monster*> monsters;
	Monster* monster;
	sf::Vector2f closestMonster;

	UIButton* uibutton;
	UIButton* testbutton1;
	UIButton* testbutton2;
	UIButton* testbutton3;
	
	Bullet* bullet;
	ObjectPool<Bullet> poolBullets;
	int bulletCount;
	int bulletDamage = 1000;
	bool attack;

	sf::Vector2f monsterlook = { 0,0 };
	sf::FloatRect playerwall;
	Player* player;
	int maxexp = 100;
	int exp = 0;


	float accel = 500.f;
	float speed = 3000.f;
	float tick = 0.5f;
	bool filpX = false;
	bool increaseDamage;

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	std::vector<ClipInfo> clipInfos;
	ClipInfo currentClipInfo;

	int MaxHp = 1000;
	int Hp = 0;

	SpriteGo* playerHp;
	SpriteGo* playerMaxHp;
	SpriteGo* expbar;
	SpriteGo* maxexpbar;

public:
	Player(const std::string& textureId = "", const std::string& n = "");
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
	void ShootAndLook();
	void PlayerMove(float dt);
	void ClearBullet();
	//void GetMonsterList(Monster* monster);
	void SetMonster(Monster* monster);
	void SetMonsterList(const std::list<Monster*> list);
	void RemoveMonster(Monster* monster);
	void OnHitted(int damage);
	int GetHp() const;
	int ReturnExp();
	void GetExp(int exp);
	int ReturnMaxExp();
	void LevelUp();
	void GetMaxExp(float exp);
	bool isplaying = true;
	bool isAlive = true;
	void IncreaseBullet();
	void IncreaseAttack();
	void IncreaseSpeed();
	void PlayerUI();
	void OnHitBullet(int damage);
	void OnDiePlayer();
	Player* GetPlayer();
	void PlayerReset();
};

