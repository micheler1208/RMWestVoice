/*
  ==============================================================================

    GlideState.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

namespace RMWestVoice
{
class GlideState
{
public:
    enum class Mode
    {
        off,
        always,
        autoLegato
    };

    void prepare(double newSampleRate) noexcept;
    void reset() noexcept;

    void setMode(Mode newMode) noexcept { mode = newMode; }
    Mode getMode() const noexcept { return mode; }

    void setTimeSecondsPerOctave(float newTimeSecondsPerOctave) noexcept;
    float getTimeSecondsPerOctave() const noexcept { return timeSecondsPerOctave; }

    void setCurrentFrequency(float frequencyHz) noexcept;
    void startTransition(float targetFrequencyHz, bool isLegato) noexcept;

    float getNextFrequency() noexcept;
    float getCurrentFrequency() const noexcept;

    bool isGliding() const noexcept { return remainingSamples > 0; }
    int getRemainingSamples() const noexcept { return remainingSamples; }

private:
    static float sanitizeFrequency(float frequencyHz) noexcept;
    static double frequencyToLog2(float frequencyHz) noexcept;
    static float log2ToFrequency(double log2Frequency) noexcept;

    bool shouldGlide(bool isLegato) const noexcept;

    double sampleRate { 44100.0 };
    double currentLog2Frequency { 0.0 };
    double targetLog2Frequency { 0.0 };
    double log2Increment { 0.0 };

    int remainingSamples { 0 };
    bool hasFrequency { false };

    float timeSecondsPerOctave { 0.08f };
    Mode mode { Mode::autoLegato };
};
} // namespace RMWestVoice
