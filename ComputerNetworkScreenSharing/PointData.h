#pragma once

#include <string>
class PointData
{
public:
	char type;
	char name[CLIENT_NAME_SIZE];
	LONG x;
	LONG y;

	PointData() {
		type = 'c';
		name[0] = '\0';
		x = -1;
		y = -1;
	};
	~PointData() {};

	std::string ToString()
	{
		std::string result = "";
		result += type;
		result += 'x';
		
		int x_ = x;
		int y_ = y;

		int num = (int)(x_ / 1000);
		x_ = x_ % 1000;
		result += std::to_string(num);

		num = (int)(x_ / 100);
		x_ = x_ % 100;
		result += std::to_string(num);

		num = (int)(x_ / 10);
		result += std::to_string(num);

		num = (int)(x_ % 10);
		result += std::to_string(num);

		result += 'y';

		num = (int)(y_ / 1000);
		y_ = y_ % 1000;
		result += std::to_string(num);

		num = (int)(y_ / 100);
		y_ = y_ % 100;
		result += std::to_string(num);

		num = (int)(y_ / 10);
		result += std::to_string(num);

		num = (int)(y_ % 10);
		result += std::to_string(num);

		for (int i = 0; i != CLIENT_NAME_SIZE; i++)
			result += name[i];

		return result;
	}
};

