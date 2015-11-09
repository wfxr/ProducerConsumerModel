#pragma once
#include <mutex>
#include <queue>

class Product;

// ��Ʒ�ֿ�
class Repository {
public:
    std::mutex mutex;

    Repository(int storage) : _storage(storage), _producer_count(0) {}
    bool full();
    bool empty();
    bool hasProducer();
    void push(std::shared_ptr<Product> &product);
    std::shared_ptr<Product> fetch();
    void add_producer();
    void remove_producer();

    std::condition_variable cv; // �����������ڿ�淢���仯ʱ������߷���֪ͨ
private:
    std::queue<std::shared_ptr<Product>> _products; // �洢�Ĳ�Ʒ
    size_t _storage;                                // �ֿ�����
    size_t _producer_count; // �Ѷ��ĵ������ߵ�����
};

inline bool Repository::full() { return _products.size() == _storage; }

inline bool Repository::empty() { return _products.empty(); }

inline void Repository::push(std::shared_ptr<Product> &product) {
    if (full()) throw std::overflow_error("repository is already full!");
    _products.push(product);
    cv.notify_all();
}

inline std::shared_ptr<Product> Repository::fetch() {
    if (empty()) throw std::underflow_error("repository is empty!");
    auto ret = _products.front();
    _products.pop();
    cv.notify_all();
    return ret;
}

inline void Repository::add_producer() { ++_producer_count; }

inline void Repository::remove_producer() { --_producer_count; }

inline bool Repository::hasProducer() { return _producer_count != 0; }
