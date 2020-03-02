#pragma once

#include <string>

struct WheelInfo
{
    std::string name;

    std::string address;

    std::string key;

    WheelInfo():
        name(std::string()),
        address(std::string()),
        key(std::string()) {}

    WheelInfo(std::string name, std::string address):
        name(name),
        address(address),
        key(std::string()) {}

    WheelInfo(std::string name, std::string address, std::string key):
        name(name),
        address(address),
        key(key) {}

    bool empty() {
        return this->address.empty();
    };

    bool hasKey() {
        return this->key.size() > 0;
    };
};
