#pragma once

namespace BigNum {
    // For example 2.3e5 would have base = 2.3 and exp = 5
    struct BigInt {
        int base = 0;
        int exp = 0;
    };

    BigInt add(const BigInt &num1, const BigInt &num2);
}