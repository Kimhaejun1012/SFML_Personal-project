#include "stdafx.h"
#include "Monster.h"
#include "Scene.h"
#include "Player.h"
#include "SceneDev1.h"
#include "DataTable.h"
#include "MonsterTable.h"
#include "Bullet.h"
#include "UIButton.h"

//std::cout << "����" << std::endl;
Monster::Monster(const std::string& textureId, const std::string& n) : SpriteGo(textureId, n)
{
}

Monster::~Monster()
{
}

void Monster::Init()
{
	SpriteGo::Init();

	windowsize = FRAMEWORK.GetWindowSize();
	//���� ������, ����
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

	//�ظ�
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerMoveDown.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerMoveDownRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerMoveRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerMoveUp.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerMoveUpRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerAttackDown.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerAttackDownRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerAttackRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerAttackUp.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerAttackUpRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/HammerIdle.csv"));


	//���� ������
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveDown.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveDownRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveUp.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossMoveUpRight.csv"));
	monster.AddClip(*RESOURCE_MGR.GetAnimationClip("monstercsv/BossPattern1.csv"));

	//���� ���� ����
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
	//����
	clipInfos.push_back({ "GoombaMoveUpRight", "GoombaRunUpRight" ,true, Utils::Normalize({-1.f, -1.f}) });
	clipInfos.push_back({ "GoombaMoveUp", "GoombaRunUp" ,true, {0.f, -1.f} });
	clipInfos.push_back({ "GoombaMoveUpRight", "GoombaRunUpRight",false, Utils::Normalize({1.f, -1.f}) });

	clipInfos.push_back({ "GoombaMoveRight", "GoombaRunRight" ,true, {-1.f, 0.f} });
	clipInfos.push_back({ "GoombaMoveRight", "GoombaRunRight" ,false, {1.f, 0.f} });

	clipInfos.push_back({ "GoombaMoveDownRight", "GoombaRunDownRight" ,true, Utils::Normalize({-1.f, 1.f}) });
	clipInfos.push_back({ "GoombaMoveDown", "GoombaRunDown",true,{0.f, 1.f} });
	clipInfos.push_back({ "GoombaMoveDownRight", "GoombaRunDownRight",false, Utils::Normalize({1.f, 1.f}) });

	//�ظ�
	clipInfosHammer.push_back({ "HammerMoveUpRight", "HammerAttackUpRight" ,"HammerIdle",true, Utils::Normalize({-1.f, -1.f})});
	clipInfosHammer.push_back({ "HammerMoveUp", "HammerAttackUp" ,"HammerIdle",true, {0.f, -1.f} });
	clipInfosHammer.push_back({ "HammerMoveUpRight", "HammerAttackUpRight","HammerIdle",false, Utils::Normalize({1.f, -1.f}) });

	clipInfosHammer.push_back({ "HammerMoveRight", "HammerAttackRight" ,"HammerIdle",true, {-1.f, 0.f} });
	clipInfosHammer.push_back({ "HammerMoveRight", "HammerAttackRight" ,"HammerIdle",false, {1.f, 0.f} });

	clipInfosHammer.push_back({ "HammerMoveDownRight", "HammerAttackDownRight" ,"HammerIdle",true, Utils::Normalize({-1.f, 1.f}) });
	clipInfosHammer.push_back({ "HammerMoveDown", "HammerAttackDown","HammerIdle",true,{0.f, 1.f} });
	clipInfosHammer.push_back({ "HammerMoveDownRight", "HammerAttackDownRight","HammerIdle",false, Utils::Normalize({1.f, 1.f}) });

	//����
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
	//BossHpUI();

	monster.Play("BossMoveDown");
	SetOrigin(origin);


	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}

	poolBullets.Clear();
	currentClipInfo = clipInfos[6];
	currentClipInfoboss = clipInfosboss[6];
	currentClipInfoHammer = clipInfosHammer[6];
}

void Monster::Update(float dt)
{
	SpriteGo::Update(dt);
	monster.Update(dt);
	SetOrigin(Origins::BC);



	tick -= dt;
	if (player == nullptr)
		return;

	// ���� ������ �ƴҰ�� �������ʹ� ���ó�� �����̸鼭 �Ѿ˽�
	// ���� ���ڷ� ���� ������ �ɸ���� �������̶� �Ѿ˽�°� ���߰� ���ϸ� �÷�����



	direction = Utils::Normalize(player->GetPosition() - position);
	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}

	if (monsterType == Types::Boss)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		sceneDev1->GetBossHp(hp);
		sceneDev1->GetBossMaxHp(maxHp);

		if (randomPattern==0)
		{
			if (attackTimer > attackRate)
			{
				SpawnBullet((MonsterBullet::Types)0);
				//Shoot();
				attackTimer = 0;
			}
			BossMove(dt);
			bosspattern += dt;
			attackTimer += dt;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num6))
		{
			isPattern = true;

			randomPattern = 1; // ���� ���� ����
			bosspattern = 0;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num7))
		{
			isPattern = true;
			//randomPattern = Utils::RandomRange(1, 5); // ���� ���� ����
			randomPattern =2; // ���� ���� ����
			bosspattern = 0;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num8))
		{
			isPattern = true;
			//randomPattern = Utils::RandomRange(1, 5); // ���� ���� ����
			randomPattern = 3; // ���� ���� ����
			bosspattern = 0;
		}
		if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num9))
		{
			isPattern = true;
			randomPattern =4; 
			bosspattern = 0;
		}
		if (randomPattern == 1)
		{
			BossPattern1(dt);

			if (monster.GetTotalFrame() - monster.GetCurFrame() <= 1)
			randomPattern = 0;
				std::cout << "������ ��" << std::endl;
		}
		if (randomPattern == 2)
		{

			BossPattern2(dt);
			if (monster.GetTotalFrame() - monster.GetCurFrame() <= 1)
			{
				std::cout << "������ ��"<<std::endl;
				randomPattern = 0;
				isone = true;
			}
		}
		if (randomPattern == 3)
		{
			BossPattern3(dt);
			if (monster.GetTotalFrame() - monster.GetCurFrame() <= 10)
			{
				if (isone)
				{
					Pattern3Bullet((MonsterBullet::Types)1);
					isone = false;
				}
			}
			if (monster.GetTotalFrame() - monster.GetCurFrame() <= 1)
			{
				randomPattern = 0;
				isone = true;
				std::cout << "������ ��" << std::endl;
			}
		}
		if (randomPattern == 4)
		{
			std::cout << monster.GetTotalFrame() - monster.GetCurFrame() << std::endl;
			BossPattern4(dt);
			if(monster.GetTotalFrame() - monster.GetCurFrame() <= 70)
			{
				zero += dt;
				float end = 0.5f;
				if (zero >= end)
				{
					SpawnBullet2(5, (MonsterBullet::Types)3);
					zero = 0.f;
				}
			}

			if (monster.GetTotalFrame() - monster.GetCurFrame() <= 1)
			{

				randomPattern = 0;
				std::cout << "������ ��" << std::endl;
				}
		}
		bosspattern = 0;
		isPattern = false;
		}


	if(monsterType == Types::Monster1)
	{
		GoombaMove(dt);
	}

	if (monsterType == Types::Hammer)
	{
		if (monsterscale)
		{
			sprite.setScale(2.0f, 2.0f);
			monsterscale = false;
		}
		float distance = Utils::Distance(player->GetPosition(), position);
		auto min = std::min_element(clipInfosHammer.begin(), clipInfosHammer.end(),
			[this](const ClipInfoHammer& lhs, const ClipInfoHammer& rhs) {
				return Utils::Distance(lhs.point, direction) < Utils::Distance(rhs.point, direction);
			});
		currentClipInfoHammer = *min;
		std::string clipId = distance <= 550 ? (distance <= 350 ? currentClipInfoHammer.attack : currentClipInfoHammer.move) : currentClipInfoHammer.idle;

		if (GetFlipX() != currentClipInfoHammer.flipX)
		{
			SetFlipX(currentClipInfoHammer.flipX);
		}

		attackTimer += dt;

			if (clipId == currentClipInfoHammer.attack)
			{
				if (monster.GetCurrentClipId() != clipId)
				{
					monster.Play(clipId);
				}
					if (attackTimer > attackRate)
					{
						SpawnBullet((MonsterBullet::Types)4);
						attackTimer = 0;
					}
				
			}
			else if (clipId == currentClipInfoHammer.move)
			{
				if (monster.GetCurrentClipId() != clipId)
				{
					monster.Play(clipId);
				}
					position += direction * speed * dt;
					SetPosition(position);

			}
			else if (clipId == currentClipInfoHammer.idle)
			{
				if (monster.GetCurrentClipId() != clipId)
				{
					monster.Play(clipId);
				}
					position += direction * 0.f * dt;
					SetPosition(position);
				}
			}
		}


void Monster::BossPattern1(float dt)
{

		if (monster.GetCurrentClipId() != "BossPattern1")
		{
			monster.Play("BossPattern1");
		}
		position += direction * (speed+300) * dt;
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

}

void Monster::BossPattern2(float dt)
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);

	if (monster.GetCurrentClipId() != "BossPattern2")
	{
		monster.Play("BossPattern2");
	}

	sf::Vector2f BosPos = GetPosition();
	if (monster.GetTotalFrame() - monster.GetCurFrame() <= 10)
	{
		if (isone)
		{
			sceneDev1->SpawnMonsters(2, BosPos, (Monster::Types)0);
			sceneDev1->SpawnMonsters(1, BosPos, (Monster::Types)1);
			isone = false;
		}
	}
}

void Monster::BossPattern3(float dt)
{
	if (monster.GetCurrentClipId() != "BossPattern3")
	{
		monster.Play("BossPattern3");
	}
}

void Monster::BossPattern4(float dt)
{
	if (monster.GetCurrentClipId() != "BossPattern4")
	{
		monster.Play("BossPattern4");

	}

}

void Monster::BossDefault(float dt)
{
	BossMove(dt);
	attackTimer += dt;
	//if (attackTimer > attackRate)
	//{


		SpawnBullet((MonsterBullet::Types)0);
		Shoot();

	//	attackTimer = 0;
	//}
}

void Monster::GoombaMove(float dt)
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

	if (hp <= 0 && monsterType != Types::Boss)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		if (scene != nullptr)
		{
			sceneDev1->OnDieMonster(this);
		}
	}
	else if (hp <= 0 && monsterType == Types::Boss)
	{

		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);

		if (scene != nullptr)
		{
			sceneDev1->OnDieMonster(this);
			sceneDev1->bossdie = true;
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
		// �÷��̾� �ǰ�
		player->OnHitted(damage);
	}

}



void Monster::GetMap2(const sf::FloatRect& mapsize)
{
	this->mapsize = mapsize;
}


void Monster::Shoot()
{

	playerlook = Utils::Normalize(player->GetPosition() - GetPosition());
	monsterbullet->Fire(GetPosition(), playerlook);

}

void Monster::SpawnBullet(MonsterBullet::Types t)
{
	monsterbullet = poolBullets.Get();
	monsterbullets = poolBullets.GetUseList();
	monsterbullet->SetType(t);
	monsterbullet->SetMapSize(mapsize);
	playerlook = Utils::Normalize(player->GetPosition() - GetPosition());
	monsterbullet->Fire(GetPosition(), playerlook);
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	sceneDev1->AddGo(monsterbullet);

	if (t == MonsterBullet::Types::Pattern31)
	{
		SpawnBullet2(10, MonsterBullet::Types::Pattern32);
	}
}

void Monster::Pattern3Bullet(MonsterBullet::Types t)
{
	monsterbullet = poolBullets.Get();
	monsterbullets = poolBullets.GetUseList();
	monsterbullet->SetType(t);
	monsterbullet->SetMapSize(mapsize);
	playerlook = Utils::Normalize(player->GetPosition() - GetPosition());

	pattern31BulletPos = GetPosition();
	monsterbullet->Fire(pattern31BulletPos, playerlook);
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	sceneDev1->AddGo(monsterbullet);

	if (t == MonsterBullet::Types::Pattern31)
	{
		SpawnBullet2(50, MonsterBullet::Types::Pattern32);
	}
}


void Monster::SpawnBullet2(int count, MonsterBullet::Types t)
{
	for (int i = 0; i < count; i++)
	{
		sf::Vector2f a;
		a.x = Utils::RandomRange(-1.f, 1.f);
		a.y = Utils::RandomRange(-1.f, 1.f);
		monsterbullet = poolBullets.Get();
		monsterbullets = poolBullets.GetUseList();
		monsterbullet->SetType(t);
		monsterbullet->SetMapSize(mapsize);

		if (t == MonsterBullet::Types::Pattern4)
		{
			monsterbullet->Fire(sf::Vector2f(GetPosition().x, sprite.getGlobalBounds().top - 30), a);
		}
		else
		monsterbullet->Fire(GetPosition(), a);
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
		sceneDev1->AddGo(monsterbullet);

	}
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

//void Monster::BossHpUI()
//{
//	Scene* scene = SCENE_MGR.GetCurrScene();
//	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
//	bossHpbar = ((SpriteGo*)sceneDev1->AddGo(new SpriteGo("graphics/BossHpBar.png", "BossHpBar")));
//	bossMaxHpBar = ((SpriteGo*)sceneDev1->AddGo(new SpriteGo("graphics/BossHpBar.png", "BossHpBar")));
//
//
//
//
//	bossHpbar->SetOrigin(Origins::ML);
//	bossHpbar->sortLayer = 106;
//	bossMaxHpBar->SetOrigin(Origins::ML);
//	bossMaxHpBar->sprite.setColor({ 0,0,0,150 });
//	bossMaxHpBar->sprite.setPosition(windowsize.x *0.5, windowsize.y * 0.5);
//	bossHpbar->sprite.setPosition(windowsize.x * 0.5, windowsize.y * 0.5);
//	bossMaxHpBar->sortLayer = 106;
//
//	bossHpbar->SetActive(true);
//	bossMaxHpBar->SetActive(true);
//
//	//bossHpbar->sprite.setSize({ (static_cast<float>(hp) / static_cast<float>(maxHp)) * 50.f, 7.f });
//}