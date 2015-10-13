#pragma once

#include <memory>
#include <mutex>
#include <iostream>

#include "Repository.h"
#include "Product.h"

extern std::mutex stream_lock;

// ������
class Producer {
public:
	Producer(std::shared_ptr<Repository> repository, double produce_speed)
		: _repository(repository) {
		set_produce_speed(produce_speed);
	}

	void set_produce_speed(double produce_speed) {
		if (produce_speed <= 0.0)
			throw std::logic_error("produce speed should be greater than 0.0");
		_produce_speed = produce_speed;
	}

	// �ȴ��ֿ��λ
	void waitRepositoryVacancy(std::unique_lock<std::mutex>& lock) {
		_repository->cv.wait(lock, [this] {return !_repository->full(); });
	}

	void produce_one() {
		std::unique_lock<std::mutex> repositoryLock(_repository->mutex);

		// ����ֿ�������ȴ���λ
		while (_repository->full()) {
			stream_lock.lock();
			std::cout << "Producer is waiting for an empty slot..." << std::endl;
			stream_lock.unlock();
			waitRepositoryVacancy(repositoryLock);
		}

		// ��ʼ�����������׶β���ռ��repository
		repositoryLock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(unit_time_cost()));
		auto newProduct = std::make_shared<Product>();

		// ����Ʒ����ֿ�
		repositoryLock.lock();
		_repository->push(newProduct);
	}
	void produce(int count) {
		for (auto i = 0; i < count; ++i) {
			produce_one();
			stream_lock.lock();
			std::cout << "Product No." << i << " produced" << std::endl;
			stream_lock.unlock();
		}
	}
	size_t unit_time_cost() {
		return static_cast<size_t>(1000 / _produce_speed);
	}
private:
	std::shared_ptr<Repository> _repository;
	double _produce_speed;
};