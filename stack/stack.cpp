#include <iostream>
#include <sstream>
#include <string>

template <typename T>
class stack {
private:
    T* data;
    size_t _size;
    size_t capacity;

public:
    stack() : data{}, _size{}, capacity{} {}

    explicit stack(size_t& n) {
        _size = 0;
        capacity = n;
        data = new T[capacity];
    }

    T push (const T& elem) {
        data[_size] = elem;

        return data[_size++];
    }

    T pop() {
        if ( empty() ) {
            throw("");
        }

        T elem = data[--_size];

        return elem;
    }

    bool empty() {
        return _size == 0;
    }

    size_t size() {
        return _size;
    }

    std::string print() {
        std::stringstream str{};
        if (_size == 0) {
            return "";
        }

        for (size_t i = 0; i < _size - 1; ++i) {
            str << data[i] << ' ';
        }

        str << data[_size - 1] << '\n';

        return str.str();
    }

    ~stack() {
        delete [] data;
    }
};

std::string get_data() {
    std::string str{}, buf{};

    while (std::getline(std::cin, buf, '\0')) {
        if ( !buf.empty() ) {
            str += buf + '\n';
        }
        else {
            break;
        }
    }

    return str;
}

std::string test() {

    std::istringstream stream(get_data());
    std::ostringstream out{};
    size_t num{};
    std::string buf{};

    while (stream >> buf) {
        if ( buf.empty() ) {
            return "";
        }

        if (buf == "set_size" && stream.peek() != '\n') {
            stream >> buf;

            try {
                num = std::stoi(buf);
                break;
            }
            catch(...) {
                out << "error\n";
                stream.ignore(256, '\n');
            }
        }
        else {
            out << "error\n";
            stream.ignore(256, '\n');
        }
    }

    stack<std::string> my_stack(num);
    std::string t;

    while ( stream >> buf ) {
        if ( buf.empty() ) {
            break;
        }

        if (buf == "push" && stream >> t && stream.peek() == '\n') {

            if (my_stack.size() < num) {
                my_stack.push(t);
            }
            else {
                out << "overflow\n";
            }
        }
        else if (buf == "pop" && stream.peek() == '\n') {
            try {
                out << my_stack.pop() << '\n';
            }
            catch(...) {
                out << "underflow\n";
            }
        }
        else if (buf == "print" && stream.peek() == '\n') {
            if (my_stack.empty()) {
                out << "empty\n";
            }
            else {
                out << my_stack.print();
            }
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
}
