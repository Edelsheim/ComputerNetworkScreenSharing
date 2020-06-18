#pragma once

#include <iostream>
#include <concurrent_queue.h>

class MessageQueue
{
private:
	MessageQueue() {};
	~MessageQueue() {};
	Concurrency::concurrent_queue<std::wstring> qeueu;

public:
	static MessageQueue* GetInstance() {
		static MessageQueue q;
		return &q;
	}

	void Push(std::wstring message)
	{
		this->qeueu.push(message);
	}

	std::wstring Pop()
	{
		std::wstring result;
		if (this->qeueu.try_pop(result)) {
			return result;
		}
		else {
			return L"";
		}
	}
};

