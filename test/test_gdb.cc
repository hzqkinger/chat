//#include <stdio.h>
//#include <pthread.h>
//void* rout1(void* arg)
//{
//	printf("I am %s!\n",(char*)arg);
//	return (void*)0;
//}
//void* rout2(void* arg)
//{
//	printf("I am %s!\n",(char*)arg);
//	return (void*)0;
//}
//void* rout3(void* arg)
//{
//	printf("I am %s!\n",(char*)arg);
//	return (void*)0;
//}
//int main()
//{
//	pthread_t t1,t2,t3;
//	pthread_create(&t1,NULL,rout1,(void*)"thread1");
//	pthread_create(&t2,NULL,rout1,(void*)"thread2");
//	pthread_create(&t3,NULL,rout1,(void*)"thread3");
//	pthread_join(t1,NULL);
//	pthread_join(t2,NULL);
//	pthread_join(t3,NULL);
//	return 0;
//}


#include<iostream>
using namespace std;
#include<pthread.h>
#include<unistd.h>

void *work(void *arg)
{
	pthread_detach(pthread_self());
	int *pa = (int*)arg;
	cout << "sssssssss" << endl;
	cout << *pa << endl;

	return pa;
}
int main()
{
	pthread_t tid1,tid2;
	int index = 1;
	pthread_create(&tid1,NULL,work,&(++index));
	pthread_create(&tid2,NULL,work,&(++index));
	while(1){
		cout << "ss" <<endl;
		sleep(1);
	}
//	pthread_join(tid1,NULL);
//	pthread_join(tid2,NULL);
//	pthread_detach(tid1);
//	pthread_detach(tid2);
//	sleep(1);

	return 0;
}
