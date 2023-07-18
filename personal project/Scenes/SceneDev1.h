#pragma once
#include "Scene.h"
#include "ObjectPool.h"

class TileMap;
class Player;
class Element;
class MapStructure;
class Monster;
class SpriteGo;
class UIButton;

class SceneDev1 : public Scene
{
protected:
	//ObjectPool<Monster> poolMonsters;
	sf::Vector2f coinDir;
	sf::Vector2f coinPos;
	float speed = 80;
	SpriteGo* tempCoin;

	UIButton* testbutton1;
	UIButton* testbutton2;
	UIButton* testbutton3;

	std::vector<SpriteGo*> coins;
	SpriteGo* coin;
	ObjectPool<Monster> poolMonsters;
	sf::FloatRect tileSize;
	TileMap* tileMap = nullptr;
	Player* player;
	Element* element;
	bool nextScene;
	SpriteGo* mapmap;
	Monster* monster;
	sf::FloatRect wallBounds;
	SpriteGo* nextdoor;
	int monsterCount = 0;
	std::list<Monster*> monsters;
public:
	SceneDev1();
	virtual ~SceneDev1() override;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	//void Scene1MonsterSetting(int count);
	void SpawnMonsters(int count, sf::Vector2f center);

	const std::list<Monster*>* GetMonsterList() const;

	void NextScene();
	template<typename T>
	void ClearObjectPool(ObjectPool<T>& pool);
	//void CreateMonster(int count);
	void OnDieMonster(Monster* monster);
	void GetCoin(std::vector<SpriteGo*> coin);


};

template<typename T>
inline void SceneDev1::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto obj : pool.GetUseList())
	{
		RemoveGo(obj);
	}
	pool.Clear();
}