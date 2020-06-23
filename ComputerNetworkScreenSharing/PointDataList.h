#pragma once

#include <iostream>
#include <concurrent_unordered_map.h>
#include <concurrent_vector.h>

#include "pch.h"
#include "PointData.h"
typedef Concurrency::concurrent_vector<PointData> PointDataVector;
typedef Concurrency::concurrent_unordered_map<std::wstring, PointDataVector> PointDataListMap;

class PointDataList
{
private:
	PointDataList() {};
	~PointDataList() {};

	PointDataListMap point_data_map;
public:

	static PointDataList* GetQueue() {
		static PointDataList q;
		return &q;
	}

	void Insert(std::wstring key, PointData value)
	{
		if (point_data_map.find(key) == point_data_map.end())
		{
			// not found
			PointDataVector vector;
			vector.push_back(value);
			point_data_map.insert(std::make_pair(key, vector));
		}
		else
		{
			// found
			point_data_map.at(key).push_back(value);
		}
	}

	void Insert(std::wstring key, CPoint point, char type, char* name)
	{
		PointData point_data;
		point_data.type = type;
		point_data.x = point.x;
		point_data.y = point.y;
		Insert(key, point_data);
	}

	PointDataVector GetValue(std::wstring key)
	{

		if (point_data_map.find(key) == point_data_map.end())
		{
			PointDataVector data;
			data.clear();
			return data;
		}
		else
		{
			return point_data_map.at(key);
		}
	}
};

