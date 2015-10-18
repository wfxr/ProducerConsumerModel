/*!
* brief: ��������-��������ģ��ʵ��
*
* date: 2015/10/18
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
    Consumer consumer1(repository, "Consumer1");
    Consumer consumer2(repository, "Consumer2");

    // �ֱ��趨��λ�����ĺ�ʱ
    producer.set_unit_cost(20);
    consumer1.set_unit_cost(30);
    consumer2.set_unit_cost(50);

    // ������ִ�������������߳�
    std::thread producer_thread([&producer] { producer.produce(5); });
    std::thread consumer_thread1([&consumer1] { consumer1.consume(2); });
    std::thread consumer_thread2([&consumer2] { consumer2.consume(3); });

    // �ȴ��̺߳ϲ�
    producer_thread.join();
    consumer_thread1.join();
    consumer_thread2.join();

    return 0;
}