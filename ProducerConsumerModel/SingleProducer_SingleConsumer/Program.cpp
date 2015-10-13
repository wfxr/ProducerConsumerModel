/*!
* brief: ��������-��������ģ��ʵ��
*
* date: 2015/10/13
* author: Wenxuan
*/
#include <mutex>

#include "Repository.h"
#include "Producer.h"
#include "Consumer.h"

std::mutex stream_lock; // ȫ��������֤cout��ͬ��

int main() {
	// �����ֿ�
	auto repository = std::make_shared<Repository>(10);

	// ���������ߺ�������
	Producer producer(repository);
	Consumer consumer(repository);

	// �ֱ��趨��λ�����ĺ�ʱ
	producer.set_unit_cost(20);
	consumer.set_unit_cost(30);

	auto count = 50;  // ׼�����������ѵĲ�Ʒ����
	std::thread producer_thread([&producer, count] {producer.produce(count); });
	std::thread consumer_thread([&consumer, count] {consumer.consume(count); });

	// �ȴ��̺߳ϲ�
	producer_thread.join();
	consumer_thread.join();

	return 0;
}