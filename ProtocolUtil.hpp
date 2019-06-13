#pragma  once

#include<iostream>
#include<sstream>
#include<vector>
#include<string>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include"json/json.h"
#include"Log.hpp"

#define BACKLOG 5
#define MESSAGE_SIZE 1024
class Request{
	public:
		std::string method;//REGISTER ,LOGIN , LOGOUT
		std::string content_length;//Content_Length:89
		std::string blank;
		std::string text;//
	public:
		Request():blank("\n"){}
		~Request(){}

};

class Util{
	public:
		static bool RegisterEnter(std::string &n_ ,std::string &s_ ,std::string &passwd)
		{
//			LOG("RegisterEnter",WARNING);/////////////////////////////////////

			std::cout << "Please Enter Nick Name:";//----------------------------------------
			std::cin >>n_;
			std::cout << "Please Enter School:";
			std::cin >> s_;
			std::cout << "Please Enter Passwd:";
			std::cin >>	passwd;
			std::string again;
			std::cout << "Please Enter Passwd Again:";
			std::cin >>again;
			if(passwd == again){
				return true;
			}
			return false;
		}
		static bool LoginEnter(unsigned int &id, std::string &passwd)
		{
//			LOG("LoginEnter",WARNING);/////////////////////////////////////

			std::cout << "Please Enter Your ID:";
			std::cin >> id;
			std::cout << "Please Enter Your Passwd:";
			std::cin >> passwd;
	
			return true;
		}
		static void Seralizer(Json::Value &root, std::string &outString)//序列化
		{
			Json::FastWriter w;
			outString = w.write(root);
//			std::cout << outString << std::endl;//////////////////////
		}
		static void UnSeralizer(std::string &inString, Json::Value &root)//反序列化
		{
//			std::cout <<"before UnSeralizer: inString" << inString << std::endl;//////////
			Json::Reader r;
			r.parse(inString,root,false);
//			std::cout <<"after UnSeralizer: inString" << inString << std::endl;//////////
		}
	
		static std::string IntToString(int x)
		{
			std::stringstream ss;
			ss << x;
			return ss.str();
		}
		static int StringToInt(std::string &str)
		{
			int x;
			std::stringstream ss(str);
			ss >> x;
			return x;
		}
		static void RecvOneLine(int sock,std::string &outString)
		{
			char c = 'X';
			while(c != '\n'){
				ssize_t s = recv(sock,&c,1,0);
				if(s > 0){
					if(c == '\n')break;
					outString.push_back(c);
				}
				else {
					break;
				}
			}
		}
		//TCP
		static void RecvRequest(int sock,Request &rq)
		{
			RecvOneLine(sock,rq.method);
			RecvOneLine(sock,rq.content_length);
			RecvOneLine(sock,rq.blank);
	
			std::string &cl = rq.content_length;//content_length:88
			std::size_t pos  = cl.find(":");//所有的问题都出在这里，要查找的是":",而不是": "
			if(pos == std::string::npos)
				return;
			std::string sub = cl.substr(pos + 1);
			int size = StringToInt(sub);
			char c;
			for(auto i = 0; i < size; ++i){
				recv(sock,&c,1,0);
				(rq.text).push_back(c);
			}
		}
		static void SendRequest(int sock,Request &rq)
		{
			std::string &m_ = rq.method;
			std::string &cl_ = rq.content_length;
			std::string &b_ = rq.blank;
			std::string &text_ = rq.text;

			send(sock,m_.c_str(),m_.size(),0);
			send(sock,cl_.c_str(),cl_.size(),0);
			send(sock,b_.c_str(),b_.size(),0);
			send(sock,text_ .c_str(),text_.size(),0);
		}

		//UDP
		static void RecvMessage(int sock,std::string &message,struct sockaddr_in &peer)
		{
			char msg[MESSAGE_SIZE];
			socklen_t len = sizeof(peer);
			ssize_t s = recvfrom(sock,msg,sizeof(msg) - 1,0,(struct sockaddr*)&peer,&len);
			if(s <= 0){
				LOG("recvform message error",WARNING);
			}
			else{
				message = msg;
			}
		}
		static void SendMessage(int sock,const std::string &message,struct sockaddr_in &peer)
		{
			sendto(sock,message.c_str(),message.size(),0,(struct sockaddr*)&peer,sizeof(peer));
		}

		static void addUser(std::vector<std::string> &online,std::string &f)
		{
			for(auto it = online.begin();it != online.end();++it){
				if(*it == f)
					return;
			}
			online.push_back(f);
		}

};


class SocketApi{
	public:
		static int Socket(int type)
		{
			int sock = socket(AF_INET,type,0);
			if(sock < 0){
				LOG("socket error",ERROR);
				exit(2);
			}
		}

		static void Bind(int sock,int port)
		{
			struct sockaddr_in local;
			local.sin_family = AF_INET;
			local.sin_addr.s_addr = htonl(INADDR_ANY);
			local.sin_port = htons(port);

			if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0){
				LOG("bind error",ERROR);
				exit(3);
			}
		}

		static void Listen(int sock)
		{
			if(listen(sock,BACKLOG) < 0){
				LOG("listen error",ERROR);
				exit(4);
			}
		}

		static int Accept(int listen_sock,std::string &out_ip,int &out_port)
		{
			struct sockaddr_in peer;
			socklen_t len = sizeof(peer);//该参数是为了获得客户端的套接字

			int sock = accept(listen_sock,(struct sockaddr*)&peer,&len);
			if(sock < 0){
				LOG("accept error",WARNING);
				return -1;
			}
			
			out_ip = inet_ntoa(peer.sin_addr);
			out_port = htons(peer.sin_port);

			return sock;
		}

		static bool Connect(int sock, std::string peer_ip,const int port)
		{
			struct sockaddr_in peer;
			peer.sin_family = AF_INET;
			peer.sin_addr.s_addr = inet_addr(peer_ip.c_str());
			peer.sin_port = htons(port);

			if(connect(sock,(sockaddr*)&peer,sizeof(peer)) < 0){
				LOG("connect error",WARNING);
				return false;
			}
			return true;
		}

};
