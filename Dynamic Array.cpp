#include <bits/stdc++.h>

template <typename T>
class DynamicArray {
private:
    const int lambda = 1;
    T* values;
    size_t capacity;
    size_t size;

    void ensure_capacity() {
        if (size < capacity) return;
        
        size_t new_capacity = capacity * (1 + lambda);
        T* new_values = new T[new_capacity];
        
        std::move(values, values + size, new_values);
        
        delete[] values;
        values = new_values;
        capacity = new_capacity;
    }

public:
    class Iterator {
    private:
        T* pointer;

    public:
        Iterator(T* p) : pointer(p) {}

        const T& operator*() const {
            return *pointer;
        }

        T& operator*() {
            return *pointer;
        }

        Iterator& operator++() {
            ++pointer;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return pointer != other.pointer;
        }

        T* getPointer() const {
            return pointer;
        }
    };

    DynamicArray() : lambda(1), values(new T[1]), capacity(1), size(0) {}

    ~DynamicArray() {
        delete[] values;
    }

    void push_back(const T& value) {
        ensure_capacity();
        values[size++] = value;
    }

    void insert(size_t pos_before, const T& value) {
        ensure_capacity();

        for (size_t i = size; i > pos_before; --i) {
            values[i] = std::move(values[i - 1]);
        }
        
        values[pos_before] = value;
        ++size;
    }
    
    void erase(size_t pos) {
        for (size_t i = pos; i < size - 1; ++i) {
            values[i] = std::move(values[i + 1]);
        }
        
        --size;
    }

    size_t getSize() const {
        return size;
    }

    const T& operator[](size_t index) const {
        return values[index];
    }

    T& operator[](size_t index) {
        return values[index];
    }

    Iterator begin() {
        return Iterator(values);
    }

    Iterator end() {
        return Iterator(values + size);
    }

    Iterator erase(const Iterator& it) {
        T* pos = it.getPointer();
        for (T* i = pos; i < values + size - 1; ++i) {
            *i = std::move(*(i + 1));
        }
        --size;
        return Iterator(pos);
    }
};

template <typename T>
typename DynamicArray<T>::Iterator find(DynamicArray<T>& arr, const T& value) {
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        if (*it == value) {
            return it;
        }
    }
    return arr.end();
}

int main() {
    DynamicArray<int> arr;

    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.push_back(4);

    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    arr.insert(2, 99);
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    arr.erase(1);
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    auto it = find(arr, 99);
    if (it != arr.end()) {
        std::cout << *it << "\n";
    } else {
        std::cout << "-\n";
    }

    it = find(arr, 99);
    if (it != arr.end()) {
        arr.erase(it);
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "-\n";
    }

    return 0;
}
