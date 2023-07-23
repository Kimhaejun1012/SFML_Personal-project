#pragma once
#include "DataTable.h"
#include "MonsterBullet.h"

struct BulletInfo
{
	MonsterBullet::Types bulletType;
	float speed;
	int damage;
	float attackRate;
	int bulletCount;
};

class BulletTable : public DataTable
{
protected:
	std::unordered_map<MonsterBullet::Types, BulletInfo> table;

public:
	BulletTable() : DataTable(DataTable::Ids::MonsterBullet) {};
	virtual ~BulletTable() override = default;

	virtual bool Load() override;
	virtual void Release() override;

	const BulletInfo* Get(MonsterBullet::Types id);

};

