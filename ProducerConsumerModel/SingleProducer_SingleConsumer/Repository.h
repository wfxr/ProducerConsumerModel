#pragma once
#include <mutex>
#include <queue>

class Product;

// ��Ʒ�ֿ�
class Repository {
public:
	std::mutex mutex;

	Repository(int storage) :_storage(storage) {}
	std::condition_variable cv; // �����������ڿ�淢���仯ʱ�������ߺ������߷���֪ͨ
	bool full() {
		return _products.size() == _storage;
	}
	bool empty() {
		return _products.empty();
	}
	void push(std::shared_ptr<Product>& product) {
		_products.push(product);
		cv.notify_all();
	}
	auto fetch() {
		auto ret = _products.front();
		_products.pop();
		cv.notify_all();
		return ret;
	}
private:
	std::queue<std::shared_ptr<Product>> _products;
	size_t _storage; // �ֿ�����
};