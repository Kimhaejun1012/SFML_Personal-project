#include "stdafx.h"
#include "Player.h"
#include "SceneDev1.h"
#include "Bullet.h"
#include "Monster.h"
#include "Scene.h"

//몬스터 안죽음, 몬스터한테 안나감, 몬스터 닿아도 안사라짐

void Player::Init()
{
	SpriteGo::Init();
	increaseDamage = false;

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
	bulletCount = 25;
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

	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}

	poolBullets.Clear();
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

	tick -= dt;
	animation.Update(dt);
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y= INPUT_MGR.GetAxis(Axis::Vertical);

	PlayerMove(dt);
	LookMonster();
	Shoot();
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
	if (direction == sf::Vector2f(0.f, 0.f) && tick < 0.1f)
	{
		tick = 0.5f;
		int count = 0;
		Bullet*bullet = poolBullets.Get();
		if (increaseDamage)
		{
			increaseDamage = false;
		}
		bullet->SetDamage(bulletDamage);

		float modifiedAngle = Utils::Angle(monsterlook);  // 기존 각도 계산
		//float additionalAngle = (count % 2 == 1) ? 15.f * count : -15.f * count;  // 추가 각도 계산
		float additionalAngle = -15 + (30 / bulletCount * count); //(count == 0) ? 0.f : ((count % 2 == 1) ? 15.f * count : -15.f * count);
		//float additionalAngle =  (360 / bulletCount * count);
		float finalAngle = modifiedAngle + additionalAngle;  // 기존 각도와 추가 각도 합산
		sf::Vector2f fireDirection = Utils::DirectionFromAngle(finalAngle);  // 총알 발사 각도 계산

		bullet->Fire(GetPosition(), fireDirection, 500.f);

		if (scene != nullptr)
		{
			bullet->SetMonsterList(sceneDev1->GetMonsterList());
			scene->AddGo(bullet);
		}
		count++;
	}
}

void Player::LookMonster()
{
	//sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(position);
	if (direction == sf::Vector2f(0.f, 0.f))
	{
		monsterlook = Utils::Normalize(monster->GetPosition() - GetPosition());
		sprite.setRotation(Utils::Angle(monsterlook));
	}
	else
	{
		sprite.setRotation(0.f);
	}
}

void Player::MoveStop()
{
	direction == sf::Vector2f(0.f, 0.f);
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

void Player::SetMonsterList(const std::list<Monster*>* list)
{
	monsters = list;
}