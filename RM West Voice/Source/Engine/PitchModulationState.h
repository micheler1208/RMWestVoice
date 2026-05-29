#pragma once

namespace RMWestVoice
{
class PitchModulationState
{
public:
    void prepare(double newSampleRate) noexcept;
    void reset() noexcept;

    void setPitchBendRangeSemitones(float semitones) noexcept;
    void setPitchWheel(float normalizedBipolar) noexcept;
    void setModWheel(float normalized) noexcept;
    void setAftertouch(float normalized) noexcept;

    void setVibratoDepthCents(float cents) noexcept;
    void setVibratoRateHz(float hz) noexcept;
    void setVibratoFadeSeconds(float seconds) noexcept;
    void setVibratoAftertouchAmount(float normalized) noexcept;

    void noteStarted(bool isLegato) noexcept;
    void noteStopped() noexcept;

    float getNextPitchRatio() noexcept;
    float getCurrentPitchBendSemitones() const noexcept { return currentPitchBendSemitones; }
    float getCurrentVibratoCents() const noexcept { return currentVibratoCents; }

private:
    static float clamp(float value, float low, float high) noexcept;
    static float centsToPitchRatio(float cents) noexcept;
    static float calculateSmoothingCoefficient(double sampleRate, float seconds) noexcept;

    void updatePitchWheelSmoothing() noexcept;

    double sampleRate { 44100.0 };
    float pitchWheelSmoothingCoefficient { 0.0f };

    float targetPitchWheel { 0.0f };
    float currentPitchWheel { 0.0f };
    float pitchBendRangeSemitones { 12.0f };
    float currentPitchBendSemitones { 0.0f };

    float modWheel { 0.0f };
    float aftertouch { 0.0f };
    float vibratoDepthCents { 35.0f };
    float vibratoRateHz { 5.5f };
    float vibratoFadeSeconds { 0.15f };
    float vibratoAftertouchAmount { 0.0f };
    float vibratoFadeLevel { 0.0f };
    float vibratoPhaseRadians { 0.0f };
    float currentVibratoCents { 0.0f };

    bool noteIsActive { false };
};
} // namespace RMWestVoice
