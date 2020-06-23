#pragma once

#include <iostream>
#include <concurrent_unordered_set.h>

typedef Concurrency::concurrent_unordered_set<std::wstring> ConnectSet;

class ConnectList
{
private:
	ConnectList() {};
	~ConnectList() {};

	ConnectSet connectSet;
public:
	static ConnectList* GetConnectList()
	{
		static ConnectList cl;
		return &cl;
	}

	bool Insert(std::wstring connectInfo)
	{
		// Find == false is not have
		if (Find(connectInfo) == false)
		{
			connectSet.insert(connectInfo);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Find(std::wstring connectInfo)
	{
		if (connectSet.find(connectInfo) == connectSet.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
};

