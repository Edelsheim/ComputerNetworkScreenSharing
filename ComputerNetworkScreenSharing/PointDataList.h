#pragma once

#include <iostream>
#include <concurrent_unordered_map.h>
#include <concurrent_vector.h>

#include "pch.h"
#include "PointData.h"
typedef Concurrency::concurrent_vector<PointData> PointDataVector;
typedef Concurrency::concurrent_unordered_map<std::wstring, PointDataVector*> PointDataListMap;

class PointDataList
{
private:
	PointDataList() {};
	~PointDataList() {
		PointDataListMap::iterator a = point_data_map.begin();
		while (1)
		{
			if (a == point_data_map.end())
				break;

			if (a->second != nullptr)
				delete a->second;
			a->second = nullptr;

			a++;
		}
		point_data_map.clear();
	};
public:

	static PointDataList* GetQueue() {
		static PointDataList q;
		return &q;
	}

	PointDataListMap point_data_map;
	void Insert(std::wstring key, PointData value)
	{
		if (point_data_map.find(key) == point_data_map.end())
		{
			// not found
			PointDataVector *vector = new PointDataVector();
			vector->push_back(value);
			point_data_map.insert(std::make_pair(key, vector));
		}
		else
		{
			// found
			PointDataVector* vector = point_data_map.at(key);
			vector->push_back(value);
		}
	}

	void Insert(std::wstring key, CPoint point, char type, std::string id)
	{
		PointData point_data;
		point_data.type = type;
		point_data.x = point.x;
		point_data.y = point.y;
		int i = 0;
		for (int i = 0; i != CLIENT_NAME_SIZE - 1; i++)
			point_data.id[i] = id.c_str()[i];
		point_data.id[i] = '\0';
		Insert(key, point_data);
	}

	PointDataVector* GetData(std::wstring key)
	{
		if (point_data_map.find(key) == point_data_map.end())
		{
			return nullptr;
		}
		else
		{
			PointDataVector* a = point_data_map.at(key);
			return a;
		}
	}

	void ClearWithKey(std::wstring key)
	{
		if (point_data_map.find(key) != point_data_map.end())
		{
			point_data_map.at(key)->clear();
		}
	}
};

