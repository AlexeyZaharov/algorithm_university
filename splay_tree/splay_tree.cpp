#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <cmath>
#include <unordered_map>

template <typename T>
struct node {
    node<T>* parent;
    node<T>* left;
    node<T>* right;
    const long long key;
    T data;

    node(const long long& key_, const T& data_, node* p = nullptr) : parent(p),
                                                                              left(nullptr), right(nullptr), key(key_), data(data_)  {}

    std::string to_str() const {
        std::ostringstream out;

        out << '[' << key << ' ' << data;

        if (parent) {
            out << ' ' << parent->key;
        }

        out << ']';

        return out.str();
    }
};

template <typename T>
class splay_tree {
    node<T>* root;
    size_t size;

    node<T>* min(node<T>* root_) {
        node<T>* cur = root_;

        if (!cur) {
            return nullptr;
        }

        while (cur && cur->left) {
            cur = cur->left;
        }

        splay(cur);

        return cur;
    }

    node<T>* max(node<T>* root_) {
        node<T>* cur = root_;

        if (!cur) {
            return nullptr;
        }

        while (cur && cur->right) {
            cur = cur->right;
        }

        splay(cur);

        return cur;
    }

    void left_rotate(node<T>* node_) {
        node<T>* parent = node_->parent;
        node<T>* right = node_->right;

        if (parent != nullptr) {
            if (parent->left == node_) {
                parent->left = right;
            }
            else {
                parent->right = right;
            }
        }

        node<T>* left_child_of_right = right->left;

        right->left = node_;
        node_->right = left_child_of_right;

        node_->parent = right;
        right->parent = parent;

        if (left_child_of_right != nullptr) {
            left_child_of_right->parent = node_;
        }
    }

    void right_rotate(node<T>* node_) {
        node<T>* parent = node_->parent;
        node<T>* left = node_->left;

        if (parent != nullptr) {
            if (parent->left == node_) {
                parent->left = left;
            }
            else {
                parent->right = left;
            }
        }

        node<T>* right_child_of_left = left->right;

        left->right = node_;
        node_->left = right_child_of_left;

        node_->parent = left;
        left->parent = parent;

        if (right_child_of_left != nullptr) {
            right_child_of_left->parent = node_;
        }
    }

    void zig(node<T>* node) {
        if (node == node->parent->right) {
            left_rotate(node->parent);
        }
        else {
            right_rotate(node->parent);
        }
    }

    void zigzig(node<T>* node) {
        if (node == node->parent->left && node->parent == node->parent->parent->left) {
            right_rotate(node->parent->parent);
            right_rotate(node->parent);
        }
        else {
            left_rotate(node->parent->parent);
            left_rotate(node->parent);
        }
    }

    void zigzag(node<T>* node) {
        if (node == node->parent->left && node->parent == node->parent->parent->right) {
            right_rotate(node->parent);
            left_rotate(node->parent);
        }
        else {
            left_rotate(node->parent);
            right_rotate(node->parent);
        }
    }

    void splay(node<T>* node) {
        while(node->parent) {
            if (node->parent->parent == nullptr) {
                zig(node);
            }
            else if ((node == node->parent->left && node->parent == node->parent->parent->left) ||
                     (node == node->parent->right && node->parent == node->parent->parent->right)) {
                zigzig(node);
            }
            else {
                zigzag(node);
            }
        }

        root = node;
    }

    node<T>* get_node(const long long& key) {
        node<T>* cur = root;

        if(!cur) {
            return nullptr;
        }

        while (cur->right || cur->left) {
            if (cur->key < key) {
                if (cur->right) {
                    cur = cur->right;
                }
                else {
                    break;
                }
            }
            else if (cur->key > key) {
                if (cur->left) {
                    cur = cur->left;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }

        return  cur;
    }

    size_t find_h_and_indexes(const node<T>* root_, size_t cur_h, size_t cur_index,
            std::unordered_map< size_t, const node<T>* >& map) const {

        if (root_) {
            size_t new_h = cur_h + 1;
            size_t left_index = 2*cur_index + 1;
            size_t right_index = 2*cur_index + 2;

            map[cur_index] = root_;

            return std::max(find_h_and_indexes(root_->left, new_h, left_index, map),
                    find_h_and_indexes(root_->right, new_h, right_index, map));
        }
        else {
            return cur_h;
        }
    }

public:
    splay_tree() : root(nullptr), size{0} {};

    bool add(const long long& new_key, const T& new_data) {
        bool success = true;

        if (root) {
            node<T>* cur = root;
            node<T>* parent = cur;

            while (cur->right || cur->left) {
                cur = new_key < cur->key ?
                        cur->left :
                        cur->right;

                if (!cur) {
                    break;
                }

                parent = cur;
            }

            if (new_key < parent->key) {
                ++size;
                parent->left = new node<T>(new_key, new_data, parent);
                splay(parent->left);
            }
            else if (new_key > parent->key) {
                ++size;
                parent->right = new node<T>(new_key, new_data, parent);
                splay(parent->right);
            }
            else {
                success = false;
            }
        }
        else {
            ++size;
            root = new node<T>(new_key, new_data);
        }

        return success;
    }

    bool delete_node(const long long& key) {
        bool success = false;

        node<T>* node_ = get_node(key);

        if (node_->key == key) {
            success = true;

            splay(node_);

            node<T>* left_root = node_->left;
            node<T>* right_root = node_->right;

            if (left_root && right_root) {
                left_root = max(left_root);

                left_root->parent = nullptr;
                left_root->right = right_root;
                right_root->parent = left_root;

                root = left_root;
            }
            else if (left_root){
                left_root->parent = nullptr;
                root = left_root;
            }
            else if (right_root) {
                right_root->parent = nullptr;
                root = right_root;
            }
            else {
                root = nullptr;
            }

            --size;
            delete node_;
        }

        return success;
    }

    node<T>* search(const long long& key) {
        if (!is_empty()) {
            node<T>* node_ = get_node(key);
            splay(node_);

            return  node_->key == key ? node_ : nullptr;
        }
        else {
            return nullptr;
        }
    }

    bool set(const long long& key, const T& new_data) {
        bool success = false;

        node<T>* node = get_node(key);

        if (node->key == key) {
            success = true;
            node->data = new_data;
            splay(node);
        }

        return success;
    }

    const node<T>* min() {
        return min(root);
    }

    const node<T>* max() {
        return max(root);
    }

    bool is_empty() {
        return size == 0;
    }

    std::string print() const {
        std::ostringstream out{};

        size_t max_level = 0;
        size_t cur_index = 0;

        std::unordered_map<size_t, const node<T>*> map;

        max_level = find_h_and_indexes(root, max_level, cur_index, map);

        unsigned long long num_of_print_nodes = static_cast<unsigned long long>(1 << max_level) - 1;

        if (!num_of_print_nodes) {
            out << '_' << '\n';
        }

        std::vector<std::string> v(num_of_print_nodes, "_");

        for (auto& i : map) {
            v[i.first] = i.second->to_str();
        }

        while (cur_index < num_of_print_nodes) {
            ++cur_index;
            if (((cur_index + 1) & cur_index) == 0 ) {
                out << v[cur_index - 1] << '\n';
            }
            else {
                out << v[cur_index - 1] << ' ';
            }
        }

        return out.str();
    }

    ~splay_tree() {
        std::queue<node<T>*> queue;
        queue.push(root);

        while (!queue.empty()) {
            node<T>* cur = queue.front();
            queue.pop();

            if(cur) {
                queue.push(cur->left);
                queue.push(cur->right);
            }

            delete cur;
        }
    }
};

std::string get_data() {
    std::string buf{};

    std::getline(std::cin, buf, '\0');
    return buf;
}

std::string test() {

    std::istringstream stream(get_data());
    std::ostringstream out{};
    std::string buf{};

    splay_tree<std::string> my_tree;

    long long key;
    std::string data;

    while (stream >> buf) {

        if (buf == "add" && stream.peek() != '\n' && stream >> key &&
                    stream.peek() != '\n' && stream >> data && stream.peek() == '\n') {
            bool success = my_tree.add(key, data);

            if(!success) {
                out << "error\n";
            }
        }
        else if (buf == "set" && stream.peek() != '\n' && stream >> key &&
                 stream.peek() != '\n' && stream >> data && stream.peek() == '\n' && !my_tree.is_empty()) {
            bool success = my_tree.set(key, data);

            if(!success) {
                out << "error\n";
            }
        }
        else if (buf == "search" && stream.peek() != '\n' && stream >> key && stream.peek() == '\n') {
            node<std::string>* node = my_tree.search(key);
            if (node) {
                out << 1 << ' ' << node->data << '\n';
            }
            else {
                out << 0 << '\n';
            }
        }
        else if (buf == "delete" && stream.peek() != '\n' && stream >> key && stream.peek() == '\n' && !my_tree.is_empty()) {
            bool success = my_tree.delete_node(key);

            if(!success) {
                out << "error\n";
            }
        }
        else if (buf == "min" && stream.peek() == '\n' && !my_tree.is_empty()) {
            const node<std::string>* min = my_tree.min();
            out << min->key << ' ' << min->data << '\n';
        }
        else if (buf == "max" && stream.peek() == '\n' && !my_tree.is_empty()) {
            const node<std::string>* max = my_tree.max();
            out << max->key << ' ' << max->data << '\n';
        }
        else if (buf == "print" && stream.peek() == '\n') {
            out << my_tree.print();
        }
        else {
            out << "error\n";
            stream.ignore(256, '\n');
        }
    }

    return out.str();
}

int main() {
    std::cout << test();

    return 0;
}
