#include<iostream>
using namespace std;
#include<pthread.h>
#include<unistd.h>

class A
{
	private:
		int a = 55;
	public:
		static void *func(void *arg)
		{
			pthread_detach(pthread_self());
			int *pa = (int*)arg;
			for(int i = 0; i < 5 ; ++i){
				cout << "*pa = " << *pa << endl;
				sleep(1);
			}
		}
		void ptt()
		{
			pthread_t tid;
			int *x = new int(6);
			pthread_create(&tid,NULL,func,x);

		}
};
