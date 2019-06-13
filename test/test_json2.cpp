/*************************************************************************
    > File Name: test_json2.cpp
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年04月20日 星期六 16时02分22秒
 ************************************************************************/

//jsoncpp主要包括三种类型的类：Value，Reader，Writer。jsoncpp中全部对象，类名都在namespace Json中，包括json.h就可以。

#include<iostream>
#include<string>
#include<vector>
//using namespace std;
#include"json/json.h"

void test_Value(){
//Json::Value 是jsoncpp 中最基本、最重要的类，用于表示各种类型的对象。
//	Value对象有点像javascript里面的数组，可以装任意类型的数据

	Json::Value tmp;

	tmp["name"] = "hzq";
	tmp["school"] = Json::Value("xagydx");

	
	tmp["row"]["col"] = 1;
	tmp["row"]["xcol"]["xx"] = "我可以无限取方括号插入";
	tmp["row"]["list"] = {
		"花括号表示一个对象"
	};
	tmp["oo"]["oo"] = 99999;//插入的整型数字不能太大
	tmp["oo"] = 18;//重新赋值会覆盖前面的

// 数组里面可以append各种不同类型的元素
	tmp["arr"].append("gazx");//0
	tmp["arr"].append("tyzx");//1
	tmp["arr"].append(123);//2
	tmp["arr"].append(12.3);//3

	Json::Value pp;
	pp["name"] = "pp";
	pp["age"] = 11;
	tmp["arr"].append(pp);//4

	int index = 0;
	tmp["arr"][index] = "sssssssssss";//注意，访问数组第0个元素时，不能直接在方括号里写0。必须像这样写一个变量

	tmp["xx"];
	tmp["xx1"].resize(0);
	tmp["xx2"].resize(2);
	index = 0;
	tmp["xx2"][index++] = "hello";
	tmp["xx2"][index++] = "world";
	
	//tmp["xx"].resize(5);
	//tmp["xx"] = {"太阳","月亮"};//貌似不能用初始化列表的方式来初始化Json数组
	//tmp["object"] = { { "currency", "USD"}, {"value", 42.99} };
/////////////////////////////////////

	std::cout << tmp["name"].asString() << std::endl;
//	Json::ValueType type = tmp.type();
//	std::cout << type << std::endl;
	std::cout << tmp << std::endl;
}

std::string test_Writer(){
//如上说了 Json::Value 的使用方式，如今到了该查看刚才赋值内容的时候了，查看 json 内容，使用Writer 类就可以。
//Jsoncpp 的 Json::Writer 类是一个纯虚类，并不能直接使用。
//在此我们使用 Json::Writer 的子类：Json::FastWriter、Json::StyledWriter、Json::StyledStreamWriter。
	Json::Value tmp;
	tmp["name"] = "hzq";
	tmp["ame"]["nick"] = "mmll";
	tmp["age"] = 23;

	Json::FastWriter fw;
	std::string ff = fw.write(tmp);
	std::cout << "string ff = " << ff << std::endl;
	return ff;

	Json::Value root = ff;
	std::cout << "Json::Value root=fw.write(tmp) > " << root << std::endl;

	Json::StyledWriter sw;
	ff = sw.write(tmp);
	std::cout << "string ff = " << ff << std::endl;
	//return ff;
	
	root = ff;
	std::cout << "Json::Value root=sw.write(tmp) > "<< root << std::endl;

	std::cout << tmp << std::endl;

	//return ff;
}

void test_Reader(){
	std::string str = test_Writer();
	Json::Value root;

	Json::Reader r;
	std::cout << "parse的返回值 " << r.parse(str,root,false) << std::endl;

	std::cout << root << std::endl;
}
int main()
{
//	test_Value();

//	test_Writer();
	
	test_Reader();

	return 0;
}
