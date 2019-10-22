#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

template <typename T>
class queue {
private:
    T* data;
    size_t head;
    size_t tail;
    size_t _size;
    size_t capacity;

public:
    queue() : data{}, _size{}, capacity{} {
        head = tail = 0;
    }

    explicit queue(size_t& n) {
        head = tail = 0;
        _size = 0;
        capacity = n;
        data = new T[capacity];
    }

    T push (const T& elem) {
        if (empty()) {
            data[head] = elem;
            tail = (tail + 1) % capacity;
            ++_size;
            return data[head];
        }
        if ( tail == head) {
            throw("");
        }
        else {
            data[tail] = elem;
            tail = (tail + 1) % capacity;
            ++_size;
            return data[tail]; // NOTE
        }
    }

    T pop() {
        if ( empty() ) {
            throw("");
        }

        T elem = data[head];
        head = (head + 1) % capacity;

        --_size;

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

        for (size_t i = head, j = 0; j < _size - 1; ++j, i = (i + 1) % capacity) {
            str << data[i] << ' ';
        }

        str << data[(head + _size - 1) % capacity] << '\n';

        return str.str();
    }

    ~queue() {
        delete [] data;
    }
};

std::string get_data(const char* file) {
    std::string str{}, buf{};
    std::ifstream fin(file);

    if (fin.is_open()) {
        while (std::getline(fin, buf, '\0')) {
            if (!buf.empty()) {
                str += buf + '\n';
            } else {
                break;
            }
        }
        fin.close();
    }

    return str;
}

std::string test(const char* file) {

    std::istringstream stream(get_data(file));
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

    queue<std::string> my_queue(num);
    std::string t;

    while ( stream >> buf ) {
        if ( buf.empty() ) {
            break;
        }

        if (buf == "push" && stream >> t && stream.peek() == '\n') {

            if (my_queue.size() < num) {
                my_queue.push(t);
            }
            else {
                out << "overflow\n";
            }
        }
        else if (buf == "pop" && stream.peek() == '\n') {
            try {
                out << my_queue.pop() << '\n';
            }
            catch(...) {
                out << "underflow\n";
            }
        }
        else if (buf == "print" && stream.peek() == '\n') {
            if (my_queue.empty()) {
                out << "empty\n";
            }
            else {
                out << my_queue.print();
            }
        }
        else {
            out << "error\n";
            stream.ignore(256, '\n');
        }
    }

    return out.str();
}

int main(int argc, char *argv[]) {
    std::ofstream fout(argv[2]);

    if( fout.is_open() ) {
        fout << test(argv[1]);
        fout.close();
    }
}
