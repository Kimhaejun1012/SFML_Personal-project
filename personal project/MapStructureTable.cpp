#include "stdafx.h"
#include "MapStructureTable.h"
#include "rapidcsv.h"


bool MapStructureTable::Load()
{
	std::string sturcturefile = "map/mapStructure.csv";
	MapStructureInfo structureinfo;

	rapidcsv::Document doc(sturcturefile);
	std::vector<int> ids = doc.GetColumn<int>(0);
	std::vector<std::string> StructureID = doc.GetColumn<std::string>(1);

	for (int i = 0; i < ids.size(); ++i)
	{
		structureinfo.structureType = (MapStructure::Types)ids[i];
		structureinfo.textureId = StructureID[i];

		table.insert({ (MapStructure::Types)ids[i] ,structureinfo });
	}
	return true;
}

void MapStructureTable::Release()
{
}

const MapStructureInfo& MapStructureTable::Get(MapStructure::Types id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "???" << std::endl;
	}

	return find->second;
}
