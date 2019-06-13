#pragma once

#include<iostream>
#include<pthread.h>
#include"ProtocolUtil.hpp"
#include"UserManager.hpp"
#include"Log.hpp"
#include"DataPool.hpp"
#include"Message.hpp"

class ChatServer;
class Param{
	public:
		ChatServer *sp;//指向服务器对象的指针
		int sock;//服务器的小二套接字描述符
		std::string ip;//客户端的IP
		int port;//客户端的PORT

		Param(ChatServer *sp_,int &sock_,const std::string &ip_,const int &port_)
		:sp(sp_),sock(sock_),ip(ip_),port(port_){}
		~Param(){}
};


class ChatServer{
	private:
		int tcp_listen_sock;
		int tcp_port;
		int udp_work_sock;
		int udp_port;
		UserManager um;

		DataPool pool;

	public:
		ChatServer(int tcp_port_ = 8080,int udp_port_ = 8888)
			:tcp_port(tcp_port_)
			 ,tcp_listen_sock(-1)
			 ,udp_port(udp_port_)
			 ,udp_work_sock(-1)
		{}

		void InitServer()
		{
//			LOG("InitServer",WARNING);/////////////////////////////////////
			tcp_listen_sock = SocketApi::Socket(SOCK_STREAM);
			udp_work_sock = SocketApi::Socket(SOCK_DGRAM);//--------------------------------------------------
			SocketApi::Bind(tcp_listen_sock, tcp_port);
			SocketApi::Bind(udp_work_sock,udp_port);

			SocketApi::Listen(tcp_listen_sock);
		}

		unsigned int RegisterUser(const std::string &name,const std::string &school,const std::string &passwd)
		{
//			LOG("RegisterUser",WARNING);/////////////////////////////////////
			return um.Insert(name,school,passwd);
		}
		unsigned int LoginUser(const unsigned int &id,const std::string &passwd,const std::string &ip,int port)
		{
//			LOG("LoginUser",WARNING);/////////////////////////////////////
			return um.Check(id,passwd);
		}

		//UDP
		void Product()
		{
			std::string message;
			struct sockaddr_in peer;
			Util::RecvMessage(udp_work_sock,message,peer);

			std::cout << "Debug: recv message " << message << std::endl;
			if(!message.empty()){

				Message m;
				m.ToRecvValue(message);
////////////////////////////////////////////////////////////////////////
				if(m.Type() == LOGIN_TYPE){
					std::cout << "i am server, i can arrive here" << std::endl;
					um.AddOnlineUser(m.Id(),peer);//只要有人登陆，把他添加进在线列表
					std::string name_,shool_;
					um.GetUserInfo(m.Id(),name_,shool_);
					//id,shool,name,text,type
					Message new_msg(name_,shool_,m.Text(),m.Id(),m.Type());

					new_msg.ToSendString(message);
				}
////////////////////////////////////////////////////////////////////////

				pool.PutMessage(message);
			}

		}
		void Consume()
		{
			std::string message;
			pool.GetMessage(message);
			std::cout << "Debug: send message " << message << std::endl;
			auto online = um.OnlineUser();
			for(auto it = online .begin();it != online.end();++it){
				Util::SendMessage(udp_work_sock,message,it->second);
			}
		}

		static void *HandlerRequest(void *arg)
		{
		//	pthread_detach(pthread_self());//////////////////////////////
		//	LOG("HandlerRequest---------------------------------------------------------",WARNING);/////////////////////////////////////

			Param *p = (Param *)arg;
			int sock = p->sock;//服务器端的小二套接字描述符
			ChatServer *sp = p->sp;//指向服务器对象的指针
			std::string ip = p->ip;//客户端的IP
			int port = p->port;//客户端的PORT
			delete p;
		//	std::cout << "in HandlerRequest-------------" << ip << ":" << port << std::endl;///////////////////
		//	std::cout << "服务器的小二套接字描述符：" << sock << std::endl;///////////////
			pthread_detach(pthread_self());

			Request rq;
			Util::RecvRequest(sock,rq);//接收request请求的时候出问题了
			Json::Value root;

			//LOG(rq.text,NORMAL);

			Util::UnSeralizer(rq.text,root);
			std::cout << rq.text;///////把请求报文的text字段打印一下
			if(rq.method == "REGISTER"){
//				LOG("REGISTER",NORMAL);/////////////////////
				std::string name = root["name"].asString();//asInt
				std::string school = root["school"].asString();
				std::string passwd = root["passwd"].asString();
				unsigned int id = sp->RegisterUser(name,school,passwd);//--------------------------------------
				send(sock,(void*)&id,sizeof(id),0);
			}
			else if(rq.method == "LOGIN"){
//				LOG("LOGIN",NORMAL);/////////////////////////
				unsigned int id = root["id"].asInt();
//				std::cout << "Login id:" << id << std::endl;/////////////////////////////
				std::string passwd = root["passwd"].asString();
//				std::cout << "Login passwd:" <<passwd << std::endl;////////////////////

				//check , move user to online
				unsigned int result = sp->LoginUser(id,passwd,ip,port);//-------------------------------
				send(sock,&result,sizeof(result),0);
			}
			else{//Logout, TODO
//				std::cout << "sssssssssssssssssssssssssssssssssssssssssssssssssssss" << std::endl;//////////
			}
			//recv by sock
			//分析数据  然后处理
			//response

			close(sock);
		}
		void Start()
		{
			std::string ip;
			int port;
			for(;;){
				int sock = SocketApi::Accept(tcp_listen_sock,ip,port);//以阻塞的方式，一直在这里接收客户端的连接
				if(sock > 0){
					std::cout << "get a new client" << ip << ":" << port << std::endl;
//					std::cout << "服务器的小二套接字描述符：" << sock << std::endl;///////////////

					Param *p = new Param(this,sock,ip,port);
					//一旦有客户端来连接，服务器就创建一个线程
					pthread_t tid;
					pthread_create(&tid,NULL,HandlerRequest,p);
				}
			}
		}

		~ChatServer()
		{}
};
