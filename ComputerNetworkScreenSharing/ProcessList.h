#pragma once

#include <iostream>
#include <concurrent_unordered_map.h>
#include "DrawingView.h"

typedef Concurrency::concurrent_unordered_map<std::wstring, DrawingView*> ProcessMap;

class ProcessList
{
private:
	ProcessList() {};
	~ProcessList() {};
	
	ProcessMap processMap;
public:
	static ProcessList* GetProcessList()
	{
		static ProcessList pl;
		return &pl;
	}

	bool Insert(std::wstring processName, DrawingView* dv)
	{
		if (processMap.find(processName) == processMap.end())
		{
			processMap.insert(std::make_pair(processName, dv));
			return true;
		}
		else
		{
			return false;
		}
	}

	const DrawingView* At(std::wstring processName)
	{
		if (processMap.find(processName) == processMap.end())
			return nullptr;
		else
			return processMap.at(processName);
	}

	void Remove(std::wstring processName)
	{
		processMap.at(processName);
	}

	void RemoveAll()
	{
		ProcessMap::iterator iterator = processMap.begin();
		while (1)
		{
			if (iterator == processMap.end())
				break;

			delete iterator->second;
			iterator->second = nullptr;

			iterator++;
		}

		processMap.clear();
	}
};

