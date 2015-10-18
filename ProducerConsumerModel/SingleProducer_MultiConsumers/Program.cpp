/*!
* brief: ��������-��������ģ��ʵ��
*
* date: 2015/10/18
* author: Wenxuan
*/
#include <mutex>
#include <vector>

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
    producer.set_unit_cost(1000);
    consumer1.set_unit_cost(3000);
    consumer2.set_unit_cost(5000);

    // ������ִ�������������߳�
    std::vector<std::thread> threads;
    threads.emplace_back([&producer] { producer.produce(20); });
    threads.emplace_back([&consumer1] { consumer1.consume(6); });
    threads.emplace_back([&consumer2] { consumer2.consume(14); });

    // �ȴ��̺߳ϲ�
    for (auto& th : threads) th.join();

    return 0;
}