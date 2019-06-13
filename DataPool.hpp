#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<semaphore.h>

class DataPool{
	private:
		std::vector<std::string> pool;
		int cap;
		sem_t data_sem;
		sem_t blank_sem;

		int product_step;
		int consume_step;
	public:
		DataPool(int cap_ = 512):cap(cap_),pool(cap_),product_step(0),consume_step(0)
		{
			sem_init(&data_sem,0,0);
			sem_init(&blank_sem,0,cap);
		}

		//线程安全的循环队列：
		//1.生产者不能超过消费者一圈
		//2.消费者不能超过生产者
		//3.队列为空时，不能消费，只能生产
		//4.队列为满时，不能生产，只能消费
		void PutMessage(const std::string &msg)
		{
			sem_wait(&blank_sem);
			pool[product_step] = msg;
			product_step++;
			product_step %= cap;

			sem_post(&data_sem);
		}
		void GetMessage(std::string &msg)
		{
			sem_wait(&data_sem);
			msg = pool[consume_step];
			consume_step++;
			consume_step %= cap;
			
			sem_post(&blank_sem);
		}

		~DataPool()
		{
			sem_destroy(&data_sem);
			sem_destroy(&blank_sem);
		}
};
