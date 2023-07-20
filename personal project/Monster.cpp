#include "stdafx.h"
#include "Monster.h"
#include "Scene.h"
#include "Player.h"
#include "SceneDev1.h"
#include "DataTable.h"
#include "MonsterTable.h"
#include "Bullet.h"

Monster::Monster(const std::string n)
	: SpriteGo("", n)
{
}

Monster::~Monster()
{
}

void Monster::Init()
{
	SpriteGo::Init();
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/monster-1_Idle.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/monster-1_Move.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/monster-1_Attack.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/boss.csv"));
	//hp = maxHp;
	monster.SetTarget(&sprite);

	bossMoveDuration = 2.0f;
	bossMoveTimer = bossMoveDuration;
	bossMoveDir = {0.f,0.f};
	//Types::Boss
	//sprite.setScale(2.f, 2.f);
	//SetOrigin(Origins::BC);
	//SetPosition(0, 0);
	//sprite.getTexture() = "boss";
}

void Monster::Release()
{
	SpriteGo::Release();
}

void Monster::Reset()
{
	//SpriteGo::Reset();
	monster.Play("monster1Idle");
	SetOrigin(origin);
	//SetPosition({ 0, 0 });

	attackTimer = attackRate;
}

void Monster::Update(float dt)
{
	SpriteGo::Update(dt);
	monster.Update(dt);


	if (player == nullptr)
		return;

	direction = Utils::Normalize(player->GetPosition() - position);
	float distance = Utils::Distance(player->GetPosition(), position);

	tick -= dt;
	if (monsterType == Types::Boss)
	{
		sprite.setScale(3.f, 3.f);
		bossMoveTimer -= dt;
		SetOrigin(Origins::MC);
		if (bossMoveTimer <= 10.f)
		{
			bossMoveDir = { Utils::RandomRange(-1.f, 1.f) ,Utils::RandomRange(-1.f, 1.f)};
			sprite.move(bossMoveDir.x, bossMoveDir.y);
			bossMoveTimer = bossMoveDuration;
		}

		if (tick <= 0.f)
		{
			monster.Play("Boss");
	
			tick = 1.25f;
		}
	}

	if (monsterType == Types::Monster1)
	{
			if (distance > 150.f)
			{
				position += direction * (speed + plusespeed) * dt;
				sprite.setPosition(position);
				if (monster.GetCurrentClipId() != "monster1Move")
					monster.Play("monster1Move");
			}
			else
			{
				if (monster.GetCurrentClipId() != "monster1Attack")
					monster.Play("monster1Attack");
				HitPlayer(dt);
			}


		if (distance < 300.f)
		{
			plusespeed = 200;
		}
	}
}

void Monster::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Monster::SetType(Types t)
{
	const MonsterInfo* info = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get(t);

	//int index = (int)zombieType;
	//textureId = info.textureId;
	//monsterType = info.monsterType;
	//int index = (int)monsterType;
	//monsterType = (Monster::Types)info->monsterType;
	monsterType = (Monster::Types)info->monsterType;
	speed = info->speed;
	maxHp = info->maxHP;
	hp = maxHp;
	damage = info->damage;
	attackRate = info->attackRate;

	std::cout << (int)t << std::endl;
}

Monster::Types Monster::GetType() const
{
	return monsterType;
}

void Monster::SetPlayer(Player* player)
{
	this->player = player;
}

void Monster::OnHitBullet(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		
		if (scene != nullptr)
		{
			sceneDev1->OnDieMonster(this);
		}
	}
}


void Monster::FollowPlayer(float dt)
{
	float distance = Utils::Distance(player->GetPosition(), position);
	if (distance > 25.f && isActive)
	{
		position += direction * speed * dt;
		SetPosition(position);
	}
}

void Monster::HitPlayer(float dt)
{

	attackTimer += dt;
	if (attackTimer > attackRate)
	{
		attackTimer = 0.f;
		// 플레이어 피격
		player->OnHitted(damage);

	}
}

void Monster::GetMap2(const sf::FloatRect& mapsize)
{
	this->mapsize = mapsize;
}

//const std::list<Monster*>* Monster::GetMonsterList() const
//{
//	return &poolMonsters.GetUseList();
//}