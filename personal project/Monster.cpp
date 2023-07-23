#include "stdafx.h"
#include "Monster.h"
#include "Scene.h"
#include "Player.h"
#include "SceneDev1.h"
#include "DataTable.h"
#include "MonsterTable.h"
#include "Bullet.h"
#include "MonsterBullet.h"
#include "UIButton.h"

//std::cout << "들어옴" << std::endl;
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

	windowsize = FRAMEWORK.GetWindowSize();
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaMoveDown.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaMoveDownRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaMoveRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaMoveUp.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaMoveUpRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaRunDown.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaRunDownRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaRunRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaRunUp.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaRunUpRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/GoombaDie.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveDown.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveDownRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveUp.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveUpRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossPattern1.csv"));
	//공격 애니매이션
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossPattern1.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossPattern2.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossPattern3.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossPattern4.csv"));




	monster.SetTarget(&sprite);

	bossMoveDuration = 2.0f;
	bossMoveTimer = bossMoveDuration;
	bossMoveDir = { 0.f,0.f };

	ObjectPool<MonsterBullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr, this](MonsterBullet* monsterbullet) {

		monsterbullet->SetPlayer(player);
		monsterbullet->SetMapSize(mapsize);
		monsterbullet->pool = ptr;

	};
	poolBullets.Init();

	bosscount = 1;
	//굼바
	clipInfos.push_back({ "GoombaMoveUpRight", "GoombaRunUpRight" ,true, Utils::Normalize({-1.f, -1.f}) });
	clipInfos.push_back({ "GoombaMoveUp", "GoombaRunUp" ,true, {0.f, -1.f} });
	clipInfos.push_back({ "GoombaMoveUpRight", "GoombaRunUpRight",false, Utils::Normalize({1.f, -1.f}) });

	clipInfos.push_back({ "GoombaMoveRight", "GoombaRunRight" ,true, {-1.f, 0.f} });
	clipInfos.push_back({ "GoombaMoveRight", "GoombaRunRight" ,false, {1.f, 0.f} });

	clipInfos.push_back({ "GoombaMoveDownRight", "GoombaRunDownRight" ,true, Utils::Normalize({-1.f, 1.f}) });
	clipInfos.push_back({ "GoombaMoveDown", "GoombaRunDown",true,{0.f, 1.f} });
	clipInfos.push_back({ "GoombaMoveDownRight", "GoombaRunDownRight",false, Utils::Normalize({1.f, 1.f}) });

	//보스
	clipInfosboss.push_back({ "BossMoveUpRight", "BossMoveUpRight" ,true, Utils::Normalize({-1.f, -1.f}) });
	clipInfosboss.push_back({ "BossMoveUp", "BossMoveUp" ,true, {0.f, -1.f} });
	clipInfosboss.push_back({ "BossMoveUpRight", "BossMoveUpRight",false, Utils::Normalize({1.f, -1.f}) });

	clipInfosboss.push_back({ "BossMoveRight", "BossMoveRight" ,true, {-1.f, 0.f} });
	clipInfosboss.push_back({ "BossMoveRight", "BossMoveRight" ,false, {1.f, 0.f} });

	clipInfosboss.push_back({ "BossMoveDownRight", "BossMoveDownRight" ,true, Utils::Normalize({-1.f, 1.f}) });
	clipInfosboss.push_back({ "BossMoveDown", "BossMoveDown",true,{0.f, 1.f} });
	clipInfosboss.push_back({ "BossMoveDownRight", "BossMoveDownRight",false, Utils::Normalize({1.f, 1.f}) });


}

void Monster::Release()
{
	//poolBullets.Clear();
	SpriteGo::Release();
}

void Monster::Reset()
{
	SpriteGo::Reset();
	monster.Play("BossMoveDown");
	SetOrigin(origin);

	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}

	poolBullets.Clear();
	currentClipInfo = clipInfos[6];
	currentClipInfoboss = clipInfosboss[6];
}

void Monster::Update(float dt)
{
	SpriteGo::Update(dt);
	monster.Update(dt);
	SetOrigin(Origins::BC);

	tick -= dt;
	if (player == nullptr)
		return;

	direction = Utils::Normalize(player->GetPosition() - position);
	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}


	if (monsterType == Types::Boss)
	{
		attackTimer += dt;
		//if(bossMoving)
		BossMove(dt);
		if (attackTimer > attackRate)
		{
			SpawnBullet((MonsterBullet::Types)0);
			Shoot();
			attackTimer = 0;
		}
	}

	if (monsterType == Types::Monster1)
	{
		if (monsterscale)
		{
			sprite.setScale(2.5f, 2.5f);
			monsterscale = false;
		}
		position += direction * speed * dt;
		SetPosition(position);
		float distance = Utils::Distance(player->GetPosition(), position);
		auto min = std::min_element(clipInfos.begin(), clipInfos.end(),
			[this](const ClipInfo& lhs, const ClipInfo& rhs) {
			return Utils::Distance(lhs.point, direction) < Utils::Distance(rhs.point, direction);
		});
		currentClipInfo = *min;
		std::string clipId = distance >= 300.f ? currentClipInfo.move : currentClipInfo.run;

		if (GetFlipX() != currentClipInfo.flipX)
		{
			SetFlipX(currentClipInfo.flipX);
		}
		if (monster.GetCurrentClipId() != clipId)
		{
			if (clipId == currentClipInfo.run)
			{
				speed = 150.f;
			}
			else
				speed = 40.f;
			monster.Play(clipId);
			if (distance <= 50.f)
			{
				HitPlayer(dt);
			}
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

	//std::cout << (int)t << std::endl;
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

	if (hp <= 0 && monsterType == Types::Boss)
	{

		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);

		if (scene != nullptr)
		{
			sceneDev1->OnDieMonster(this);
			sceneDev1->bossdie = true;
		}
	}
	else if (hp <= 0 && monsterType == Types::Monster1 /*&& monster.GetCurrentClipId() != "GoombaDie"*/)
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


void Monster::Shoot()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);

	playerlook = Utils::Normalize(player->GetPosition() - GetPosition());
	//std::cout << playerlook.x << playerlook.y << std::endl;
	MonsterBullet* monsterbullet = poolBullets.Get();
	monsterbullet->SetMapSize(mapsize);

	//float modifiedAngle = Utils::Angle(monsterlook);  // 기존 각도 계산
	//float additionalAngle = (count % 2 == 1) ? 15.f * count : -15.f * count;  // 추가 각도 계산
	//float additionalAngle = -5 + (30 / bulletCount * count); //(count == 0) ? 0.f : ((count % 2 == 1) ? 15.f * count : -15.f * count);
	//float additionalAngle =  (360 / bulletCount * count);
	//float finalAngle = modifiedAngle; // + additionalAngle;  // 기존 각도와 추가 각도 합산
	//sf::Vector2f fireDirection = Utils::DirectionFromAngle(finalAngle);  // 총알 발사 각도 계산

	monsterbullet->Fire(GetPosition(), playerlook);
	if (scene != nullptr)
	{
		//monsterbullet->SetPlayer(player->GetPlayer());
		sceneDev1->AddGo(monsterbullet);
	}

}

void Monster::SpawnBullet(MonsterBullet::Types t)
{
	monsterbullet = poolBullets.Get();
	monsterbullets = poolBullets.GetUseList();
	monsterbullet->SetType(t);
}

bool Monster::GetFlipX() const
{
	return flipX;
}

void Monster::BossMove(float dt)
{

		position += direction * speed * dt;
		SetPosition(position);
	float distance = Utils::Distance(player->GetPosition(), position);
	auto min = std::min_element(clipInfosboss.begin(), clipInfosboss.end(),
		[this](const ClipInfoBoss& lhs, const ClipInfoBoss& rhs) {
		return Utils::Distance(lhs.point, direction) < Utils::Distance(rhs.point, direction);
	});
	currentClipInfoboss = *min;
	std::string clipId = distance >= 300.f ? currentClipInfoboss.move : currentClipInfoboss.run;
	if (GetFlipX() != currentClipInfoboss.flipX)
	{
		SetFlipX(currentClipInfoboss.flipX);
	}
	if (monster.GetCurrentClipId() != clipId)
	{
		monster.Play(clipId);
	}
}

void Monster::SetFlipX(bool flip)
{
	flipX = flip;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !flipX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}
