#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <cmath>
#include <unordered_map>

template <typename T>
class my_heap {
    std::vector< std::pair<long long, T> > vector;
    std::unordered_map<long long, size_t> map;

    void sift_up(size_t ind) {
        while(ind > 0) {
            size_t parent = (ind - 1)/2;

            if (vector[ind].first >= vector[parent].first) {
                return;
            }
            else {
                std::swap(vector[ind], vector[parent]);
                std::swap(map[vector[ind].first], map[vector[parent].first]);
                ind = parent;
            }
        }
    }

    void sift_down(size_t ind) {
        size_t left_child = 2*ind + 1;
        size_t right_child = 2*ind + 2;
        size_t index_of_min = ind;

        while (left_child < vector.size() || right_child < vector.size()) {
            if (left_child < vector.size()) {
                index_of_min = vector[index_of_min] < vector[left_child] ? index_of_min : left_child;
            }

            if (right_child < vector.size()) {
                index_of_min = vector[index_of_min] < vector[right_child] ? index_of_min : right_child;
            }

            std::swap(vector[ind], vector[index_of_min]);
            std::swap(map[vector[ind].first], map[vector[index_of_min].first]);

            ind = index_of_min;
        }
    }


public:
    my_heap() : vector{} {}

    bool add(const long long& key, const T& value) {
        bool success = true;

        if (map.find(key) == map.end()) {
            vector.push_back({key, value});
            map[vector[vector.size() - 1].first] = vector.size() - 1;
            sift_up(vector.size() - 1);
        }
        else {
            success = false;
        }

        return success;
    }

    bool set(const long long& key, const T& new_value) {
        bool success = true;

        if (map.find(key) != map.end()) {
            T old_value = vector[map[key]].second;
            vector[map[key]].second = new_value;

            if (old_value < new_value) {
                sift_down(map[key]);
            }
            else {
                sift_up(map[key]);
            }
        }
        else {
            success = false;
        }

        return success;
    }

    std::tuple<bool, size_t, T> search(const long long& key) {
        std::tuple<bool, size_t, T> returned_data = {false, 0, T{}};

        if (map.find(key) != map.end()) {
            std::get<0>(returned_data) = true;
            std::get<1>(returned_data) = map[key];
            std::get<2>(returned_data) = vector[map[key]].second;
        }

        return returned_data;
    }

    bool is_empty() const {
        return vector.empty();
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

    my_heap<std::string> my_heap;

    long long key;
    std::string data;

    while (stream >> buf) {

        if (buf == "add" && stream.peek() != '\n' && stream >> key &&
                    stream.peek() != '\n' && stream >> data && stream.peek() == '\n') {
            bool success = my_heap.add(key, data);

            if(!success) {
                out << "error\n";
            }
        }
        else if (buf == "set" && stream.peek() != '\n' && stream >> key &&
                 stream.peek() != '\n' && stream >> data && stream.peek() == '\n' && !my_heap.is_empty()) {
            bool success = my_heap.set(key, data);

            if(!success) {
                out << "error\n";
            }
        }
        else if (buf == "search" && stream.peek() != '\n' && stream >> key && stream.peek() == '\n') {
            auto tuple = my_heap.search(key);
            if (std::get<0>(tuple)) {
                out << std::get<0>(tuple) << ' ' << std::get<1>(tuple) << ' ' << std::get<2>(tuple) <<'\n';
            }
            else {
                out << 0 << '\n';
            }
        }
        else if (buf == "delete" && stream.peek() != '\n' && stream >> key && stream.peek() == '\n' && !my_heap.is_empty()) {
            /*bool success = my_heap.delete_node(key);

            if(!success) {
                out << "error\n";
            }*/
        }
        else if (buf == "extract" && stream.peek() != '\n' && stream >> key && stream.peek() == '\n' && !my_heap.is_empty()) {
            /*bool success = my_heap.extract(key);

            if(!success) {
                out << "error\n";
            }*/
        }
        else if (buf == "min" && stream.peek() == '\n' && !my_heap.is_empty()) {
            /*const node<std::string>* min = my_heap.min();
            out << min->key << ' ' << min->data << '\n';*/
        }
        else if (buf == "max" && stream.peek() == '\n' && !my_heap.is_empty()) {
            /*const node<std::string>* max = my_heap.max();
            out << max->key << ' ' << max->data << '\n';*/
        }
        else if (buf == "print" && stream.peek() == '\n') {
            //out << my_heap.print();
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
