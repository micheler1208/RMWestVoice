#include "CharacterState.h"

namespace RMWestVoice
{
CharacterSettings CharacterState::getSettings() const noexcept
{
    return getSettingsForMode(mode);
}

CharacterMode CharacterState::modeFromIndex(int index) noexcept
{
    switch (index)
    {
        case 1:
            return CharacterMode::worm;

        case 2:
            return CharacterMode::hybrid;

        case 0:
        default:
            return CharacterMode::analog;
    }
}

CharacterSettings CharacterState::getSettingsForMode(CharacterMode sourceMode) noexcept
{
    switch (sourceMode)
    {
        case CharacterMode::worm:
            return {
                0.45f,
                1.18f,
                0.92f,
                0.0f,
                1.12f,
                0.08f,
                1.15f
            };

        case CharacterMode::hybrid:
            return {
                0.25f,
                1.02f,
                0.94f,
                0.22f,
                0.92f,
                -0.03f,
                0.85f
            };

        case CharacterMode::analog:
        default:
            return {
                0.70f,
                1.06f,
                0.96f,
                0.0f,
                1.0f,
                0.0f,
                1.0f
            };
    }
}
} // namespace RMWestVoice
