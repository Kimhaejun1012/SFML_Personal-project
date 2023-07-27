#include "stdafx.h"
#include "Player.h"
#include "SceneDev1.h"
#include "Monster.h"
#include "Scene.h"
#include "UIButton.h"
#include "SpriteGo.h"
#include "TileMap.h"

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

	windowsize = FRAMEWORK.GetWindowSize();

	animation.SetTarget(&sprite);
	SetOrigin(Origins::BC);


	clipInfos.push_back({ "IdleRight", "MoveRightUp","AttackDown" ,true, Utils::Normalize({-1.f, -1.f})});
	clipInfos.push_back({ "IdleUp", "MoveUp","AttackUp" ,true, {0.f, -1.f} });
	clipInfos.push_back({ "IdleRight", "MoveRightUp", "AttackUp",false, Utils::Normalize({1.f, -1.f})});

	clipInfos.push_back({ "IdleRight", "MoveRight","AttackUp" ,true, {-1.f, 0.f}});
	clipInfos.push_back({ "IdleRight", "MoveRight","AttackUp" ,false, {1.f, 0.f}});

	clipInfos.push_back({ "IdleRight", "MoveRightDown","AttackUp" ,true, Utils::Normalize({-1.f, 1.f})});
	clipInfos.push_back({ "IdleDown", "MoveDown","AttackUp",true,{0.f, 1.f}});
	clipInfos.push_back({ "IdleRight", "MoveRightDown","AttackDown",false, Utils::Normalize({1.f, 1.f})});


	playerrec.setOutlineColor(sf::Color::Yellow);
	playerrec.setFillColor(sf::Color::Transparent);
	playerrec.setOutlineThickness(3.f);
	playerrec.setSize(sf::Vector2f(45.f, 30.f));
	Utils::SetOrigin(playerrec, Origins::BC);

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
	SetPosition((wallBounds.left + wallBounds.width) * 0.5, wallBounds.top + (wallBounds.height * 0.9));
	SetFlipX(false);
	sprite.setScale(2.f, 2.f);
	level = 1;
	MaxHp = 1;
	Hp = MaxHp;
	exp = 0;

	bulletDamage = 100;
	bulletCount = 1;
	attackspeed = 0.7;
	speed = 500.f;
	maxexp = 100;


	for (auto bullet : poolBullets.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
	}
	poolBullets.Clear();

	ObjectPool<Bullet>* ptr = &poolBullets;
	poolBullets.OnCreate = [ptr](Bullet* bullet) {
		bullet->pool = ptr;
	};
	poolBullets.Init();

	currentClipInfo = clipInfos[6];

	upgradeOptions = {
"upgrade/doubleArrow.png",
"upgrade/attackUp.png",
"upgrade/healthUp.png",
"upgrade/shootSpeed.png",
"upgrade/speedUp.png",
"upgrade/fullHealth.png",
	};



	for (int i = 0; i < 6; ++i)
	{
		UIButton* testbutton1 = (UIButton*)sceneDev1->AddGo(new UIButton(upgradeOptions[i], std::to_string(i)));
		UIButton* testbutton2 = (UIButton*)sceneDev1->AddGo(new UIButton(upgradeOptions[i], std::to_string(i)));
		UIButton* testbutton3 = (UIButton*)sceneDev1->AddGo(new UIButton(upgradeOptions[i], std::to_string(i)));
		testbutton1->SetOrigin(Origins::MC);
		testbutton1->SetPosition(windowsize.x * 0.5, windowsize.y * 0.5);
		testbutton1->sortLayer = 105;
		testbutton1->SetActive(false);
		testbutton1->sprite.setScale(0.7, 0.7);
		testbutton2->sprite.setScale(0.7, 0.7);
		testbutton3->sprite.setScale(0.7, 0.7);
		upgradeButtons1.push_back(testbutton1);
		upgradeButtons2.push_back(testbutton2);
		upgradeButtons3.push_back(testbutton3);
		testbutton2->SetOrigin(Origins::MC);
		testbutton2->SetPosition(windowsize.x * 0.5, windowsize.y * 0.5);
		testbutton2->sortLayer = 105;
		testbutton2->SetActive(false);
		testbutton3->SetOrigin(Origins::MC);
		testbutton3->SetPosition(windowsize.x * 0.5, windowsize.y * 0.5);
		testbutton3->sortLayer = 105;
		testbutton3->SetActive(false);

	}
	

}

void Player::Update(float dt)
{
	//SpriteGo::Update(dt);
	Scene* scene = SCENE_MGR.GetCurrScene();

	if (Hp <= 0)
	{
		isAlive = false;
		std::cout << "플레이어 죽음!!!" << std::endl;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	attack = true;
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	attack = false;
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
		Hp += MaxHp;
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
		std::cout << "플레이어 HP : " << Hp << std::endl;

	if (damagetick >= 0.f)
	{
		damagetick -= dt;
	}
	playerrec.setPosition(GetPosition());

	attackspeedfull += dt;
	tick -= dt;
	animation.Update(dt);
	//SetOrigin(origin);

	if (isAlive && isplaying)
	{
		PlayerMove(dt);
		ShootAndLook();
	}

	playerHp->rect.setPosition(GetPosition().x - 20, sprite.getGlobalBounds().top - 10.f);
	playerMaxHp->rect.setPosition(GetPosition().x - 20,sprite.getGlobalBounds().top - 10.f);
	playerHp->rect.setSize({ (static_cast<float>(Hp) / static_cast<float>(MaxHp)) * 50.f, 7.f });
	expbar->sprite.setScale({ (static_cast<float>(exp) / static_cast<float>(maxexp)), 1.f});


	if (exp >= maxexp)
	{
		isplaying = false;


			std::cout << levelupgg;

		if(!stop)
		{
			std::cout << "stop";
			levtick -= dt;
			levelupgg -= dt;
			if (isone)
			{
				std::cout << "isone";
				level1 = Utils::RandomRange(0, 6);
				level2 = Utils::RandomRange(0, 6);
				level3 = Utils::RandomRange(0 ,6);
				upgradeButtons1[level1]->SetActive(true);
				upgradeButtons1[level1]->SetPosition(windowsize.x * 0.15, 333);
				upgradeButtons2[level2]->SetActive(true);
				upgradeButtons2[level2]->SetPosition(windowsize.x * 0.5, 333);
				upgradeButtons3[level3]->SetActive(true);
				upgradeButtons3[level3]->SetPosition(windowsize.x * 0.85, 333);
				isone = false;
			}
			if (levtick <= 0)
			{
				upgradeButtons1[level1]->SetActive(false);
				upgradeButtons2[level2]->SetActive(false);
				upgradeButtons3[level3]->SetActive(false);
				isone = true;
				levtick = 0.07;
			}
		}

		if (levelupgg <= 0)
		{
			stop = true;
		}

		if(stop)
		{
			upgradeButtons1[level1]->OnClick = [this]() {
				//testbutton1->SetPlayer(this);
				std::cout << "업그레이드 : " << level2 << std::endl;
				std::cout << "1클릭" << std::endl;

				switch (level1)
				{
				case 0:
					bulletCount++;
					break;
				case 1:
					bulletDamage += 50;
					break;
				case 2:
					Hp += 200;
					MaxHp += 200;
					break;
				case 3:
					attackspeed -= 0.5;
					break;
				case 4:
					speed += 500.f;
					break;
				case 5:
					Hp = MaxHp;
					break;
				}

				upgradeButtons1[level1]->SetActive(false);
				upgradeButtons2[level2]->SetActive(false);
				upgradeButtons3[level3]->SetActive(false);
				levelupgg = 4.0f;
				isplaying = true;
				stop = false;
				exp -= maxexp;
				maxexp *= 1.3;
			};

			upgradeButtons2[level2]->OnClick = [this]() {
			
				switch (level2)
				{
				case 0:
					bulletCount++;
					std::cout << bulletCount;
					break;
				case 1:
					bulletDamage += 50;
					std::cout << bulletDamage;
					break;
				case 2:
					Hp += 200;
					MaxHp += 200;
					std::cout << Hp;
					std::cout << MaxHp;
					break;
				case 3:
					attackspeed -= 0.5;
					std::cout << attackspeed;
					break;
				case 4:
					speed += 500.f;
					std::cout << speed;
					break;
				case 5:
					Hp = MaxHp;
					std::cout << Hp;
					break;
				}

				upgradeButtons1[level1]->SetActive(false);
				upgradeButtons2[level2]->SetActive(false);
				upgradeButtons3[level3]->SetActive(false);
				levelupgg = 4.0f;
				isplaying = true;
				stop = false;
				exp -= maxexp;
				maxexp *= 1.3;
			};

			upgradeButtons3[level3]->OnClick = [this]() {

				switch (level3)
				{
				case 0:
					bulletCount++;
					break;
				case 1:
					bulletDamage += 50;
					break;
				case 2:
					Hp += 200;
					MaxHp += 200;
					break;
				case 3:
					attackspeed -= 0.5;
					break;
				case 4:
					speed += 500.f;
					break;
				case 5:
					Hp = MaxHp;
					break;
				}
				std::cout << "업그레이드 : " << level2 << std::endl;
				std::cout << "3클릭" << std::endl;
				upgradeButtons1[level1]->SetActive(false);
				upgradeButtons2[level2]->SetActive(false);
				upgradeButtons3[level3]->SetActive(false);
				levelupgg = 4.0f;
				isplaying = true;
				stop = false;
				exp -= maxexp;
				maxexp *= 1.3;
			};
		}


		//std::cout << "나 혼자만 레벨업" << std::endl;
		//testbutton1->SetActive(true);
		//testbutton2->SetActive(true);
		//testbutton3->SetActive(true);
		//exp -= maxexp;
		//maxexp *= 1.3;
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
	window.draw(playerrec);
}


void Player::SetWallBounds(const sf::FloatRect& bounds)
{
	wallBounds = bounds;
	wallBoundsLT = { wallBounds.left + 56 , wallBounds.top + 200 };
	wallBoundsRB = { wallBounds.left + wallBounds.width - 56, wallBounds.top + wallBounds.height - 150};
}

sf::Vector2f Player::GetDirection()
{
	return direction;
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

	if (direction == sf::Vector2f(0.f, 0.f) && attackspeed < attackspeedfull && !monsters.empty())
	{

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

			bullet->Fire(GetPosition() + sf::Vector2f{ 15.f * count, 15.f * count }, monsterlook, 800.f);

			if (scene != nullptr)
			{
				bullet->SetMonsterList(sceneDev1->GetMonsterList());
				sceneDev1->AddGo(bullet);
			}
			count++;
		}
		attackspeedfull = 0;
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
	SetOrigin(Origins::BC);
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


	if (sprite.getGlobalBounds().intersects(wallBounds))
	{
		//std::cout << "충돌함" << std::endl;
		position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
	}

	sf::Vector2i playerTileIndex = (sf::Vector2i)(GetPosition() / 100.f);
	int tileSize = tilemap->tiles.size();
	for (int i = 0; i < tileSize; i++)
	{

		if ((tilemap->tiles[i].texIndex == 2 || tilemap->tiles[i].texIndex == 3 || tilemap->tiles[i].texIndex == 5 || tilemap->tiles[i].texIndex == 6 || tilemap->tiles[i].texIndex == 7) && playerrec.getGlobalBounds().intersects(tilemap->tiles[i].bound, playertile))
		{
			if (playertile.width > playertile.height)
			{
				std::cout << "위아래충돌";
				if (playerrec.getGlobalBounds().top < tilemap->tiles[i].bound.top) // bottom
				{
					SetPosition(GetPosition().x, playerrec.getGlobalBounds().top + playerrec.getGlobalBounds().height);
				}
				else if (playerrec.getGlobalBounds().top + playerrec.getGlobalBounds().height > tilemap->tiles[i].bound.top) // top
				{
					SetPosition(GetPosition().x, playerrec.getGlobalBounds().top + playerrec.getGlobalBounds().height + 5);
				}
			}
			else if (playertile.width < playertile.height)
			{
				if (playerrec.getGlobalBounds().left < tilemap->tiles[i].bound.left)
				{
					SetPosition(playerrec.getGlobalBounds().left + 15, GetPosition().y);
				}
				else if (playerrec.getGlobalBounds().left + playerrec.getGlobalBounds().width > tilemap->tiles[i].bound.left) // left
				{
					SetPosition(playerrec.getGlobalBounds().left + playerrec.getGlobalBounds().width - 15, GetPosition().y);
				}
			}
		}
		if (tilemap->tiles[i].texIndex == 1 && sprite.getGlobalBounds().intersects(tilemap->tiles[i].bound))
		{
			Hp -= 0.1f;
		}
	}
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

	if(damagetick <= 0.f)
	{
		Hp = std::max(Hp - damage, 0.f);
		std::cout << Hp << std::endl;
		damagetick = 2.f;
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
	//sceneDev1->RemoveGo(testbutton1);

}

void Player::SetTileMap(TileMap* tilemap)
{
	this->tilemap = tilemap;
}

sf::RectangleShape Player::GetPlayerRec()
{
	return playerrec;
}

bool Player::isPlaying()
{
	return isplaying;
}
