#pragma once

#include<iostream>
#include<string>
#include"ProtocolUtil.hpp"
#include"json/json.h"

#define NORMAL_TYPE 0
#define LOGIN_TYPE 1

class Message{
private:
	std::string nick_name;//发信息人的名字
	std::string school;//学校
	std::string text;//消息的内容
	unsigned int id;//发信息人的qq号

	unsigned int type;//分辨消息的种类
public:
	Message(){}
	Message(const std::string &n_,const std::string &s_,const std::string &t_,const unsigned int &id_,const unsigned int &type_ = NORMAL_TYPE)
	:nick_name(n_),school(s_),text(t_),id(id_),type(type_)
	{}

	void ToSendString(std::string &sendString)
	{
		Json::Value root;
		root["name"] = nick_name;
		root["school"] = school;
		root["text"] = text;
		root["id"] = id;
		root["type"] = type;
		
		Util::Seralizer(root,sendString);//把要发送的消息序列化
	}
	void ToRecvValue(std::string &recvString)
	{
		Json::Value root;
		Util::UnSeralizer(recvString,root);//把接收到的消息反序列化

		nick_name = root["name"].asString();
		school = root["school"].asString();
		text = root["text"].asString();
		id = root["id"].asInt();
		type = root["type"].asInt();
	}
	const std::string &NickName()
	{
		return nick_name;
	}
	const std::string &School()
	{
		return school;
	}
	const std::string &Text()
	{
		return text;
	}
	const unsigned int &Id()
	{
		return id;
	}
	const unsigned int &Type()
	{
		return type;
	}


	~Message()
	{}
};
