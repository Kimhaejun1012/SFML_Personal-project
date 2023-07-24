#include "stdafx.h"
#include "SceneDev1.h"
#include "TextGo.h"
#include "TileMap.h"
#include "Player.h"
#include "Element.h"
#include "DataTable.h"
#include "DataTableMgr.h"
#include "Monster.h"
#include "Bullet.h"
#include "UIButton.h"
#include "MonsterBullet.h"

SceneDev1::SceneDev1() : Scene(SceneId::Dev1)
{
	resourceListPath = "scripts/DefaultResourceList.csv";
}

SceneDev1::~SceneDev1()
{
	//Release();
}

void SceneDev1::Init()
{
	Release();
	size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);


	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;


	TextGo* sceneName = (TextGo*)AddGo(new TextGo("", "Scene Name"));
	sceneName->sortLayer = 100;
	sceneName->text.setCharacterSize(25);
	sceneName->text.setFillColor(sf::Color::White);
	sceneName->text.setString(L"데브 1");

	tileMap = (TileMap*)AddGo(new TileMap("mapsprite/tile.png", "Tile Map"));
	tileMap2 = (TileMap*)AddGo(new TileMap("mapsprite/tile.png", "Tile Map"));
	mapmap = (SpriteGo*)AddGo(new SpriteGo("mapstructure/mapmap.png", "mapmap"));
	mapmap2 = (SpriteGo*)AddGo(new SpriteGo("mapstructure/mapmap.png", "mapmap"));
	element = (Element*)AddGo(new Element("mapsprite/element1.png", "element1"));
	nextdoor = (SpriteGo*)AddGo(new SpriteGo("mapstructure/LayerDoor_1.png", "door"));
	player = (Player*)AddGo(new Player());
	clear = (SpriteGo*)AddGo(new SpriteGo("graphics/win.png", "win"));
	clearbutton = (UIButton*)AddGo(new UIButton("graphics/home.png", "home"));

	clear->SetOrigin(Origins::MC);
	clear->SetPosition(size.x * 0.5, size.y * 0.5);
	clear->sortLayer = 102;
	clear->SetActive(false);

	clearbutton->SetOrigin(Origins::MC);
	clearbutton->SetPosition(size.x * 0.5, size.y * 0.8);
	clearbutton->sortLayer = 103;
	clearbutton->SetActive(false);

	tileMap->Load("map/map1.csv");
	tileMap->SetOrigin(Origins::MC);
	tileMap2->Load("map/map1.csv");
	tileMap2->SetOrigin(Origins::MC);
	tileMap2->SetPosition(tileMap->GetPosition().x, tileMap->GetPosition().y - 3000);
	mapmap2->SetPosition(tileMap2->GetPosition());
	mapmap2->SetOrigin(Origins::MC);


	element->SetOrigin(Origins::MC);
	element->SetPosition(sf::Vector2f{ tileMap->GetPosition() });


	//보스 체력바
	bossHpbar = ((SpriteGo*)AddGo(new SpriteGo("graphics/BossHpBar.png", "BossHpBar")));
	bossMaxHpBar = ((SpriteGo*)AddGo(new SpriteGo("graphics/BossHpBar.png", "BossHpBar")));
	bossicon = ((SpriteGo*)AddGo(new SpriteGo("graphics/bossicon.png", "BossHpBar")));



	bossHpbar->SetOrigin(Origins::ML);
	bossMaxHpBar->SetOrigin(Origins::ML);
	bossHpbar->sortLayer = 100;
	bossMaxHpBar->sprite.setColor({ 0,0,0,100 });
	bossHpbar->sprite.setPosition(size.x * 0.22, size.y * 0.12);
	bossMaxHpBar->sprite.setPosition(bossHpbar->sprite.getPosition());
	bossMaxHpBar->sortLayer = 100;

	bossicon->SetOrigin(Origins::MC);
	bossicon->sortLayer = 103;
	bossicon->SetPosition(bossMaxHpBar->sprite.getGlobalBounds().left * 2.25 /*+ (bossMaxHpBar->sprite.getGlobalBounds().width * 2)*/, bossMaxHpBar->sprite.getGlobalBounds().top- 15);

	bossicon->SetActive(false);
	bossHpbar->SetActive(false);
	bossMaxHpBar->SetActive(false);


	tileSize = tileMap->vertexArray.getBounds();
	mapmap->SetPosition(0, 0);
	mapmap->SetOrigin(Origins::MC);
	//nextdoor->SetPosition(-19.f, tileSize.top - 37);
	nextdoor->SetOrigin(Origins::MC);


	poolMonsters.OnCreate = [this](Monster* monster) {

		monster->SetPlayer(player);
	};
	poolMonsters.Init();


	tempCoin = ((SpriteGo*)AddGo(new SpriteGo("graphics/coin.png", "coin")));

	clearbutton->OnClick = [this]() {

		std::cout << "클리어 버튼 클릭" << std::endl;
		clear->SetActive(false);
		clearbutton->SetActive(false);
		bossdie = false;
		bossicon->SetActive(false);
		bossHpbar->SetActive(false);
		bossMaxHpBar->SetActive(false);
		bosson = 1;
		SCENE_MGR.ChangeScene(SceneId::Menu);
	};
	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneDev1::Release()
{
	//poolMonsters.Release();
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go; // <- 요기
	}
}

void SceneDev1::Enter()
{
	Scene::Enter();

	wallBounds = mapmap->sprite.getGlobalBounds();
	player->SetWallBounds(wallBounds);
	nextdoor->SetPosition(-19.f, wallBounds.top + 148);
	SpawnMonsters(1, sf::Vector2f(0, 0), monsterType1);
	SpawnMonsters(1, sf::Vector2f(0, 0), monsterType0);


}

void SceneDev1::Exit()
{
	//player->Release();
	//ClearObjectPool(poolMonsters);
	player->Reset();
	monster->Reset();
	stage = 1;
	//poolMonsters.Clear();
	//poolMonsters.Release();
	Scene::Exit();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);
	worldView.setCenter(player->GetPosition().x, player->GetPosition().y);
	NextScene();
	player->SetMonsterList(monsters);
	//std::cout << nextdoor->GetActive();
	if (bosson <= 0 && !bossdie)
	{
		bossicon->SetActive(true);
		bossHpbar->SetActive(true);
		bossMaxHpBar->SetActive(true);
		
		sf::IntRect bosshprect(0, 0, (int)((static_cast<float>(bosshp) / static_cast<float>(bossmaxhp) * 500)), 100);

		bossHpbar->sprite.setTextureRect(bosshprect);
		//bossHpbar->sprite.setScale(((static_cast<float>(bosshp) / static_cast<float>(bossmaxhp))),1.f);
	}
	else
	{
		bossicon->SetActive(false);
		bossHpbar->SetActive(false);
		bossMaxHpBar->SetActive(false);
	}

	if (bosspattern2)
	{
		SpawnMonsters(3, bossPos, (Monster::Types)0);
		bosspattern2 = false;
	}



	//std::cout << stage;
	if (nextScene && player->sprite.getGlobalBounds().intersects(nextdoor->sprite.getGlobalBounds()))
	{
		stage++;
		//wallBounds = mapmap2->sprite.getGlobalBounds();
		switch (stage)
		{
		case 1:
			wallBounds = mapmap->sprite.getGlobalBounds();
			std::cout << "스위치 1" << std::endl;
			break;
		case 2:
			wallBounds = mapmap2->sprite.getGlobalBounds();
			std::cout << "스위치 2" << std::endl;
			stage++;
			break;
		}

		//monsterbullet->SetMapSize(mapmap2->sprite.getGlobalBounds());
		monster->GetMap2(wallBounds);
		SpawnMonsters(1, tileMap2->GetPosition(), (Monster::Types)2);
		player->SetWallBounds(mapmap2->sprite.getGlobalBounds());
		player->SetPosition(tileMap2->GetPosition().x, tileMap2->GetPosition().y + 700);
		nextdoor->SetPosition(-19.f, wallBounds.top - 148);
	}

	if(INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
	SCENE_MGR.ChangeScene(SceneId::Title);
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F1))
		SCENE_MGR.ChangeScene(SceneId::Menu);


	for (int i = 0; i < coins.size(); ++i)
	{
		coinDir = Utils::Normalize(player->GetPosition() - coins[i]->sprite.getPosition());
		float distance = Utils::Distance(player->GetPosition(), coins[i]->sprite.getPosition());

		if (distance > 25.f)
		{
			coinPos = coins[i]->sprite.getPosition() + coinDir * speed * dt;
			coins[i]->sprite.setPosition(coinPos);
		}
		if (coins[i]->GetActive()) {

			if (coins[i]->sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
			{
				//std::cout << "경험치 올랐어요" << std::endl;
				int a = 30;
				player->GetExp(a);
				coins[i]->SetActive(false);
			}
		}
	}
	if (bossdie)
	{
		clear->SetActive(true);
		clearbutton->SetActive(true);
		//std::cout << "보스 죽음 클리어 메뉴 띄워라" << std::endl;
	}


}


void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

//void SceneDev1::Scene1MonsterSetting(int count)
//{
//	for (int i = 0; i < count; i++)
//	{
//		Monster::Types monsterType = (Monster::Types)0;
//		monster->SetType(monsterType);
//		monster->SetPlayer(player);
//		monster->Init();
//		//Zombie* zombie = poolZombies.Get();
//		//zombie->SetActive(false);
//	}
//}

void SceneDev1::SpawnMonsters(int count, sf::Vector2f center, Monster::Types a)
{
	for (int i = 0; i < count; i++)
	{

		tempCoin->SetActive(false);
		coins.push_back(tempCoin);
		//if (zombiePool.empty())
		//if (poolMonsters.GetPool().empty())
		//{
		//	CreateMonster(2);
		//}

		//Zombie* zombie = zombiePool.front();
		//zombiePool.pop_front();
		monster = poolMonsters.Get();
		monsters = poolMonsters.GetUseList();
		monster->SetType(a);
		monster->GetMap2(wallBounds);
		//zombie->SetActive(true);
		//이미지 2개를 두고 위에서 아래로 보낸다 일정 시간동안 보내다가 일정 시간이 지나면 사진 고정되게
		sf::Vector2f pos;
		do
		{
			pos = center + Utils::RandomInCircle(400);
		} while (Utils::Distance(center, pos) < 200.f && 3 > 200.f);

		monster->SetPosition(pos);

		if (a == Monster::Types::Boss)
			bosson -=1;

		//zombies.push_back(zombie);
		//zombie->Reset();
		AddGo(monster);
		monsterCount++;
		std::cout << "몬스터 생성" << std::endl;

	}
}

void SceneDev1::SpawnMonsters2(int count, sf::Vector2f center)
{
	for (int i = 0; i < count; i++)
	{

		tempCoin->SetActive(false);
		coins.push_back(tempCoin);


		//if (zombiePool.empty())
		//if (poolMonsters.GetPool().empty())
		//{
		//	CreateMonster(2);
		//}

		//Zombie* zombie = zombiePool.front();
		//zombiePool.pop_front();
		monster = poolMonsters.Get();
		monsters = poolMonsters.GetUseList();

		//zombie->SetActive(true);
		//이미지 2개를 두고 위에서 아래로 보낸다 일정 시간동안 보내다가 일정 시간이 지나면 사진 고정되게
		sf::Vector2f pos;
		do
		{
			pos = center + Utils::RandomInCircle(400);
		} while (Utils::Distance(center, pos) < 200.f && 3 > 200.f);

		monster->SetPosition(pos);

		//zombies.push_back(zombie);
		//zombie->Reset();
		AddGo(monster);
		monsterCount++;

	}
}
//void SceneDev1::CreateMonster(int count)
//{
//	for (auto zombie : poolMonsters.GetUseList())
//	{
//		//zombie->SetActive(false);
//		//RemoveGo(zombie);
//		//zombiePool.push_back(zombie);
//		//poolZombies.Return(zombie);
//		RemoveGo(zombie);
//	}
//	//zombies.clear();
//	poolMonsters.Clear();
//}

void SceneDev1::OnDieMonster(Monster* monster)
{
	RemoveGo(monster);
	poolMonsters.Return(monster);
	monsters.remove(monster);

	for (int i = 0; i < coins.size(); ++i)
	{
		if (coins[i]->GetActive())
		{
			continue;
		}
		else
		{
			//std::cout << "코인 테스트" << std::endl;
			coins[i]->SetActive(true);
			coins[i]->SetPosition(monster->GetPosition());
			coins[i]->SetOrigin(Origins::MC);
			coins[i]->sortLayer = 99;
			//GetCoin(coin);
			break;
			//std::cout << "코인 생성" << coin->GetPosition().x << std::endl;// 적절한 위치 설정
			// 코인을 화면에 그리는 로직 추가 (예: RenderWindow에 그리기)
		}
	}
	//auto cointex = FindGo("coin");
	//cointex->SetPosition(player->GetPosition());
	//cointex->SetOrigin(Origins::MC);
	//cointex->sortLayer= 5;
	//std::cout << cointex->GetPosition().x << cointex->GetPosition().y << std::endl;
	//player->RemoveMonster(monster);
	monsterCount--;

}

void SceneDev1::GetCoin(std::vector<SpriteGo*> coin)
{
	coin;
}

const std::list<Monster*>* SceneDev1::GetMonsterList() const
{
	return &poolMonsters.GetUseList();
}

void SceneDev1::BossPattern2(bool bosspattern2)
{
	this ->bosspattern2 = bosspattern2;
}


void SceneDev1::GetBossHp(int maxhp)
{
	bosshp = maxhp;
}

void SceneDev1::GetBossMaxHp(int maxhp)
{
	bossmaxhp = maxhp;
}

void SceneDev1::NextScene()
{
	if (monsterCount == 0)
	{
		nextScene = true;
		nextdoor->SetActive(true);
	}
	else
	{
		nextScene = false;
		nextdoor->SetActive(false);
	}
}
