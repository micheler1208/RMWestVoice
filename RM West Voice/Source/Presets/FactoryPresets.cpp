#include "FactoryPresets.h"

#include <algorithm>

namespace
{
constexpr std::array<const char*, RMWestVoice::FactoryPresets::parameterCount> parameterIDs {{
    "CHARACTER",
    "WAVE",
    "OSC_MIX",
    "DETUNE_CENTS",
    "GLIDE_MODE",
    "GLIDE_TIME",
    "NOTE_PRIORITY",
    "BEND_RANGE",
    "VIB_DEPTH",
    "VIB_RATE",
    "VIB_FADE",
    "VIB_AFTERTOUCH",
    "AMP_ATTACK",
    "AMP_DECAY",
    "AMP_SUSTAIN",
    "AMP_RELEASE",
    "FILTER_CUTOFF",
    "FILTER_RESONANCE",
    "DRIVE",
    "FILTER_KEYTRACK",
    "FILTER_ENV_AMOUNT",
    "OUTPUT_HIGHPASS_CUTOFF",
    "OUTPUT_HIGHPASS_RESONANCE",
    "WIDTH",
    "DELAY_MIX",
    "DELAY_TIME",
    "DELAY_FEEDBACK",
    "REVERB_MIX",
    "REVERB_SIZE",
    "REVERB_DAMPING",
    "OUTPUT_GAIN"
}};

constexpr std::array<RMWestVoice::FactoryPreset, 5> presets {{
    {
        "Classic Worm",
        {{
            1.0f, 2.0f, 0.42f, 7.0f, 2.0f, 0.16f, 0.0f, 1.0f, 42.0f, 5.8f,
            0.20f, 0.25f, 0.12f, 0.85f, 0.78f, 0.35f, 3600.0f, 0.42f, 2.1f, 0.48f,
            1.15f, 240.0f, 1.0f, 0.22f, 0.10f, 0.32f, 0.28f, 0.06f, 0.42f, 0.52f,
            0.58f
        }}
    },
    {
        "Smooth Whine",
        {{
            0.0f, 2.0f, 0.35f, 0.0f, 2.0f, 0.08f, 0.0f, 1.0f, 35.0f, 5.5f,
            0.15f, 0.0f, 0.1f, 1.0f, 0.8f, 0.2f, 4800.0f, 0.25f, 1.35f, 0.35f,
            0.75f, 250.0f, 1.0f, 0.18f, 0.0f, 0.28f, 0.25f, 0.0f, 0.35f, 0.45f,
            0.6f
        }}
    },
    {
        "Hybrid Lead",
        {{
            2.0f, 3.0f, 0.55f, 9.0f, 2.0f, 0.10f, 0.0f, 1.0f, 30.0f, 6.1f,
            0.12f, 0.15f, 0.1f, 0.70f, 0.76f, 0.22f, 5200.0f, 0.22f, 1.18f, 0.25f,
            0.50f, 280.0f, 1.0f, 0.30f, 0.05f, 0.24f, 0.18f, 0.10f, 0.56f, 0.40f,
            0.55f
        }}
    },
    {
        "Dry Analog",
        {{
            0.0f, 0.0f, 0.20f, -4.0f, 2.0f, 0.05f, 0.0f, 1.0f, 25.0f, 5.2f,
            0.08f, 0.0f, 0.1f, 0.55f, 0.85f, 0.14f, 6200.0f, 0.18f, 1.2f, 0.25f,
            0.35f, 220.0f, 1.0f, 0.05f, 0.0f, 0.22f, 0.1f, 0.0f, 0.30f, 0.50f,
            0.64f
        }}
    },
    {
        "Wide Mix Lead",
        {{
            0.0f, 2.0f, 0.38f, -7.0f, 1.0f, 0.22f, 0.0f, 2.0f, 38.0f, 4.8f,
            0.18f, 0.0f, 0.12f, 0.90f, 0.72f, 0.50f, 2800.0f, 0.36f, 1.65f, 0.55f,
            1.40f, 250.0f, 1.1f, 0.25f, 0.22f, 0.38f, 0.42f, 0.12f, 0.48f, 0.58f,
            0.54f
        }}
    }
}};
} // namespace

namespace RMWestVoice
{
int FactoryPresets::getNumPresets() noexcept
{
    return static_cast<int>(presets.size());
}

int FactoryPresets::sanitizeIndex(int index) noexcept
{
    return index >= 0 && index < getNumPresets() ? index : 0;
}

const FactoryPreset& FactoryPresets::getPreset(int index) noexcept
{
    return presets[static_cast<std::size_t>(sanitizeIndex(index))];
}

const char* FactoryPresets::getParameterID(std::size_t index) noexcept
{
    return index < parameterIDs.size() ? parameterIDs[index] : "";
}
} // namespace RMWestVoice
