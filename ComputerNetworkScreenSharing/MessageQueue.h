#pragma once

#include <iostream>
#include <concurrent_queue.h>
#include "PointData.h"

template<class T>
class MessageQueue
{
private:
	Concurrency::concurrent_queue<T> qeueu;

public:
	MessageQueue() {};
	~MessageQueue() {};

	void Push(T message)
	{
		this->qeueu.push(message);
	}

	T Pop(bool& pop_result)
	{
		T result;
		pop_result = this->qeueu.try_pop(result);
		return result;
	}
};

namespace StaticQueue
{
	static MessageQueue<std::wstring>* GetMessageQueue() {
		static MessageQueue<std::wstring> q;
		return &q;
	};

	static MessageQueue<PointData>* GetReceiveQueue()
	{
		static MessageQueue<PointData> receiveQueue;
		return &receiveQueue;
	}

	static MessageQueue<PointData>* GetSendQueue()
	{
		static MessageQueue<PointData> sendQueue;
		return &sendQueue;
	}
}
