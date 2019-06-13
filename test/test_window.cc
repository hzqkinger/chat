//g++ main.cc -lncurses -lpthread
#include"Window.hpp"
#include<unistd.h>
#include<string>
int main()
{
	Window w;
	w.DrawHeader();
	w.DrawOutput();
	w.DrawOnline();
	w.DrawInput();
	sleep(2);

//	std::string ss = "hhhhhh";
//	w.PutStringToWin(w.header,2,3,ss);
//	sleep(5);

	//w.Welcome();
	
	std::string message;
	for(;;){
		w.GetStringFromInput(message);
		w.PutMessageToOutput(message);
	}
	return 0;
}
