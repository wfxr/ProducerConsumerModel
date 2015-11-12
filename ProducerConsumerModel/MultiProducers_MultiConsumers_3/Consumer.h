#pragma once

#include <memory>
#include <mutex>
#include <iostream>
#include <string>
#include <thread>

#include "Repository.h"
#include "Product.h"

extern std::mutex stream_lock;

// ������
class Consumer {
public:
    Consumer(std::shared_ptr<Repository>, std::string name);

    void set_unit_cost(size_t);

    void wait_product_arrival(std::unique_lock<std::mutex> &);

    void consume();

private:
    void consume_product(std::shared_ptr<Product> product);

    std::shared_ptr<Repository> _repository;
    size_t _unit_cost = 1;
    std::string _name;
};

inline Consumer::Consumer(std::shared_ptr<Repository> repository,
                          std::string name)
    : _repository(repository), _name(name) {}

inline void Consumer::set_unit_cost(size_t unit_cost) {
    _unit_cost = unit_cost;
}

void Consumer::wait_product_arrival(std::unique_lock<std::mutex> &lock) {
    stream_lock.lock();
    std::cout << "\t" << _name << " is waiting for products..." << std::endl;
    stream_lock.unlock();
    _repository->cv.wait(lock, [this] { return !_repository->empty(); });
}

void Consumer::consume_product(std::shared_ptr<Product> product) {
    stream_lock.lock();
    std::cout << "\t" << _name << ": [" << product->get_id() << "] received."
              << std::endl;
    stream_lock.unlock();

    // ģ�����Ѻ�ʱ
    std::this_thread::sleep_for(std::chrono::milliseconds(_unit_cost));

    stream_lock.lock();
    std::cout << "\t" << _name << ": [" << product->get_id() << "] consumed."
              << std::endl;
    stream_lock.unlock();
}

void Consumer::consume() {
    for (;;) {
        // �����ֿ���
        std::unique_lock<std::mutex> repositoryLock(_repository->mutex);

        // ����ֿ��޻���ȴ�����
        if (_repository->empty()) wait_product_arrival(repositoryLock);

        // ��ʼ���ѣ�ȡ�ò�Ʒ���ͷŲֿ���
        auto product = _repository->fetch();
        repositoryLock.unlock();
        consume_product(product);
    }
}
