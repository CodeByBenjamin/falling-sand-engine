#pragma once
#include <cstdint>

class Rng {
private:
    uint32_t state;
public:
    explicit Rng(uint32_t seed = 12345) : state(seed ? seed : 1) {}

    uint32_t next() {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    bool coin() { return (next() >> 16) & 1; }

    float randFloat() {
        return static_cast<float>(next() >> 8) * (1.0f / 16777216.0f);
    }
};