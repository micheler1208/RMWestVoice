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
    inline constexpr auto oscMix = "OSC_MIX";
    inline constexpr auto detuneCents = "DETUNE_CENTS";
    inline constexpr auto glideMode = "GLIDE_MODE";
    inline constexpr auto glideTime = "GLIDE_TIME";

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
    inline constexpr int saw = 0;
    inline constexpr int tri = 1;
    inline constexpr int sawTri = 2;
    inline constexpr int sawPulse = 3;
}

namespace GlideMode
{
    inline constexpr int off = 0;
    inline constexpr int always = 1;
    inline constexpr int autoLegato = 2;
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
        juce::StringArray { "Saw", "Tri", "Saw+Tri", "Saw+Pulse" },
        Wave::sawTri));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::oscMix,
        "Osc Mix",
        juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f },
        0.35f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::detuneCents,
        "Detune",
        juce::NormalisableRange<float> { -25.0f, 25.0f, 0.1f },
        0.0f));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        ID::glideMode,
        "Glide Mode",
        juce::StringArray { "Off", "Always", "Auto-Legato" },
        GlideMode::autoLegato));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ID::glideTime,
        "Glide Time",
        juce::NormalisableRange<float> { 0.0f, 2.0f, 0.001f, 0.4f },
        0.08f));

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
