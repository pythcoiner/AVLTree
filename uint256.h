//
// Created by cc1 on 10/12/22.
//

#ifndef UNTITLED_UINT256_H
#define UNTITLED_UINT256_H

#include <iostream>
#include <cstdint>
#include <string>
#include <iomanip>
#include <sstream>
#include <bitset>

// Define the uint256 type as a struct with 8 std::uint64_t data members.
struct uint256 {
    std::uint64_t data[4];

    uint256(std::uint64_t d0, std::uint64_t d1, std::uint64_t d2, std::uint64_t d3){
        this->data[0] = d0;
        this->data[1] = d1;
        this->data[2] = d2;
        this->data[3] = d3;
    }

    uint256(){
        this->data[0] = 0;
        this->data[1] = 0;
        this->data[2] = 0;
        this->data[3] = 0;
    }

    uint256(uint64_t d0){
        this->data[0] = 0;
        this->data[1] = 0;
        this->data[2] = 0;
        this->data[3] = d0;
    }

    uint256(int d0){
        this->data[0] = 0;
        this->data[1] = 0;
        this->data[2] = 0;
        this->data[3] = d0;
    }

    std::string to_hex(){
        std::stringstream ss;
        ss << "0x";
        ss << std::hex << std::setfill('0') << std::setw(16) << std::hex << this->data[0];
        ss << std::hex << std::setfill('0') << std::setw(16) << std::hex << this->data[1];
        ss << std::hex << std::setfill('0') << std::setw(16) << std::hex << this->data[2];
        ss << std::hex << std::setfill('0') << std::setw(16) << std::hex << this->data[3];

        return ss.str();
    }

    // Overload the > operator to compare two uint256 values.
    friend bool operator>(const uint256 &lhs, const uint256 &rhs) {
        for (std::size_t i = 0; i < 4; i++) {
            if (lhs.data[i] > rhs.data[i]) {
                return true;
            } else if (lhs.data[i] < rhs.data[i]) {
                return false;
            }
        }
        return false;
    }

    // Overload the < operator to compare two uint256 values.
    friend bool operator<(const uint256 &lhs, const uint256 &rhs) {
        for (std::size_t i = 0; i < 4; i++) {
            if (lhs.data[i] < rhs.data[i]) {
                return true;
            } else if (lhs.data[i] > rhs.data[i]) {
                return false;
            }
        }
        return false;
    }

    // Overload the == operator to compare two uint256 values.
    friend bool operator==(const uint256 &lhs, const uint256 &rhs) {
        for (std::size_t i = 0; i < 4; i++) {
            if (lhs.data[i] != rhs.data[i]) {
                return false;
            }

        }
        return true;
    }
};
#endif //UNTITLED_UINT256_H
