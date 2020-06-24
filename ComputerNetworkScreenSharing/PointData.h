#pragma once

#include <string>
class PointData
{
public:
	char type;
	char id[CLIENT_NAME_SIZE];
	LONG x;
	LONG y;

	PointData() {
		type = 'c';
		memset(id, 0, CLIENT_NAME_SIZE);
		x = -1;
		y = -1;
	};
	~PointData() {};

	void GetData(char* data)
	{
		int x_ = x;
		int y_ = y;

		data[0] = type;
		data[1] = 'x';
		data[2] = (int)(x_ / 1000) + '0';
		x_ = x_ % 1000;
		data[3] = (int)(x_ / 100) + '0';
		x_ = x_ % 100;
		data[4] = (int)(x_ / 10) + '0';
		data[5] = (int)(x_ % 10) + '0';

		data[6] = 'y';
		data[7] = (int)(y_ / 1000) + '0';
		y_ = y_ % 1000;
		data[8] = (int)(y_ / 100) + '0';
		y_ = y_ % 100;;
		data[9] = (int)(y_ / 10) + '0';
		data[10] = (int)(y_ % 10) + '0';

		for (int i = 0; i != CLIENT_NAME_SIZE; i++)
			data[11 + i] = id[i];
	}

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
			result += id[i];

		return result;
	}
};

