#include <bits/stdc++.h>

template <typename T>
class LinkedList
{
private:
    struct Node
    {
        T value;
        Node* next;
        Node* prev;

        Node() : next(this), prev(this) {}
        Node(const T& value) : value(value), next(this), prev(this) {}
        
        ~Node() {}
    };

    Node* _head;
    size_t _size;

    Node* first_node() const { return _head->next; }
    Node* last_node() const { return _head->prev; }

public:
    LinkedList() : _head(new Node()), _size(0) {}

    ~LinkedList()
    {
        while (!empty())
        {
            pop_back();
        }
        delete _head;
    }

    bool empty() const { return _size == 0; }
    size_t size() const { return _size; }

    const T& front() const { return first_node()->value; }
    T& front() { return first_node()->value; }

    const T& back() const { return last_node()->value; }
    T& back() { return last_node()->value; }

    void push_front(const T& value) { insert_value_before(value, first_node()); }
    void pop_front() { erase_value(first_node()); }
    
    void push_back(const T& value) { insert_value_before(value, _head); }
    void pop_back() { erase_value(last_node()); }

    class Iterator
    {
    public:
        Node* node;

        Iterator(Node* node) : node(node) {}

        const T& operator*() const { return node->value; }
        T& operator*() { return node->value; }

        Iterator& operator++()
        {
            node = node->next;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            node = node->next;
            return temp;
        }

        Iterator& operator+=(int offset)
        {
            for (int i = 0; i < offset; ++i)
            {
                node = node->next;
            }
            return *this;
        }

        Iterator operator+(int offset)
        {
            Iterator temp = *this;
            temp += offset;
            return temp;
        }

        bool operator!=(const Iterator& other) const { return node != other.node; }
        size_t operator-(const Iterator& other) const
        {
            size_t count = 0;
            Node* temp = other.node;
            while (temp != node)
            {
                temp = temp->next;
                ++count;
            }
            return count;
        }
    };

    Iterator begin() { return Iterator(first_node()); }
    Iterator end() { return Iterator(_head); }

    void insert(const Iterator& it_before, const T& value) { insert_value_before(value, it_before.node); }
    void erase(const Iterator& it) { erase_value(it.node); }

private:
    void insert_node_before(Node* node, Node* next_node)
    {
        node->prev = next_node->prev;
        node->next = next_node;
        next_node->prev->next = node;
        next_node->prev = node;
    }

    void insert_value_before(const T& value, Node* before_node)
    {
        Node* new_node = new Node(value);
        insert_node_before(new_node, before_node);
        ++_size;
    }

    void erase_node(Node* node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void erase_value(Node* node)
    {
        erase_node(node);
        delete node;
        --_size;
    }
};

int main()
{
    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    std::cout << list.front() << "\n";
    std::cout << list.back() << "\n";

    for (auto it = list.begin(); it != list.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    list.push_front(0);

    for (auto it = list.begin(); it != list.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    list.pop_front();

    for (auto it = list.begin(); it != list.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    auto it = list.begin();
    ++it;
    list.insert(it, 10);
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    it = list.begin();
    ++it;
    list.erase(it);
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    list.pop_back();
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << std::boolalpha << list.empty() << "\n";

    std::cout << list.size() << "\n";

    return 0;
}
