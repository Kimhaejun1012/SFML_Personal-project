#include "stdafx.h"
#include "BulletTable.h"
#include "rapidcsv.h"


const BulletInfo* BulletTable::Get(MonsterBullet::Types id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "몬스터총알 못찾았음" << std::endl;
		return nullptr;
	}

	return &(find->second);
}

bool BulletTable::Load()
{
	std::string bulletfile = "tables/BulletTable.csv";


	rapidcsv::Document doc(bulletfile);
	std::vector<int> ids = doc.GetColumn<int>(0);
	std::vector<float> bulletSpeed = doc.GetColumn<float>(1);
	std::vector<int> bulletDamage = doc.GetColumn<int>(2);
	std::vector<float> attactRate = doc.GetColumn<float>(3);
	std::vector<int> bulletCount = doc.GetColumn<int>(4);
	for (int i = 0; i < ids.size(); ++i)
	{
		BulletInfo bulletinfo;
		bulletinfo.bulletType = (MonsterBullet::Types)ids[i];
		bulletinfo.speed = bulletSpeed[i];
		bulletinfo.damage = bulletDamage[i];
		bulletinfo.attackRate = attactRate[i];
		bulletinfo.bulletCount = bulletCount[i];
		//table.insert({ bulletinfo.bulletType ,bulletinfo });
		table.insert({ (MonsterBullet::Types)ids[i] ,bulletinfo });
	}
	return true;
}

void BulletTable::Release()
{
	table.clear();
}

