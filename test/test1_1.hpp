#pragma once
#include<iostream>
using namespace std;//.hpp文件可以使用命名空间
#include"test1.hpp"

//.hpp文件类可以循环引用
class B;
class A{
	public:
		void funcA(const B& b)
		{
			cout << "A::funcA" << endl;
			//a.funcA(a,b);
		}
};
//class B{
//	public:
//		void funcB(const A& a)
//		{
//			cout << "B::funcB" << endl;
//			//a.funcA(a,b);
//		}
//};
