#include "stdafx.h"
#include "MonsterTable.h"
#include "rapidcsv.h"


const MonsterInfo& MonsterTable::Get(Monster::Types id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "좀비 못찾았음" << std::endl;
	}

	return find->second;
}

bool MonsterTable::Load()
{
	std::string monsterfile = "tables/MonsterTypes.csv";
	MonsterInfo monsterinfo;

	rapidcsv::Document doc(monsterfile);
	std::vector<int> ids = doc.GetColumn<int>(0);
	std::vector<std::string> zombieID = doc.GetColumn<std::string>(1);
	std::vector<float> zombieSpeed = doc.GetColumn<float>(2);
	std::vector<int> zombieHP = doc.GetColumn<int>(3);
	std::vector<int> zombieDamage = doc.GetColumn<int>(4);
	std::vector<float> attactRate = doc.GetColumn<float>(5);

	for (int i = 0; i < ids.size(); ++i)
	{
		monsterinfo.monsterType = (Monster::Types)ids[i];
		monsterinfo.textureId = zombieID[i];
		monsterinfo.speed = zombieSpeed[i];
		monsterinfo.maxHP = zombieHP[i];
		monsterinfo.damage = zombieDamage[i];
		monsterinfo.attackRate = attactRate[i];

		table.insert({ (Monster::Types)ids[i] ,monsterinfo });
	}
	return true;
}

void MonsterTable::Release()
{
	table.clear();
}

