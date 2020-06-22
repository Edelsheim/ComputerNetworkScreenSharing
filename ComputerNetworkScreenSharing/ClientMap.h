#pragma once

#include <iostream>
#include <concurrent_unordered_map.h>
typedef Concurrency::concurrent_unordered_map<std::wstring, std::wstring> ClientInfoMap;

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

	void Insert(std::pair<std::wstring, std::wstring> pair)
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

	void Insert(std::wstring key, std::wstring value)
	{
		Insert(std::make_pair(key, value));
	}


};

