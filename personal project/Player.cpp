#include "stdafx.h"
#include "Player.h"
#include "SceneDev1.h"
#include "Monster.h"
#include "Scene.h"
#include "UIButton.h"
#include "SpriteGo.h"


Player::Player(const std::string& textureId, const std::string& n) : SpriteGo(textureId, n), attack(false)
{

}

void Player::Init()
{
	increaseDamage = false;
	windowsize = FRAMEWORK.GetWindowSize();
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/IdleUp.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/IdleDown.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/IdleRight.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/MoveUp.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/MoveRightUp.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/MoveRightDown.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/MoveDown.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/MoveRight.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/AttackUp.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/AttackDown.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("playercsv/AttackRight.csv"));



	animation.SetTarget(&sprite);
	SetOrigin(Origins::BC);
	bulletCount = 1;


	clipInfos.push_back({ "IdleRight", "MoveRightUp","AttackDown" ,true, Utils::Normalize({-1.f, -1.f})});
	clipInfos.push_back({ "IdleUp", "MoveUp","AttackUp" ,true, {0.f, -1.f} });
	clipInfos.push_back({ "IdleRight", "MoveRightUp", "AttackUp",false, Utils::Normalize({1.f, -1.f})});

	clipInfos.push_back({ "IdleRight", "MoveRight","AttackUp" ,true, {-1.f, 0.f}});
	clipInfos.push_back({ "IdleRight", "MoveRight","AttackUp" ,false, {1.f, 0.f}});

	clipInfos.push_back({ "IdleRight", "MoveRightDown","AttackUp" ,true, Utils::Normalize({-1.f, 1.f})});
	clipInfos.push_back({ "IdleDown", "MoveDown","AttackUp",true,{0.f, 1.f}});
	clipInfos.push_back({ "IdleRight", "MoveRightDown","AttackDown",false, Utils::Normalize({1.f, 1.f})});


}

void Player::Release()
{
	//poolBullets.Release();
}
void Player::Reset()
{
	SpriteGo::Reset();
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	PlayerReset();


	PlayerUI();
	animation.Play("IdleUp");
	SetOrigin(origin);
	SetPosition({ 0, 500 });
	SetFlipX(false);
	sprite.setScale(2.f, 2.f);
	Hp = MaxHp;
	exp = 0;

	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	poolBullets.Clear();

	ObjectPool<Bullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr](Bullet* bullet) {
		//bullet->textureId = "tables/Bullet.csv";
		bullet->pool = ptr;
	};
	poolBullets.Init();

	currentClipInfo = clipInfos[6];



	testbutton1 = (UIButton*)sceneDev1->AddGo(new UIButton("upgrade/doubleArrow.png", "mouse"));
	testbutton2 = (UIButton*)sceneDev1->AddGo(new UIButton("upgrade/doubleAttack.png", ""));
	testbutton3 = (UIButton*)sceneDev1->AddGo(new UIButton("upgrade/doubleSpeed.png", ""));
	testbutton1->SetOrigin(Origins::MC);
	testbutton1->SetPosition(windowsize.x * 0.5, windowsize.y * 0.5);
	testbutton1->sortLayer = 100;
	testbutton1->SetActive(false);
	testbutton2->SetOrigin(Origins::MC);
	testbutton2->SetPosition(testbutton1->GetPosition().x - 300, testbutton1->GetPosition().y);
	testbutton2->sortLayer = 100;
	testbutton2->SetActive(false);
	testbutton3->SetOrigin(Origins::MC);
	testbutton3->SetPosition(testbutton1->GetPosition().x + 300, testbutton1->GetPosition().y);
	testbutton3->sortLayer = 100;
	testbutton3->SetActive(false);
	testbutton1->OnClick = [this]() {
		//testbutton1->SetPlayer(this);
		//testbutton1->IncreaseBullet();
		std::cout << "1클릭" << std::endl;
		testbutton1->SetActive(false);
		testbutton2->SetActive(false);
		testbutton3->SetActive(false);
	};
	testbutton2->OnClick = [this]() {
		//testbutton2->SetPlayer(this);
		//testbutton2->IncreaseAttact();
		std::cout << "2클릭" << std::endl;
		testbutton1->SetActive(false);
		testbutton2->SetActive(false);
		testbutton3->SetActive(false);
	};

	testbutton3->OnClick = [this]() {
		//testbutton3->SetPlayer(this);
		//testbutton3->IncreaseSpeed();
		std::cout << "3클릭" << std::endl;
		testbutton1->SetActive(false);
		testbutton2->SetActive(false);
		testbutton3->SetActive(false);
	};

}

void Player::Update(float dt)
{
	//SpriteGo::Update(dt);
	Scene* scene = SCENE_MGR.GetCurrScene();

	//std::cout <<"플레이어 경험치 바 :" << expbar->GetPosition().x << expbar->GetPosition().y << std::endl;


	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	attack = true;
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	attack = false;
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
		Hp += MaxHp;
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
		std::cout << "플레이어 HP : " << Hp << std::endl;

	tick -= dt;
	animation.Update(dt);
	SetOrigin(origin);
	PlayerMove(dt);
	ShootAndLook();
	playerHp->rect.setPosition(GetPosition().x - 30, sprite.getGlobalBounds().top - 10.f);
	playerMaxHp->rect.setPosition(GetPosition().x - 30,sprite.getGlobalBounds().top - 10.f);
	playerHp->rect.setSize({ (static_cast<float>(Hp) / static_cast<float>(MaxHp)) * 50.f, 7.f });
	expbar->sprite.setScale({ (static_cast<float>(exp) / static_cast<float>(maxexp)), 1.f});

	if (exp >= maxexp)
	{
		std::cout << "나 혼자만 레벨업" << std::endl;
		testbutton1->SetActive(true);
		testbutton2->SetActive(true);
		testbutton3->SetActive(true);
		exp -= maxexp;
		maxexp *= 1.3;
	}
	//std::cout << wallBounds.top << std::endl;
}

bool Player::GetFlipX() const
{
	return filpX;
}


void Player::SetFlipX(bool filp)
{
	filpX = filp;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !filpX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}


void Player::SetWallBounds(const sf::FloatRect& bounds)
{
	wallBounds = bounds;
	wallBoundsLT = { wallBounds.left + 56 , wallBounds.top + 270 };
	wallBoundsRB = { wallBounds.left + wallBounds.width - 56, wallBounds.top + wallBounds.height - 270};
}

sf::Vector2f Player::GetDirection()
{
	return direction;
}

void Player::Shoot()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	if (direction == sf::Vector2f(0.f, 0.f) && tick < 0.1f && !monsters.empty())
	{
		tick = 0.5f;
		int count = 0;
		while (count != bulletCount)
		{
			Bullet* bullet = poolBullets.Get();
			if (increaseDamage)
			{
				increaseDamage = false;
			}
			bullet->SetDamage(bulletDamage);

			float modifiedAngle = Utils::Angle(monsterlook);  // 기존 각도 계산
			//float additionalAngle = (count % 2 == 1) ? 15.f * count : -15.f * count;  // 추가 각도 계산
			//float additionalAngle = -5 + (30 / bulletCount * count); //(count == 0) ? 0.f : ((count % 2 == 1) ? 15.f * count : -15.f * count);
			//float additionalAngle =  (360 / bulletCount * count);
			float finalAngle = modifiedAngle; // + additionalAngle;  // 기존 각도와 추가 각도 합산
			sf::Vector2f fireDirection = Utils::DirectionFromAngle(finalAngle);  // 총알 발사 각도 계산

			bullet->Fire(GetPosition(), fireDirection, 800.f);

			if (scene != nullptr)
			{
				bullet->SetMonsterList(sceneDev1->GetMonsterList());
				sceneDev1->AddGo(bullet);
			}
			count++;
		}
	}
}

void Player::LookMonster()
{
	//플레이어에 있은 죽은 몬스터 빼줘야함
	//auto minValue = *std::min_element(monsters.begin(), monsters.end());
	//플레이어랑 몬스터랑 거리를 계산해서 작은값을 리턴해주는 람다식을 짜자
	//sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);
	if (direction == sf::Vector2f(0.f, 0.f))
	{
		//float minValue = *std::min_element(monsters.begin(), monsters.end());
		float closestDistance = std::numeric_limits<float>::max();
		closestMonster = { 0.f, 0.f };
		for (const auto& monster : monsters) {
			float distance = Utils::Distance(monster->GetPosition(), GetPosition());
			if (distance < closestDistance) {
				closestMonster = monster->GetPosition();
				closestDistance = distance;
				//std::cout << closestDistance << std::endl;
			}
		}
		monsterlook = Utils::Normalize(closestMonster - GetPosition());
		//sprite.setRotation(Utils::Angle(closestMonster));
	}
	//else
	//{
	//	sprite.setRotation(0.f);
	//}
}

void Player::ShootAndLook()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);

	// 플레이어 주변에 있는 몬스터를 찾기 위한 로직
	float closestDistance = std::numeric_limits<float>::max();
	closestMonster = { 0.f, 0.f };
	for (const auto& monster : monsters) {
		float distance = Utils::Distance(monster->GetPosition(), GetPosition());
		if (distance < closestDistance) {
			closestMonster = monster->GetPosition();
			closestDistance = distance;
		}
	}
	monsterlook = Utils::Normalize(closestMonster - GetPosition());
	

	//if (direction == sf::Vector2f(0.f, 0.f) && tick < 0.1f && !monsters.empty())
	if (direction == sf::Vector2f(0.f, 0.f) && tick < 0.1f && attack)
	{
		tick = 0.5f;
		int count = 0;
		while (count != bulletCount)
		{
			Bullet* bullet = poolBullets.Get();
			if (increaseDamage)
			{
				increaseDamage = false;
			}
			bullet->SetDamage(bulletDamage);

			//float modifiedAngle = Utils::Angle(monsterlook);  // 기존 각도 계산
			//float additionalAngle = (count % 2 == 1) ? 15.f * count : -15.f * count;  // 추가 각도 계산
			//float additionalAngle = -5 + (30 / bulletCount * count); //(count == 0) ? 0.f : ((count % 2 == 1) ? 15.f * count : -15.f * count);
			//float additionalAngle =  (360 / bulletCount * count);
			//float finalAngle = modifiedAngle; // + additionalAngle;  // 기존 각도와 추가 각도 합산
			//sf::Vector2f fireDirection = Utils::DirectionFromAngle(finalAngle);  // 총알 발사 각도 계산

			bullet->Fire(GetPosition(), monsterlook, 800.f);

			if (scene != nullptr)
			{
				bullet->SetMonsterList(sceneDev1->GetMonsterList());
				sceneDev1->AddGo(bullet);
			}
			count++;
		}
	}
}


void Player::PlayerMove(float dt)
{
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);
	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 1.f)
	{
		direction /= magnitude;
	}

	position += direction * speed * dt;
	SetPosition(position);


	if (direction.x != 0.f || direction.y != 0.f)
	{
		auto min = std::min_element(clipInfos.begin(), clipInfos.end(),
			[this](const ClipInfo& lhs, const ClipInfo& rhs) {
				return Utils::Distance(lhs.point, direction) < Utils::Distance(rhs.point, direction);
			});
		currentClipInfo = *min;
	}

		std::string clipId = magnitude == 0.f ?
		(monsters.empty() ? currentClipInfo.idle : closestMonster.y < GetPosition().y ? "AttackUp" : "AttackDown") : currentClipInfo.move;

		if (GetFlipX() != currentClipInfo.flipX)
		{
			SetFlipX(currentClipInfo.flipX);
		}

		if (animation.GetCurrentClipId() != clipId)
		{
			animation.Play(clipId);
		}
		

	//if (sprite.getGlobalBounds().intersects(wallBounds))
	//{
	//	//std::cout << "충돌함" << std::endl;
	//	position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
	//}

}

void Player::ClearBullet()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	for (auto bullet : poolBullets.GetUseList())
	{
		sceneDev1->RemoveGo(bullet);
	}
	poolBullets.Clear();
}

void Player::SetMonster(Monster* monster)
{
	this->monster = monster;
}

void Player::SetMonsterList(const std::list<Monster*> list)
{
	monsters = list;
}

void Player::RemoveMonster(Monster* monster)
{
	monsters.remove(monster);
}

void Player::OnHitted(int damage)
{
	if (!isAlive)
		return;
	Hp = std::max(Hp - damage, 0);
	std::cout << Hp << std::endl;
	if (Hp == 0)
	{
		isAlive = false;
		std::cout << "플레이어 죽음!!!" << std::endl;
	}
}

int Player::GetHp() const
{
	return Hp;
}

int Player::ReturnExp()
{
	return exp;
}

void Player::GetExp(int exp)
{
	bool isexp = true;
	if(isexp)
	this->exp += exp;
	isexp = false;
}

int Player::ReturnMaxExp()
{
	return maxexp;
}

void Player::LevelUp()
{
	isplaying = false;
}

void Player::GetMaxExp(float exp)
{
	this->exp -= maxexp;
	maxexp *= exp;
}

void Player::IncreaseBullet()
{
	bulletCount += 2;
}

void Player::IncreaseAttack()
{
	bulletDamage += 30;
}

void Player::IncreaseSpeed()
{
	speed += 300;
}

void Player::PlayerUI()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	playerMaxHp = ((SpriteGo*)sceneDev1->AddGo(new SpriteGo("", "")));
	playerHp = ((SpriteGo*)sceneDev1->AddGo(new SpriteGo("", "")));
	maxexpbar = ((SpriteGo*)sceneDev1->AddGo(new SpriteGo("graphics/ExpBarmax.png", "maxexpbar")));
	expbar = ((SpriteGo*)sceneDev1->AddGo(new SpriteGo("graphics/ExpBar.png", "expbar")));
	playerHp->rect.setSize(sf::Vector2f(50.f, 7.f));
	playerHp->rect.setFillColor(sf::Color::Green);




	expbar->SetOrigin(Origins::BL);
	expbar->sortLayer = 101;
	//expbar->sprite.setScale(0, 0);
	maxexpbar->sprite.setPosition(windowsize.x * 0.27, 50);
	expbar->sprite.setPosition(windowsize.x * 0.27, 50);
	maxexpbar->SetOrigin(Origins::BL);
	maxexpbar->sortLayer = 101;
	sf::Color expbar = maxexpbar->sprite.getColor();
	maxexpbar->sprite.setColor({0,0,0,150});
	playerMaxHp->rect.setSize(sf::Vector2f(50.f, 7.f));
	playerMaxHp->rect.setFillColor(sf::Color::Black);
	sf::Color aaa = playerMaxHp->sprite.getColor();
	aaa.a = 125;
	playerMaxHp->rect.setFillColor(aaa);

}

void Player::OnHitBullet(int damage)
{
	Hp -= damage;
	if (Hp <= 0)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);

		if (scene != nullptr)
		{
			OnDiePlayer();
		}
	}
}

void Player::OnDiePlayer()
{
	std::cout << "플레이어 죽음" << Hp << std::endl;
}

Player* Player::GetPlayer()
{
	return player;
}

void Player::PlayerReset()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
	sceneDev1->RemoveGo(playerHp);
	sceneDev1->RemoveGo(playerMaxHp);
	sceneDev1->RemoveGo(expbar);
	sceneDev1->RemoveGo(maxexpbar);
	sceneDev1->RemoveGo(testbutton1);
	sceneDev1->RemoveGo(testbutton2);
	sceneDev1->RemoveGo(testbutton3);
}