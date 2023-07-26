#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "ObjectPool.h"
#include "Bullet.h"

class Monster;
class UIButton;
class SpriteGo;
class TileMap;


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

	int level;

	bool isone = true;
	bool islev = true;

	UIButton* uibutton;
	int level1;
	int level2;
	int level3;

	bool stop = false;


	std::vector<UIButton*> upgradeButtons1;
	std::vector<UIButton*> upgradeButtons2;
	std::vector<UIButton*> upgradeButtons3;
	std::vector<std::string> upgradeOptions;

	float levelup = 0.07f;

	float levelupgg = 4.0f;

	Bullet* bullet;
	ObjectPool<Bullet> poolBullets;

	int bulletDamage;
	
	bool attack;

	sf::Vector2f monsterlook = { 0,0 };
	sf::FloatRect playerwall;
	Player* player;


	TileMap* tilemap = nullptr;

	//std::vector<TileMap*> tiles;

	float tick = 0.1f;
	float levtick = 0.07;

	bool filpX = false;
	bool increaseDamage;

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	sf::RectangleShape playerrec;
	float damagetick = 0.f;

	std::vector<ClipInfo> clipInfos;
	ClipInfo currentClipInfo;

	bool playermove;

	sf::Vector2f playerPos;
	sf::FloatRect playertile;

	int bulletCount;
	float attackspeed;
	float attackspeedfull = 1.0;
	float speed;
	float MaxHp;
	float Hp;
	int maxexp;
	int exp = 0;

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

	void SetTileMap(TileMap* tilemap);

	sf::RectangleShape GetPlayerRec();
	/*void SetTile(Tile::tile);*/

	bool isPlaying();
};

