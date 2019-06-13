#pragma once
#include<iostream>
using namespace std;//.hpp文件可以使用命名空间
#include"test1_1.hpp"
//class A;
class B{
	public:
		void funcB(const A& a)
		{
			cout << "B::funcB" << endl;
			//a.funcA(a,b);
		}

		static int aa;

		void print()
		{
			cout << aa << endl;
		}
};

int B::aa = 2;
