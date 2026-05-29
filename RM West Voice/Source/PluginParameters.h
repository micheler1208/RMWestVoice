/*
  ==============================================================================

    PluginParameters.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace RMWestVoiceParameters
{
namespace ID
{
    inline constexpr auto wave = "WAVE";
    inline constexpr auto detuneCents = "DETUNE_CENTS";

    inline constexpr auto ampAttack = "AMP_ATTACK";
    inline constexpr auto ampDecay = "AMP_DECAY";
    inline constexpr auto ampSustain = "AMP_SUSTAIN";
    inline constexpr auto ampRelease = "AMP_RELEASE";

    inline constexpr auto filterCutoff = "FILTER_CUTOFF";
    inline constexpr auto filterResonance = "FILTER_RESONANCE";

    inline constexpr auto outputHighpassCutoff = "OUTPUT_HIGHPASS_CUTOFF";
    inline constexpr auto outputHighpassResonance = "OUTPUT_HIGHPASS_RESONANCE";
    inline constexpr auto outputGain = "OUTPUT_GAIN";
}

namespace Wave
{
    inline constexpr int tri = 0;
    inline constexpr int saw = 1;
}

namespace FilterType
{
    inline constexpr int lowPass = 0;
    inline constexpr int bandPass = 1;
    inline constexpr int highPass = 2;
}

inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        ID::wave,
        "Wave",
        juce::StringArray { "Tri", "Saw" },
        Wave::tri));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::detuneCents,
        "Detune",
        juce::NormalisableRange<float> { -25.0f, 25.0f, 0.1f },
        0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::ampAttack,
        "Amp Attack",
        juce::NormalisableRange<float> { 0.1f, 1.0f },
        0.1f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::ampDecay,
        "Amp Decay",
        juce::NormalisableRange<float> { 0.1f, 1.0f },
        1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::ampSustain,
        "Amp Sustain",
        juce::NormalisableRange<float> { 0.1f, 1.0f },
        0.8f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::ampRelease,
        "Amp Release",
        juce::NormalisableRange<float> { 0.1f, 3.0f },
        0.2f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::filterCutoff,
        "Filter Cutoff",
        juce::NormalisableRange<float> { 20.0f, 20000.0f, 0.1f, 0.6f },
        4800.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::filterResonance,
        "Filter Resonance",
        juce::NormalisableRange<float> { 1.0f, 10.0f, 0.1f },
        1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::outputHighpassCutoff,
        "Output Highpass Cutoff",
        juce::NormalisableRange<float> { 20.0f, 20000.0f, 0.1f, 0.6f },
        250.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::outputHighpassResonance,
        "Output Highpass Resonance",
        juce::NormalisableRange<float> { 1.0f, 10.0f, 0.1f },
        1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::outputGain,
        "Output Gain",
        juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f },
        0.6f));

    return { params.begin(), params.end() };
}
} // namespace RMWestVoiceParameters
