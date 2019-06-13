#pragma once
#include<iostream>
#include<string>
#include<pthread.h>
#include"ProtocolUtil.hpp"
#include"Message.hpp"
//#include"Log.hpp"///////////////////////////
#include"Window.hpp"

#define TCP_PORT 8080
#define UDP_PORT 8888

class ChatClient;
struct ParamPair{
	Window *wp;
	ChatClient *cp;
};

class ChatClient{
	private:
		std::string peer_ip;//记录服务器的IP地址
		int tcp_sock;//记录客户端的tcp套接字描述符
		int udp_sock;//记录客户端的udp套接字描述符

		std::string passwd;

		struct sockaddr_in server;//记录服务器的UDP套接字
	public:
		std::string nick_name;
		std::string school;
		unsigned int id;
	public:
		//注册，登录，收消息，发消息
		ChatClient(std::string ip_):peer_ip(ip_)
		{
			id = 0;
			tcp_sock = -1;
			udp_sock = -1;

			server.sin_family = AF_INET;
			server.sin_port = htons(UDP_PORT);
			server.sin_addr.s_addr = inet_addr(peer_ip.c_str());
		}
		void InitClient()
		{
			udp_sock = SocketApi::Socket(SOCK_DGRAM);
		}
		bool ConnectServer()
		{
			tcp_sock = SocketApi::Socket(SOCK_STREAM);
			return SocketApi::Connect(tcp_sock,peer_ip,TCP_PORT);
		}
		bool Register()
		{
			if(Util::RegisterEnter(nick_name,school,passwd) && ConnectServer()){
				Request rq;
				rq.method = "REGISTER\n";

				Json::Value root;
				root["name"] = nick_name;
				root["school"] = school;
				root["passwd"] = passwd;

				Util::Seralizer(root,rq.text);

				rq.content_length = "Content_Length:";
				rq.content_length += Util::IntToString((rq.text).size());
				rq.content_length += "\n";

				bool res = false;
				Util::SendRequest(tcp_sock,rq);
				recv(tcp_sock,&id,sizeof(id),0);//没做错误判断
				
				if(id >= 10000){//我们规定返回的id为大于等于10000
					std::cout << "Register Success! Your Login ID Is :" << id <<std::endl;
					res = true;
				}
				else {
					std::cout << "Register Failed! Code Is :" << id <<std::endl;
				}
				close(tcp_sock);
				return res;
			}
		}
		bool Login()
		{
			if(Util::LoginEnter(id,passwd) && ConnectServer()){
				Request rq;
				rq.method = "LOGIN\n";

				Json::Value root;
				root["id"] = id;
				root["passwd"] = passwd;

				Util::Seralizer(root,rq.text);

				rq.content_length = "Content_Length:";
				rq.content_length += Util::IntToString((rq.text).size());
				rq.content_length += "\n";

				Util::SendRequest(tcp_sock,rq);
				unsigned int result = 0;
				recv(tcp_sock,&result ,sizeof(result),0);//没做错误判断
				
				bool res = false;
				if(result >= 10000){//我们规定返回的id为大于等于10000
					res = true;
//////////////////////////////////////////////////////////////////////////////					
					std::string name_ = "None";
					std::string school_ = "None";
					std::string text_ = "I am login! talk with me...";
					unsigned int type_ = LOGIN_TYPE;
					unsigned int id_ = result;
					Message m(name_,school_,text_,id_,type_);

					std::string sendString;
					m.ToSendString(sendString);
					UdpSend(sendString);
//////////////////////////////////////////////////////////////////////////////					

					std::cout << "Login Success! " << std::endl;
				}
				else {
					std::cout << "Login Failed! Code Is :" << result <<std::endl;
				}
				close(tcp_sock);

				return res;
			}
		}

		void UdpSend(const std::string &msg)//封装的sendto函数
		{
			Util::SendMessage(udp_sock,msg,server);
		}
		void UdpRecv(std::string &msg)//封装的recvfrom函数
		{
			struct sockaddr_in peer;
			Util::RecvMessage(udp_sock,msg,peer);
		}
		static void *Welcome(void *arg)
		{
			pthread_detach(pthread_self());
			Window *wp = (Window*)arg;
			wp->Welcome();
		}

		static void* Input(void *arg)
		{
			pthread_detach(pthread_self());
			struct ParamPair *pptr = (struct ParamPair*)arg;
			Window *wp = pptr->wp;//管理窗口类的指针
			ChatClient *cp = pptr->cp;

			wp->DrawInput();
			std::string text;
			for(;;){
				wp->GetStringFromInput(text);//从Input窗口中得到客户端需要发送的消息
				Message msg(cp->nick_name,cp->school,text,cp->id);
				std::string sendString;
				msg.ToSendString(sendString);//把要发送的消息序列化
				cp->UdpSend(sendString);
			}
		}
		void Chat()
		{
			Window w;
			pthread_t h,l;
			struct ParamPair pp = { &w, this };
			pthread_create(&h,NULL,Welcome,&w);//创建一个线程跑welcome窗口
			pthread_create(&l,NULL,Input,&pp);//创建一个线程跑输入信息的窗口

			w.DrawOutput();//画出显示消息的窗口
			w.DrawOnline();//画出在线用户列表的窗口
			std::string recvString;
			std::string showString;
			std::vector<std::string> online;//存储在线用户的name-school-----------------
			for(;;){//主线程管理输出信息的窗口，在线用户列表的窗口
				Message msg;
				UdpRecv(recvString);//阻塞式接收消息
				msg.ToRecvValue(recvString);

				if(msg.Id() ==  id && msg.Type() == LOGIN_TYPE){
					nick_name = msg.NickName();
					school = msg.School();
				}

				showString = msg.NickName();
				showString += '-';
				showString += msg.School();

				std::string f = showString;//name-shcool
				Util::addUser(online,f);

				showString += '#';
				showString += msg.Text();//name-shcool#text

				w.PutMessageToOutput(showString);
				w.PutUserToOnline(online);
			}
		}

		void Logout()
		{
			//1.构建退出的message报文
			//2.服务器收到该报文，在online用户把发报文的用户删掉；且把报文放到数据池
			//3.客户端收到报文，把Online的窗口刷新一下:w
		}

		~ChatClient(){}
};
