#include <iostream>
#include <sstream>
#include <vector>
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
                index_of_min = vector[index_of_min].first <= vector[left_child].first ? index_of_min : left_child;
            }

            if (right_child < vector.size()) {
                index_of_min = vector[index_of_min].first <= vector[right_child].first ? index_of_min : right_child;
            }

            std::swap(vector[ind], vector[index_of_min]);
            std::swap(map[vector[ind].first], map[vector[index_of_min].first]);

            if(ind != index_of_min) {
                ind = index_of_min;
                left_child = 2*ind + 1;
                right_child = 2*ind + 2;
            }
            else {
                break;
            }
        }
    }

    std::string elem_to_str(const size_t& ind) {
        std::ostringstream out;

        out << '[' << vector[ind].first << ' ' << vector[ind].second;
        if (ind != 0) {
            out << ' ' << vector[static_cast<size_t>( (ind - 1)/2 )].first;
        }
        out << ']';

        return out.str();
    }

    void delete_elem(size_t ind) {
        std::swap(vector[ind], vector[ vector.size() - 1]);
        map[vector[ind].first] = ind;

        long long key_of_sift_elem = vector[ind].first;
        long long key_of_delete_elem = vector[ vector.size() - 1].first;

        vector.pop_back();

        sift_up(ind);
        sift_down(map[key_of_sift_elem]);

        map.erase(map.find(key_of_delete_elem));
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
            vector[map[key]].second = new_value;
        }
        else {
            success = false;
        }

        return success;
    }

    std::pair<long long, T> extract() {
        std::pair<long long, T> root = vector[0];

        delete_elem(0);

        return root;
    }

    bool delete_elem_by_key(const long long& key) {
        bool success = true;

        if (map.find(key) != map.end()) {
            delete_elem(map[key]);
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

    std::string print() {
        std::ostringstream out{};
        std::string empty_node("_");
        size_t num_of_printed_elements = 0;

        if (vector.size() == 0) {
            out << empty_node << '\n';
            return out.str();
        }

        for (size_t index = 0; index < vector.size(); ++index) {
            out << elem_to_str(index);
            ++num_of_printed_elements;

            out << (((num_of_printed_elements + 1) & num_of_printed_elements) == 0 ? '\n' : ' ');
        }

        while (((num_of_printed_elements + 1) & num_of_printed_elements) != 0) {
            ++num_of_printed_elements;
            out << empty_node;
            out << (((num_of_printed_elements + 1) & num_of_printed_elements) == 0 ? '\n' : ' ');
        }

        return out.str();
    }

    std::tuple<long long, size_t, T> min() const {
        return {vector[0].first, 0, vector[0].second};
    }

    std::tuple<long long, size_t, T> max() {
        std::pair<long long, T> max = vector[0];

        for (auto& elem : vector) {
            if (elem.first > max.first) {
                max = elem;
            }
        }

        return {max.first, map[max.first], max.second};
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

    long long key{};
    std::string data{};

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
                out << std::get<0>(tuple) << ' ' << std::get<1>(tuple) << ' ' << std::get<2>(tuple) << '\n';
            }
            else {
                out << 0 << '\n';
            }
        }
        else if (buf == "delete" && stream.peek() != '\n' && stream >> key && stream.peek() == '\n' && !my_heap.is_empty()) {
            bool success = my_heap.delete_elem_by_key(key);

            if(!success) {
                out << "error\n";
            }
        }
        else if (buf == "extract" && stream.peek() == '\n' && !my_heap.is_empty()) {
            std::pair<long long, std::string> root = my_heap.extract();

            out << root.first << ' ' << root.second << '\n';
        }
        else if (buf == "min" && stream.peek() == '\n' && !my_heap.is_empty()) {
            std::tuple<long long, size_t, std::string> min = my_heap.min();
            out << std::get<0>(min) << ' ' << std::get<1>(min) << ' ' << std::get<2>(min) << '\n';
        }
        else if (buf == "max" && stream.peek() == '\n' && !my_heap.is_empty()) {
            std::tuple<long long, size_t, std::string> max = my_heap.max();
            out << std::get<0>(max) << ' ' << std::get<1>(max) << ' ' << std::get<2>(max) << '\n';
        }
        else if (buf == "print" && stream.peek() == '\n') {
            out << my_heap.print();
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
