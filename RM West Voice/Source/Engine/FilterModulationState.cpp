#include "FilterModulationState.h"

#include <algorithm>
#include <cmath>

namespace RMWestVoice
{
void FilterModulationState::prepare(double newSampleRate) noexcept
{
    sampleRate = newSampleRate > 0.0 ? newSampleRate : 44100.0;
}

void FilterModulationState::setBaseCutoffHz(float hz) noexcept
{
    baseCutoffHz = clamp(hz, 20.0f, 20000.0f);
}

void FilterModulationState::setKeyTracking(float normalized) noexcept
{
    keyTracking = clamp(normalized, 0.0f, 1.0f);
}

void FilterModulationState::setEnvelopeAmountOctaves(float octaves) noexcept
{
    envelopeAmountOctaves = clamp(octaves, 0.0f, 4.0f);
}

void FilterModulationState::setCurrentMidiNote(int midiNoteNumber) noexcept
{
    currentMidiNote = static_cast<int>(clamp(static_cast<float>(midiNoteNumber), 0.0f, 127.0f));
}

float FilterModulationState::getCutoffHz(float envelopeLevel) const noexcept
{
    const auto noteOffsetOctaves = (static_cast<float>(currentMidiNote - 60) / 12.0f) * keyTracking;
    const auto envelopeOffsetOctaves = clamp(envelopeLevel, 0.0f, 1.0f) * envelopeAmountOctaves;
    const auto cutoffHz = baseCutoffHz * std::pow(2.0f, noteOffsetOctaves + envelopeOffsetOctaves);
    const auto nyquistSafeMaximum = static_cast<float>(sampleRate * 0.45);

    return clamp(cutoffHz, 20.0f, std::min(20000.0f, nyquistSafeMaximum));
}

float FilterModulationState::clamp(float value, float low, float high) noexcept
{
    return std::min(std::max(value, low), high);
}
} // namespace RMWestVoice
