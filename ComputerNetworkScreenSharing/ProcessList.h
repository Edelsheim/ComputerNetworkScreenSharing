#pragma once

#include <iostream>
#include <concurrent_unordered_map.h>
#include "DrawingView.h"

typedef Concurrency::concurrent_unordered_map<std::wstring, DrawingView> ProcessSet;

class ProcessList
{
private:
	ProcessList() {};
	~ProcessList() {};
	
	ProcessSet processSet;
public:
	static ProcessList* GetProcessList()
	{
		static ProcessList pl;
		return &pl;
	}

	void Insert(std::wstring processName, DrawingView dv)
	{
		processSet.insert(dv);
	}

	const DrawingView At(std::wstring processName)
	{
		return processSet.at(processName);
	}
};

