#include <iostream>
#include"json/json.h"  //#include"../jsoncpp-src-0.5.0/include/json/json.h"
#include<sstream>

static void Seralizer(Json::Value &root, std::string &outString)//序列化
{
	Json::FastWriter w;
 //	Json::StyledWriter w;
	outString = w.write(root);
	std::cout << outString;
}
static void UnSeralizer(std::string &inString, Json::Value &root)//反序列化
{
	std::cout <<"before UnSeralizer: inString" << inString << std::endl;//////////
	Json::Reader r;
	r.parse(inString,root,false);
	std::cout <<"after UnSeralizer: inString" << inString << std::endl;//////////
}

int main()
{
// //	std::cout << __FILE__ << std::endl;
// //	std::cout << __LINE__ << std::endl;
 
 	std::string nick_name = "zhangsan";
 	std::string school = "sust";
 	std::string passwd = "123456";
	unsigned int id = 200;

 	Json::Value root;
 	root["name"] = nick_name;
 	root["school"] = school;
 	root["passwd"] = passwd;
	root["id"] = id;
	std::string outString;
	Seralizer(root,outString);

	Json::Value ro;
	UnSeralizer(outString,ro);
	std::cout << ro["name"].asString() << std::endl;
	std::cout << ro["school"].asString() << std::endl;
	std::cout << ro["passwd"].asString() << std::endl;
	std::cout << ro["id"].asInt() << std::endl;

 
 //	Json::FastWriter w;
 //	Json::StyledWriter w;
 //	std::string s = w.write(root);
 //	std::cout << s << std::endl;

//	std::stringstream ss("1234");
//	int x = 0;
//	ss >> x;
//	std::cout << x << std::endl;
//	std::stringstream ss;
//	int x = 4321;
//	ss << x;
//	std::cout << ss.str() << std::endl;
	return 0;
}
/*
[xxxx]tree lib
.
	include
		json
			autolink.h
			config.h
			features.h
			forwards.h
			json.h
			reader.h
			value.h
			writer.h
	lib
		libjsoncpp.a
		libjsoncpp.so
*/
//在centos7中，gcc与g++貌似有十分明确的界线
