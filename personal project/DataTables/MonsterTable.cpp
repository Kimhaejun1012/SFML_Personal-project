#include "stdafx.h"
#include "MonsterTable.h"
#include "rapidcsv.h"


const MonsterInfo* MonsterTable::Get(Monster::Types id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "좀비 못찾았음" << std::endl;
		return nullptr;
	}

	return &(find->second);
}

bool MonsterTable::Load()
{
	std::string monsterfile = "tables/MonsterTable.csv";


	rapidcsv::Document doc(monsterfile);
	std::vector<int> ids = doc.GetColumn<int>(0);
	//std::vector<std::string> monsterID = doc.GetColumn<std::string>(1);
	std::vector<float> monsterSpeed = doc.GetColumn<float>(1);
	std::vector<int> monsterHP = doc.GetColumn<int>(2);
	std::vector<int> monsterDamage = doc.GetColumn<int>(3);
	std::vector<float> attactRate = doc.GetColumn<float>(4);

	for (int i = 0; i < ids.size(); ++i)
	{
		MonsterInfo monsterinfo;
		monsterinfo.monsterType = (Monster::Types)ids[i];
		//monsterinfo.textureId = monsterID[i];
		monsterinfo.speed = monsterSpeed[i];
		monsterinfo.maxHP = monsterHP[i];
		monsterinfo.damage = monsterDamage[i];
		monsterinfo.attackRate = attactRate[i];

		table.insert({ (Monster::Types)ids[i] ,monsterinfo });
	}
	return true;
}

void MonsterTable::Release()
{
	table.clear();
}

