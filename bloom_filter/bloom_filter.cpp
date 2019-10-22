#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

std::vector<size_t> get_n_prime_numbers(size_t n) {
    std::vector<size_t> prime_numbers;
    prime_numbers.push_back(2);

    for (size_t i = 3; prime_numbers.size() < n; i += 2) {
        bool is_prime = true;

        for (size_t j = 3; j <= i / 2; j += 2) {
            is_prime = i % j != 0;

            if (!is_prime) {
                break;
            }
        }

        if (is_prime) {
            prime_numbers.push_back(i);
        }
    }

    return prime_numbers;
}

class Bloom_filter {

    class my_function {
        size_t i;
        size_t p;
        unsigned long long M;
        size_t m;

    public:
        my_function(const size_t& _i, const size_t& _p, const unsigned long long& _M, const size_t& _m) : i{_i}, p{_p}, M{_M}, m{_m} {}

        size_t operator()(const unsigned long long& key) {
            auto term_one = static_cast<unsigned long long>(i+1) * static_cast<unsigned long long>(key % M);
            auto term_two = static_cast<unsigned long long>(p % M);

            auto sum = static_cast<unsigned long long>((term_one + term_two) % M);

            auto index = static_cast<size_t>(sum % m);

            return index;
        }
    };

    size_t m;
    size_t k;
    std::vector<bool> bit_array;
    std::vector<my_function> func_array;

public:
    Bloom_filter(const size_t& _m, const size_t& _k) : m{_m}, k{_k}, bit_array(_m) {
        const unsigned long long M = (static_cast<unsigned long long>(1) << 31) - 1;

        std::vector<size_t> prime_numbers = get_n_prime_numbers(k);

        for (size_t i = 0; i < k; ++i) {
            my_function func(i, prime_numbers[i], M, m);

            func_array.push_back(func);
        }
    }

    void add(const unsigned long long& key) {
        for (auto& h : func_array) {
            bit_array[ h(key) ] = true;
        }
    }

    bool search(const unsigned long long& key) {
        bool success = true;

        for (auto& h : func_array) {
            success &= bit_array[ h(key) ];

            if (!success) {
                break;
            }
        }

        return success;
    }

    std::string print() {
        std::string str{};

        for (auto bit : bit_array) {
            str += bit ? '1' : '0';
        }

        str += '\n';

        return str;
    }
};

std::string get_data() {
    std::string buf{};

    std::getline(std::cin, buf, '\0');
    return buf;
}

std::pair<int, int> calculate_m_and_k(const size_t& n, const double& p) {

    size_t m = std::round(-static_cast<int>(n) * (std::log(p) / std::log(2)) / std::log(2));
    size_t k = std::round(-std::log(p) / std::log(2));

    return {m,k};
}

std::string test() {

    std::istringstream stream(get_data());
    std::ostringstream out{};
    std::string buf{};

    double p;
    size_t n;
    std::pair<int, int> pair_of_parametrs;

    while (stream >> buf) {
        if ( buf.empty() ) {
            return "";
        }

        if (buf == "set" && stream.peek() != '\n' && stream >> buf && stream.peek() != '\n') {
            try {
                n = std::stoi(buf);

                stream >> buf;
                p = std::stod(buf);

                if (stream.peek() == '\n') {
                    pair_of_parametrs = calculate_m_and_k(n, p);

                    if (pair_of_parametrs.first > 0 && pair_of_parametrs.second > 0) {
                        out << pair_of_parametrs.first << ' ' << pair_of_parametrs.second << '\n';
                        break;
                    }
                }
            }
            catch(...) {}
        }

        out << "error\n";
        stream.ignore(256, '\n');
    }

    if (pair_of_parametrs.first > 0 && pair_of_parametrs.second > 0) {

        Bloom_filter my_filter(pair_of_parametrs.first, pair_of_parametrs.second);

        unsigned long long t;

        while (stream >> buf) {
            if (buf.empty()) {
                break;
            }

            if (buf == "add" && stream.peek() != '\n' && stream >> t && stream.peek() == '\n') {
                my_filter.add(t);
            } else if (buf == "search" && stream.peek() != '\n' && stream >> t && stream.peek() == '\n') {
                out << my_filter.search(t) << '\n';
            } else if (buf == "print" && stream.peek() == '\n') {
                out << my_filter.print();
            } else {
                out << "error\n";
                stream.ignore(256, '\n');
            }
        }
    }

    return out.str();
}

int main() {
    std::cout << test();

    return 0;
}
