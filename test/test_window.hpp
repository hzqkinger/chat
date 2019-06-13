#pragma once

#include<iostream>
#include<string>
#include<cstring>      //#incllude<string.h>
#include<ncurses.h>//
#include<unistd.h>
#include<pthread.h>


class Window{
//	private:
	public:
		WINDOW *header;
		WINDOW *output;
		WINDOW *online;
		WINDOW *input;

		pthread_mutex_t lock;
	public:
		Window()
		{
			initscr();//使用这个ncurses这个库时，必须先调用这个函数初始化
			//初始化的时候会得到LINE,COLS这两个宏的值
			curs_set(0);//隐藏光标
			pthread_mutex_init(&lock,NULL);
		}

		void SafeWrefresh(WINDOW *w)
		{
			pthread_mutex_lock(&lock);
			wrefresh(w);
			pthread_mutex_unlock(&lock);
		}

		void DrawHeader()//1
		{//LINES , COLS
			int h = LINES * 0.2;
			int w = COLS;
			int y = 0;
			int x = 0;

			header = newwin(h,w,y,x);
			//box(w,'-','%');
			box(header,0,0);
			SafeWrefresh(header);
		}
		void DrawOutput()//2
		{
			int h = LINES * 0.6;
			int w = COLS * 0.75;
			int y = LINES * 0.2;
			int x = 0;

			output = newwin(h,w,y,x);
			//box(w,'-','%');
			box(output,0,0);
			SafeWrefresh(output);
		}
		void DrawOnline()//3
		{
			int h = LINES * 0.6;
			int w = COLS * 0.25;
			int y = LINES * 0.2;
			int x = COLS * 0.75;

			online = newwin(h,w,y,x);
			//box(w,'-','%');
			box(online,0,0);
			SafeWrefresh(online);
		}
		void DrawInput()//4
		{
			int h = LINES * 0.2;
			int w = COLS;
			int y = LINES * 0.8;
			int x = 0;

			input = newwin(h,w,y,x);
			box(input,0,0);

			const std::string tips = "Please Enter# ";
			PutStringToWin(input,2,2,tips);
			SafeWrefresh(input);
		}

		void GetStringFromInput(std::string &message)//我们只会从input中提取字符串
		{
			char buffer[1024];
			memset(buffer,0,sizeof(buffer));
			wgetnstr(input,buffer,sizeof(buffer));
			message = buffer;

			delwin(input);
			DrawInput();
		}
		void PutStringToWin(WINDOW *w,int y,int x,const std::string &message)
		{
			mvwaddstr(w,y,x,message.c_str());
			SafeWrefresh(w);
		}

		void PutMessageToOutput(const std::string &message)
		{
			static int line = 1;
			int y,x;
			getmaxyx(output,y,x);
			if(line > y - 2){
				delwin(output);
				DrawOutput();
				line = 1;
			}
			PutStringToWin(output,line++,2,message);
		}

		void Welcome()
		{
			const std::string welcome = "welcome to my chat system!";
			int num = 1;
			int y,x;
			int dir = 0; //dir为0表示从左往右；为1表示从右往左
			for(;;){
				DrawHeader();
				getmaxyx(header,y,x);
				PutStringToWin(header,y/2,num,welcome);
				if(num > x - welcome.size() - 3)dir = 1;
				if(num <= 1)dir = 0;
				if(dir == 0)num++;
				else num--;
				usleep(100000);
				delwin(header);
			}
		}

		~Window()
		{
			endwin();//同样，必须使用这个函数释放资源
			pthread_mutex_destroy(&lock);
		}
};
