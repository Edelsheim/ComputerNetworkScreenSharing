#pragma once

#include <string>
class PointData
{
public:
	char type;
	LONG x;
	LONG y;

	PointData() {
		type = 'c';
		x = -1;
		y = -1;
	};
	~PointData() {};

	std::wstring ToWString()
	{
		std::wstring result = L"";
		CStringW str(type);
		result.append(str);
		result.append(L"x");

		LONG x_ = x;
		LONG y_ = y;
		
		int num = (int)(x_ / 1000);
		x_ = x_ % 1000;
		result.append(std::to_wstring(num));
		num = (int)(x_ / 100);
		x_ = x_ % 100;
		result.append(std::to_wstring(num));
		num = (int)(x_ / 10);
		result.append(std::to_wstring(num));
		num = (int)(x_ % 10);
		result.append(std::to_wstring(num));

		result.append(L"y");
		num = (int)(y_ / 1000);
		y_ = y_ % 1000;
		result.append(std::to_wstring(num));
		num = (int)(y_ / 100);
		y_ = y_ % 100;
		result.append(std::to_wstring(num));
		num = (int)(y_ / 10);
		result.append(std::to_wstring(num));
		num = (int)(y_ % 10);
		result.append(std::to_wstring(num));

		return result;
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

		return result;
	}
};

