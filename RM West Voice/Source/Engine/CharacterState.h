#pragma once

namespace RMWestVoice
{
enum class CharacterMode
{
    analog,
    worm,
    hybrid
};

struct CharacterSettings
{
    float driftDepthCents { 0.0f };
    float sourceDrive { 1.0f };
    float sourceTrim { 1.0f };
    float hybridBlend { 0.0f };
    float filterDriveMultiplier { 1.0f };
    float filterResonanceOffset { 0.0f };
    float filterEnvelopeMultiplier { 1.0f };
};

class CharacterState
{
public:
    void setMode(CharacterMode newMode) noexcept { mode = newMode; }
    CharacterMode getMode() const noexcept { return mode; }
    CharacterSettings getSettings() const noexcept;

    static CharacterMode modeFromIndex(int index) noexcept;
    static CharacterSettings getSettingsForMode(CharacterMode mode) noexcept;

private:
    CharacterMode mode { CharacterMode::analog };
};
} // namespace RMWestVoice
