#pragma once

#include <iostream>
#include <concurrent_unordered_map.h>
typedef Concurrency::concurrent_unordered_map<std::wstring, std::string> ClientInfoMap;

class ClientMap
{
private:
	ClientInfoMap clientMap;
	ClientMap() {};
	~ClientMap() {};

public:
	static ClientMap* GetClientMap()
	{
		static ClientMap cm;
		return &cm;
	}

	void Insert(std::pair<std::wstring, std::string> pair)
	{
		if (clientMap.find(pair.first) == clientMap.end())
		{
			clientMap.insert(pair);
		}
		else
		{
			clientMap[pair.first] = pair.second;
		}
	}

	void Insert(std::wstring key, std::string value)
	{
		Insert(std::make_pair(key, value));
	}

	std::string GetValue(std::wstring key)
	{
		if (clientMap.find(key) == clientMap.end())
			return "";
		else
			return clientMap.at(key);
	}
};

