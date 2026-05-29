#pragma once

namespace RMWestVoice
{
class FilterModulationState
{
public:
    void prepare(double newSampleRate) noexcept;

    void setBaseCutoffHz(float hz) noexcept;
    void setKeyTracking(float normalized) noexcept;
    void setEnvelopeAmountOctaves(float octaves) noexcept;
    void setCurrentMidiNote(int midiNoteNumber) noexcept;

    float getCutoffHz(float envelopeLevel) const noexcept;

private:
    static float clamp(float value, float low, float high) noexcept;

    double sampleRate { 44100.0 };
    float baseCutoffHz { 4800.0f };
    float keyTracking { 0.35f };
    float envelopeAmountOctaves { 0.75f };
    int currentMidiNote { 60 };
};
} // namespace RMWestVoice
