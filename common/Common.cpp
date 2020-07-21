//
// Created by Darwin on 2020/7/19.
//

#include <string>
#include <iostream>

#include "Common.h"

namespace common {

// https://zhuanlan.zhihu.com/p/94588204
//      -fno-elide-constructors
struct Value {
 public:
    Value() : data("data") {
        // std::cout << "base" << std::endl;
    }

    Value(const Value &v) {
        std::cout << "copy ****" << std::endl;
        data = v.data;
    }
    void operator=(const Value &v) {
        std::cout << "equal ****" << std::endl;
        data = v.data;
    }


    Value(Value &&v) noexcept {
        std::cout << "move ----" << std::endl;
        data = std::move(v.data);
    }
    void operator=(Value&& v) noexcept {
        std::cout << "equal ----" << std::endl;
        data = std::move(v.data);
    }

    std::string data;
};

Value get(bool move) {
    Value v;
    if (move) {
        return std::move(v);
    } else {
        return v;
    }
}

void put1(const Value& v) {
    std::cout << "before" << std::endl;
    Value a = std::move(v);
    std::cout << "after" << std::endl;
}

void put2(Value v) {
    std::cout << "before" << std::endl;
    Value a = std::move(v);
    std::cout << "after" << std::endl;
}

void put3(Value&& v) {
    std::cout << "before" << std::endl;
    Value a = std::move(v);
    std::cout << "after" << std::endl;

}

void Test() {
    std::cout << "\ndefault" << std::endl;
    Value v1 = get(false);

    std::cout << "\nexplict move" << std::endl;
    Value v2 = get(true);

    Value d;
    std::cout << "\nput1" << std::endl;
    put1(d);

    std::cout << "\nput2" << std::endl;
    put2(std::move(d));

    std::cout << "\nput3" << std::endl;
    put3(std::move(d));
}
} // namespace common