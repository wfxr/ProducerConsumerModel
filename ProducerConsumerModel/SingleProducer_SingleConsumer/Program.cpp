/*!
* brief: ��������-��������ģ��ʵ��
*
* date: 2015/10/13
* author: Wenxuan
*/
#include <mutex>
#include <iostream>
#include <queue>
#include <functional>

#include "Repository.h"
#include "Producer.h"
#include "Consumer.h"

std::mutex stream_lock; // ȫ��������֤cout��ͬ��

int main() {
	auto repository = std::make_shared<Repository>(10);

	Producer producer(repository, 1000);
	Consumer consumer(repository, 500);

	auto count = 50;  // ���������ѵĲ�Ʒ����
	std::thread produce_thread(std::bind(&Producer::produce, &producer, count));
	std::thread consumer_thread(std::bind(&Consumer::consume, &consumer, count));

	produce_thread.join();
	consumer_thread.join();

	return 0;
}