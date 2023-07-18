#include "stdafx.h"
#include "Player.h"
#include "SceneDev1.h"
#include "Bullet.h"
#include "Monster.h"
#include "Scene.h"
#include "UIButton.h"
#include "SpriteGo.h"
//몬스터 안죽음, 몬스터한테 안나감, 몬스터 닿아도 안사라짐



void Player::Init()
{
	SpriteGo::Init();
	increaseDamage = false;
	windowsize = FRAMEWORK.GetWindowSize();
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Idle.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Move.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Up.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("tables/Down.csv"));

	ObjectPool<Bullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr](Bullet* bullet) {
		//bullet->textureId = "tables/Bullet.csv";
		bullet->pool = ptr;
	};
	poolBullets.Init();

	animation.SetTarget(&sprite);
	SetOrigin(Origins::BC);
	bulletCount = 1;



}

void Player::Release()
{
	SpriteGo::Release();
	poolBullets.Release();
}
void Player::Reset()
{
	SpriteGo::Reset();

	animation.Play("Idle");
	SetOrigin(origin);
	SetPosition({ 0, 500 });
	SetFlipX(false);
	sprite.setScale(0.5f, 0.5f);
	Hp = MaxHp;
	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	PlayerUI();
	poolBullets.Clear();
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	Scene* scene = SCENE_MGR.GetCurrScene();
	
	tick -= dt;
	animation.Update(dt);
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y= INPUT_MGR.GetAxis(Axis::Vertical);

	PlayerMove(dt);
	LookMonster();
	Shoot();
	
	playerHp->rect.setPosition(GetPosition().x - 30, sprite.getGlobalBounds().top - 10.f);
	playerMaxHp->rect.setPosition(GetPosition().x - 30,sprite.getGlobalBounds().top - 10.f);
	playerHp->rect.setSize({ (static_cast<float>(Hp) / static_cast<float>(MaxHp)) * 50.f, 7.f });
	expbar->sprite.setScale({ (static_cast<float>(exp) / static_cast<float>(maxexp)), 1.f});
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
	//SceneDev1* sceneDev1 = dynamic_cast<SceneDev1*>(scene);
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
			float additionalAngle = -5 + (30 / bulletCount * count); //(count == 0) ? 0.f : ((count % 2 == 1) ? 15.f * count : -15.f * count);
			//float additionalAngle =  (360 / bulletCount * count);
			float finalAngle = modifiedAngle + additionalAngle;  // 기존 각도와 추가 각도 합산
			sf::Vector2f fireDirection = Utils::DirectionFromAngle(finalAngle);  // 총알 발사 각도 계산

			bullet->Fire(GetPosition(), fireDirection, 800.f);

			if (scene != nullptr)
			{
				bullet->SetMonsterList(sceneDev1->GetMonsterList());
				scene->AddGo(bullet);
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
		sf::Vector2f closestMonster = { 0.f, 0.f };
		for (const auto& monster : monsters) {
			float distance = Utils::Distance(monster->GetPosition(), GetPosition());
			if (distance < closestDistance) {
				closestMonster = monster->GetPosition();
				closestDistance = distance;
				//std::cout << closestDistance << std::endl;
			}
		}
		monsterlook = Utils::Normalize(closestMonster - GetPosition());
		//sprite.setRotation(Utils::Angle(monsterlook));
	}
	else
	{
		sprite.setRotation(0.f);
	}
}

void Player::PlayerMove(float dt)
{
	velocity.x = direction.x * speed;
	velocity.y = direction.y * speed;
	position += velocity * dt;

	if (direction.x != 0.f)
	{
		bool flip = direction.x > 0.f;
		if (GetFlipX() != flip)
		{
			SetFlipX(flip);
		}
	}

	SetPosition(position);
	if (sprite.getGlobalBounds().intersects(wallBounds))
	{
		//std::cout << "충돌함" << std::endl;
		position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
	}

	// 에니메이션
	if (animation.GetCurrentClipId() == "Idle" || animation.GetCurrentClipId() == "Up" || animation.GetCurrentClipId() == "Down")
	{
		if (direction.x != 0.f)
		{
			animation.Play("Move");
		}
	}
	else if (animation.GetCurrentClipId() == "Move")
	{
		if (direction.x == 0.f)
		{
			animation.Play("Idle");
		}
	}

	if ((animation.GetCurrentClipId() == "Idle" || animation.GetCurrentClipId() == "Down" || animation.GetCurrentClipId() == "Move") && direction.x == 0)
	{
		//animation.GetCurrentClipId() = "Up";
		if (direction.y < 0.f)
		{
			animation.Play("Up");
		}
	}
	else if (animation.GetCurrentClipId() == "Up")
	{
		if (direction.y == 0.f)
		{
			animation.Play("Idle");
		}
	}
	if ((animation.GetCurrentClipId() == "Idle" || animation.GetCurrentClipId() == "Up" || animation.GetCurrentClipId() == "Move") && direction.x == 0)
	{
		if (direction.y > 0.f)
		{
			animation.Play("Down");
		}
	}
	else if (animation.GetCurrentClipId() == "Down")
	{
		if (direction.y == 0.f)
		{
			animation.Play("Idle");
		}
	}
}

void Player::ClearBullet()
{
	Scene* scene = SCENE_MGR.GetCurrScene();
	for (auto bullet : poolBullets.GetUseList())
	{
		scene->RemoveGo(bullet);
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
	if (Hp == 0)
	{
		isAlive = false;
		std::cout << "플레이어 죽음" << std::endl;
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
	playerMaxHp = ((SpriteGo*)scene->AddGo(new SpriteGo("", "")));
	playerHp = ((SpriteGo*)scene->AddGo(new SpriteGo("", "")));

	playerHp->rect.setSize(sf::Vector2f(50.f, 7.f));
	playerHp->rect.setFillColor(sf::Color::Green);

	maxexpbar = ((SpriteGo*)scene->AddGo(new SpriteGo("graphics/ExpBarmax.png", "maxexpbar")));
	expbar = ((SpriteGo*)scene->AddGo(new SpriteGo("graphics/ExpBar.png", "expbar")));


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
	
	std::cout << maxexpbar->sprite.getScale().x << maxexpbar->sprite.getScale().y << std::endl;

	//playerMaxHp->SetOrigin(Origins::BL);
	//playerMaxHp->sortLayer = 102;
	//playerMaxHp->sortOrder = 0;
	//playerHp->SetPosition(player->GetPosition());
	//playerMaxHp->SetPosition(player->GetPosition());
}
