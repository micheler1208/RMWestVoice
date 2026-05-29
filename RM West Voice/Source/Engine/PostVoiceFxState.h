#pragma once

namespace RMWestVoice
{
struct PostVoiceFxSettings
{
    float width { 0.18f };
    float delayMix { 0.0f };
    float delayTimeSeconds { 0.28f };
    float delayFeedback { 0.25f };
    float reverbMix { 0.0f };
    float reverbSize { 0.35f };
    float reverbDamping { 0.45f };
    int doublerDelaySamples { 0 };
    int delaySamples { 1 };
};

class PostVoiceFxState
{
public:
    void prepare(double newSampleRate) noexcept;

    void setWidth(float normalized) noexcept;
    void setDelayMix(float normalized) noexcept;
    void setDelayTimeSeconds(float seconds) noexcept;
    void setDelayFeedback(float normalized) noexcept;
    void setReverbMix(float normalized) noexcept;
    void setReverbSize(float normalized) noexcept;
    void setReverbDamping(float normalized) noexcept;

    const PostVoiceFxSettings& getSettings() const noexcept { return settings; }

private:
    static float clamp(float value, float low, float high) noexcept;
    void updateDerivedTiming() noexcept;

    double sampleRate { 44100.0 };
    PostVoiceFxSettings settings;
};
} // namespace RMWestVoice
