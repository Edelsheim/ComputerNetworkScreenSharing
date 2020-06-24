#pragma once

#include <concurrent_queue.h>
#include "PointData.h"

class DrawingQueue
{
private:
	Concurrency::concurrent_queue<PointData> queue;
public:
	static DrawingQueue* GetReceiveQueue()
	{
		static DrawingQueue receiveQueue;
		return &receiveQueue;
	}

	static DrawingQueue* GetSendQueue()
	{
		static DrawingQueue sendQueue;
		return &sendQueue;
	}

	void Push(PointData point)
	{
		queue.push(point);
	}

	void Push(CPoint point, char type, std::string id)
	{
		PointData point_data;
		point_data.type = type;

		int i = 0;
		for (i = 0; i != CLIENT_NAME_SIZE - 1; i++)
		{
			if (id.c_str()[i] == '\0')
			{
				point_data.id[i] = '\0';
				break;
			}
			point_data.id[i] = id.c_str()[i];
		}
		point_data.id[i] = '\0';

		point_data.x = point.x;
		point_data.y = point.y;
		Push(point_data);
	}

	PointData Pop()
	{
		PointData point;
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

