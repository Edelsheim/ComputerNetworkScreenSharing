#pragma once

#include <iostream>
#include <concurrent_unordered_map.h>

typedef Concurrency::concurrent_unordered_map<std::wstring, std::wstring> ConnectMap;

class ConnectList
{
private:
	ConnectList() {};
	~ConnectList() {};

	ConnectMap connectMap;
public:
	static ConnectList* GetConnectList()
	{
		static ConnectList cl;
		return &cl;
	}

	bool Insert(std::wstring processName, std::wstring connectInfo)
	{
		if (FindFromConnectInfo(connectInfo) == false)
		{
			connectMap.insert(std::make_pair(processName, connectInfo));
			return true;
		}
		else
		{
			return false;
		}
	}

	bool FindFromProcessName(std::wstring processName)
	{
		if (connectMap.find(processName) == connectMap.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool FindFromConnectInfo(std::wstring connectInfo)
	{
		ConnectMap::iterator iterator = connectMap.begin();
		while (1)
		{
			if (iterator == connectMap.end())
				break;

			if (iterator->second.compare(connectInfo) == 0)
				return true;

			iterator++;
		}
		return false;
	}

	const std::wstring FindProcessNameWithConnectInfo(std::wstring connectInfo)
	{
		ConnectMap::iterator iterator = connectMap.begin();
		while (1)
		{
			if (iterator == connectMap.end())
				break;

			if (iterator->second.compare(connectInfo) == 0)
				return iterator->first;

			iterator++;
		}

		return L"";
	}
};

