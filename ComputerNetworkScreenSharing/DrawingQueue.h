#pragma once

#include <concurrent_queue.h>
#include "PointData.h"

class DrawingQueue
{
private:
	Concurrency::concurrent_queue<PointData> client_queue;
	Concurrency::concurrent_queue<PointData> server_queue;
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

	void Push(PointData point, std::string queueType)
	{
		if (queueType.compare("server") == 0)
			server_queue.push(point);
		else
			client_queue.push(point);
	}

	void Push(CPoint point, char type, std::string id, std::string queueType)
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
		Push(point_data, queueType);
	}

	PointData Pop(std::string queueType)
	{
		PointData point;
		bool check = false;

		if (queueType.compare("server") == 0)
		{
			check = server_queue.try_pop(point);
		}
		else
		{
			check = client_queue.try_pop(point);
		}
		
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

