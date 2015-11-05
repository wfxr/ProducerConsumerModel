#pragma once

#include <memory>
#include <mutex>
#include <iostream>
#include <thread>

#include "Repository.h"
#include "Product.h"

extern std::mutex stream_lock;

// ������
class Producer {
public:
	Producer(std::shared_ptr<Repository>);

	void set_unit_cost(size_t);

	void wait_repository_vacancy(std::unique_lock<std::mutex>&);

	void produce();

	void produce(int count);
private:
	std::shared_ptr<Repository> _repository;
	size_t _unit_cost = 1;
};

inline Producer::Producer(std::shared_ptr<Repository> repository)
	: _repository(repository) {
}

inline void Producer::set_unit_cost(size_t unit_cost) {
	_unit_cost = unit_cost;
}

inline void Producer::wait_repository_vacancy(std::unique_lock<std::mutex>& lock) {
	_repository->cv.wait(lock, [this] {return !_repository->full(); });
}

void Producer::produce() {
	std::unique_lock<std::mutex> repositoryLock(_repository->mutex);

	// ����ֿ�������ȴ���λ
	if (_repository->full()) {
		stream_lock.lock();
		std::cout << "Producer is waiting for an empty slot..." << std::endl;
		stream_lock.unlock();
		wait_repository_vacancy(repositoryLock);
	}

	// ��ʼ�����������׶β���ռ��repository��
	repositoryLock.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(_unit_cost));
	auto newProduct = std::make_shared<Product>();

	// ����Ʒ����ֿ�
	repositoryLock.lock();
	_repository->push(newProduct);
}

void Producer::produce(int count) {
	for (auto i = 0; i < count; ++i) {
		produce();
		stream_lock.lock();
		std::cout << "Product [" << i << "] produced" << std::endl;
		stream_lock.unlock();
	}
}
