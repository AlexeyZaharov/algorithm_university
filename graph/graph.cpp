#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <unordered_set>

std::string& back(std::stack<std::string>& stack) {
    return stack.top();
}

std::string& back(std::queue<std::string>& queue) {
    return queue.front();
}

void insert(std::stack<std::string>& stack, std::vector<std::string>& vec, const std::unordered_set<std::string>& visited) {
    std::sort(vec.begin(), vec.end());
    for(auto v = vec.rbegin(); v != vec.rend(); ++v) {
        if (visited.find(*v) == visited.end())
            stack.push(*v);
    }
}

void insert(std::queue<std::string>& queue, std::vector<std::string>& vec, const std::unordered_set<std::string>& visited) {
    std::sort(vec.begin(), vec.end());
    for(auto v = vec.begin(); v != vec.end(); ++v) {
        if (visited.find(*v) == visited.end())
            queue.push(*v);
    }
}

class graph {
private:
    bool is_directed, is_DFS;
    std::unordered_map<std::string, std::vector<std::string>> table;
    //std::set<std::string> array_of_vertex;

    std::string start_vertex;

    template <typename T>
    void traversal(T& data_structure, std::string& start, std::unordered_set<std::string>& visited, std::ostringstream& out) {
        data_structure.push(start);

        while (!data_structure.empty()) {
            std::string vertex = back(data_structure);
            data_structure.pop();

            if (visited.find(vertex) == visited.end()) {
                out << vertex << std::endl;
                insert(data_structure, table[vertex], visited);
            }

            visited.insert(vertex);
        }
    }

    template <typename T>
    std::string traversal(T&& data_structure) {
        std::unordered_set<std::string> visited{};
        std::ostringstream out{};

        traversal(data_structure, start_vertex, visited, out);

        /*for (auto& v : array_of_vertex) {
            if (visited.find(v) == visited.end())
                traversal(data_structure, v, visited, out);
        }*/

        return out.str();
    }

public:
    explicit graph (std::istringstream&& data) {
        char type, searching;

        data >> type >> start_vertex >> searching;

        is_directed = type == 'd';
        is_DFS = searching == 'd';

        std::string from, to;

        while (data >> from >> to) {
            //array_of_vertex.insert(from);
            //array_of_vertex.insert(to);

            if(is_directed) {
                table[from].push_back(to);
            }
            else {
                table[from].push_back(to);
                table[to].push_back(from);
            }
        }
    }

    std::string traversal() {
        return is_DFS ? traversal(std::stack<std::string>())
                        : traversal(std::queue<std::string>());
    }
};

std::istringstream get_data() {
    std::string str;
    std::getline(std::cin, str, '\0');

    return std::istringstream(str);
}

int main() {
    graph my_graph(get_data());

    std::cout << my_graph.traversal();

    return 0;
}
