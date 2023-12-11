

#pragma once


#include <iostream>

// テンプレートクラスの宣言
template <typename T>
class MyTemplate {
public:
	void printValue(T value);
	//		{
	//			std::cout << "Value: " << value << std::endl;
	//		}
};


template<class Ch>
class MyTemplateA
	: MyTemplate<Ch>
{
public:
	void printValueA(Ch value);
};



