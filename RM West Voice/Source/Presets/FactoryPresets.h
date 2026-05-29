#pragma once

#include <array>
#include <cstddef>

namespace RMWestVoice
{
struct FactoryPreset
{
    const char* name;
    std::array<float, 31> values;
};

class FactoryPresets
{
public:
    static constexpr std::size_t parameterCount = 31;

    static int getNumPresets() noexcept;
    static int sanitizeIndex(int index) noexcept;
    static const FactoryPreset& getPreset(int index) noexcept;
    static const char* getParameterID(std::size_t index) noexcept;
};
} // namespace RMWestVoice
