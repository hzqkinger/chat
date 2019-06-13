#pragma once

#include<iostream>
#include<string>
#include<unordered_map>
#include<pthread.h>
//#include"Log.hpp"////////////////////////////////////////

class User{
	private:
		std::string nick_name;
		std::string school;
		std::string passwd;
	public:
		User(){}
		User(const std::string &n_, const std::string &s_, const std::string &pwd_)
			:nick_name(n_)
			 ,school(s_)
			 ,passwd(pwd_)
		{}
		bool IsPasswdOk(const std::string &passwd_)
		{
//			std::cout << passwd << " <---> " << passwd_ << std::endl;
			if(passwd == passwd_)
				return true;
			else
				return false;
		}
		std::string &GetNickName()
		{
			return nick_name;
		}
		std::string &GetShool()
		{
			return school;
		}
		~User()
		{}
};

class UserManager{
	private:
		unsigned int assign_id;
		std::unordered_map<unsigned int,User> users;
		std::unordered_map<unsigned int,struct sockaddr_in>online_users;

		pthread_mutex_t lock;//因为map是临界资源，所以需要加锁
		void Lock()
		{
			pthread_mutex_lock(&lock);
		}
		void Unlock()
		{
			pthread_mutex_unlock(&lock);//---------------------------
		}
	public:
		UserManager():assign_id(10000)
		{
			pthread_mutex_init(&lock,NULL);
		}

		unsigned int Insert(const std::string &n_,const std::string &s_,const std::string &p_)
		{
			Lock();

//			LOG("Insert",WARNING);/////////////////////////////////////
			unsigned int id = assign_id++;
			User u(n_,s_,p_);
			if(users.find(id) == users.end())
			{
				//users.insert(make_pair(id,u));
				users.insert({id,u});
//				std::cout << "this id:"<< id <<"can be inserted" << std::endl;////////////////////////
				Unlock();
				return id;
			}

			Unlock();
			return 1;

		}
		unsigned int Check(const unsigned int &id,const std::string &pass)
		{
			Lock();

//			LOG("Check",WARNING);/////////////////////////////////////
			auto it = users.find(id);
//			if(it == users.end())///////////////////////
//				std::cout << "i cant find this id:" << id <<std::endl;////////////////
			if(it != users.end())
			{
				User &u = it->second;//-------------------------
				if(u.IsPasswdOk(pass))
				{
					Unlock();
					return id;
				}
			}

			Unlock();
			return 22;
		}

		void AddOnlineUser(unsigned int id,struct sockaddr_in &peer)
		{
			Lock();

//			LOG("AddOnlineUser",WARNING);/////////////////////////////////////
			auto it =  online_users.find(id);//-------------------
			if(it == online_users.end()){
				online_users.insert({id,peer});
			}
			Unlock();
		}

		void GetUserInfo(const unsigned int &id,std::string &name_,std::string &shool_)
		{
			Lock();
			name_ = users[id].GetNickName();
			shool_ = users[id].GetShool();
			Unlock();
		}
		std::unordered_map<unsigned int, struct sockaddr_in> OnlineUser()
		{
			Lock();
//			LOG("OnlineUser",WARNING);/////////////////////////////////////
			auto online = online_users;
			Unlock();
			return online;
		}

		~UserManager()
		{
			pthread_mutex_destroy(&lock);
		}
};
