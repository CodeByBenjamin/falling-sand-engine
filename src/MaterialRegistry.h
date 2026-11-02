#pragma once
#include "CellProperties.h"
#include <array>
#include <cstddef>
#include <string>

class MaterialRegistry {
  private:
    CellProperties properties[static_cast<size_t>(CellType::Count)];
    std::array<bool, static_cast<size_t>(CellType::Count)> loaded;

  public:
    MaterialRegistry() = default;
    bool load();

    const CellProperties &get(CellType type) const {
        return properties[static_cast<size_t>(type)];
    };
};