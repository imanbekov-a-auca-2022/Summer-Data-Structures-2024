#include <bits/stdc++.h>

template <typename T>
class search_tree
{
private:
    struct Node
    {
        T value;
        Node* left;
        Node* right;
        int height;

        Node(const T& value) : value(value), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node) { return node ? node->height : 0; }

    int balance_factor(Node* node) { return node ? height(node->left) - height(node->right) : 0; }

    void update_height(Node* node)
    {
        if (node)
        {
            node->height = std::max(height(node->left), height(node->right)) + 1;
        }
    }

    Node* rotate_right(Node* y)
    {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        update_height(y);
        update_height(x);

        return x;
    }

    Node* rotate_left(Node* x)
    {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        update_height(x);
        update_height(y);

        return y;
    }

    Node* balance(Node* node)
    {
        update_height(node);
        int balance = balance_factor(node);

        if (balance > 1)
        {
            if (balance_factor(node->left) < 0)
            {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
        if (balance < -1)
        {
            if (balance_factor(node->right) > 0)
            {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }
        return node;
    }

    Node* insert(Node* node, const T& value, bool& inserted)
    {
        if (!node)
        {
            inserted = true;
            return new Node(value);
        }

        if (value < node->value)
        {
            node->left = insert(node->left, value, inserted);
        }
        else if (value > node->value)
        {
            node->right = insert(node->right, value, inserted);
        }
        else
        {
            inserted = false;
            return node;
        }

        return balance(node);
    }

    Node* min_value_node(Node* node)
    {
        Node* current = node;
        while (current->left != nullptr)
        {
            current = current->left;
        }
        return current;
    }

    Node* erase(Node* root, const T& value, bool& erased)
    {
        if (!root)
        {
            erased = false;
            return root;
        }

        if (value < root->value)
        {
            root->left = erase(root->left, value, erased);
        }
        else if (value > root->value)
        {
            root->right = erase(root->right, value, erased);
        }
        else
        {
            erased = true;
            if (!root->left || !root->right)
            {
                Node* temp = root->left ? root->left : root->right;
                if (!temp)
                {
                    temp = root;
                    root = nullptr;
                }
                else
                {
                    *root = *temp;
                }
                delete temp;
            }
            else
            {
                Node* temp = min_value_node(root->right);
                root->value = temp->value;
                root->right = erase(root->right, temp->value, erased);
            }
        }

        if (!root)
        {
            return root;
        }

        return balance(root);
    }

    Node* find(Node* node, const T& value)
    {
        if (!node || node->value == value)
        {
            return node;
        }
        if (value < node->value)
        {
            return find(node->left, value);
        }
        return find(node->right, value);
    }

    Node* lower_bound(Node* node, const T& value)
    {
        Node* current = node;
        Node* result = nullptr;
        while (current)
        {
            if (!(current->value < value))
            {
                result = current;
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        return result;
    }

    Node* upper_bound(Node* node, const T& value)
    {
        Node* current = node;
        Node* result = nullptr;
        while (current)
        {
            if (value < current->value)
            {
                result = current;
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        return result;
    }

public:
    search_tree() : root(nullptr) {}

    ~search_tree()
    {
        std::stack<Node*> nodes;
        if (root)
        {
            nodes.push(root);
        }
        while (!nodes.empty())
        {
            Node* node = nodes.top();
            nodes.pop();
            if (node->left)
            {
                nodes.push(node->left);
            }
            if (node->right)
            {
                nodes.push(node->right);
            }
            delete node;
        }
    }

    class Iterator
    {
    public:
        Node* node;
        Node* root;

        Iterator(Node* node, Node* root) : node(node), root(root) {}

        const T& operator*() const { return node->value; }
        T& operator*() { return node->value; }

        Iterator& operator++()
        {
            if (node)
            {
                if (node->right)
                {
                    node = node->right;
                    while (node->left)
                    {
                        node = node->left;
                    }
                }
                else
                {
                    Node* successor = nullptr;
                    Node* current = root;
                    while (current)
                    {
                        if (node->value < current->value)
                        {
                            successor = current;
                            current = current->left;
                        }
                        else if (node->value > current->value)
                        {
                            current = current->right;
                        }
                        else
                        {
                            break;
                        }
                    }
                    node = successor;
                }
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const { return node != other.node; }
    };

    Iterator begin()
    {
        Node* node = root;
        while (node && node->left)
        {
            node = node->left;
        }
        return Iterator(node, root);
    }

    Iterator end() { return Iterator(nullptr, root); }

    Iterator find(const T& value) { return Iterator(find(root, value), root); }

    Iterator lower_bound(const T& value) { return Iterator(lower_bound(root, value), root); }

    Iterator upper_bound(const T& value) { return Iterator(upper_bound(root, value), root); }

    std::pair<Iterator, bool> insert(const T& value)
    {
        bool inserted = false;
        root = insert(root, value, inserted);
        return std::make_pair(find(value), inserted);
    }

    size_t erase(const T& value)
    {
        bool erased = false;
        root = erase(root, value, erased);
        return erased ? 1 : 0;
    }

    Iterator erase(const Iterator& it)
    {
        if (it.node)
        {
            T value = *it;
            ++it;
            erase(value);
        }
        return it;
    }
};

int main()
{
    std::cout << "Hello, world";
}
