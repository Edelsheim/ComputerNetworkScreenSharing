#pragma once

#include "pch.h"
#include <concurrent_queue.h>

#include <string>
#include "MessageQueue.h"

class DrawingQueue
{
private:
	Concurrency::concurrent_queue<CPoint> queue;
public:
	static DrawingQueue* GetQueue()
	{
		static DrawingQueue q;
		return &q;
	}

	void Push(CPoint point)
	{
		queue.push(point);
		MessageQueue::GetInstance()->Push(std::to_wstring(point.x) + L"," + std::to_wstring(point.y));
	}

	CPoint Pop()
	{
		CPoint point;
		bool check = queue.try_pop(point);
		if (check)
		{
			return point;
		}
		else
		{
			point.x = -1;
			point.y = -1;
			return point;
		}
	}
};

