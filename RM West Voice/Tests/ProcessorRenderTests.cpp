#include "PluginParameters.h"
#include "PluginProcessor.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

namespace
{
int failures = 0;

void expect(bool condition, const std::string& message)
{
    if (condition)
        return;

    std::cerr << "FAIL: " << message << '\n';
    ++failures;
}

void setParameter(RMWestVoiceAudioProcessor& processor, const char* parameterID, float value)
{
    auto* parameter = processor.apvts.getParameter(parameterID);
    expect(parameter != nullptr, std::string("parameter exists: ") + parameterID);

    if (parameter == nullptr)
        return;

    parameter->beginChangeGesture();
    parameter->setValueNotifyingHost(parameter->convertTo0to1(value));
    parameter->endChangeGesture();
}

float getRawParameterValue(const RMWestVoiceAudioProcessor& processor, const char* parameterID)
{
    const auto* value = processor.apvts.getRawParameterValue(parameterID);
    expect(value != nullptr, std::string("raw parameter exists: ") + parameterID);
    return value != nullptr ? value->load() : 0.0f;
}

struct RenderStats
{
    float maxAbs { 0.0f };
    float finalBlockMaxAbs { 0.0f };
    bool allFinite { true };
};

void addScheduledMidi(juce::MidiBuffer& midi, int absoluteStart, int blockSize)
{
    const auto addEvent = [&midi, absoluteStart, blockSize](juce::MidiMessage message, int absoluteSample)
    {
        if (absoluteSample < absoluteStart || absoluteSample >= absoluteStart + blockSize)
            return;

        midi.addEvent(message, absoluteSample - absoluteStart);
    };

    addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(105)), 0);
    addEvent(juce::MidiMessage::pitchWheel(1, 12000), 6615);
    addEvent(juce::MidiMessage::controllerEvent(1, 1, 96), 8820);
    addEvent(juce::MidiMessage::channelPressureChange(1, 80), 11025);
    addEvent(juce::MidiMessage::noteOn(1, 64, static_cast<juce::uint8>(112)), 13230);
    addEvent(juce::MidiMessage::aftertouchChange(1, 64, 92), 19845);
    addEvent(juce::MidiMessage::noteOff(1, 64), 24255);
    addEvent(juce::MidiMessage::noteOff(1, 60), 30870);
    addEvent(juce::MidiMessage::noteOn(1, 67, static_cast<juce::uint8>(108)), 55125);
    addEvent(juce::MidiMessage::pitchWheel(1, 5200), 59535);
    addEvent(juce::MidiMessage::controllerEvent(1, 1, 112), 61740);
    addEvent(juce::MidiMessage::noteOff(1, 67), 79380);
}

RenderStats renderPhrase(RMWestVoiceAudioProcessor& processor)
{
    constexpr auto sampleRate = 44100.0;
    constexpr auto blockSize = 256;
    constexpr auto outputChannels = 2;
    constexpr auto totalSamples = static_cast<int>(sampleRate * 4.0);

    juce::AudioBuffer<float> buffer(outputChannels, blockSize);
    RenderStats stats;
    bool changedPreset = false;

    for (int absoluteStart = 0; absoluteStart < totalSamples; absoluteStart += blockSize)
    {
        if (! changedPreset && absoluteStart <= 44100 && 44100 < absoluteStart + blockSize)
        {
            processor.setCurrentProgram(2);
            changedPreset = true;
        }

        buffer.clear();

        juce::MidiBuffer midi;
        addScheduledMidi(midi, absoluteStart, blockSize);

        processor.processBlock(buffer, midi);

        float blockMaxAbs = 0.0f;
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            const auto* data = buffer.getReadPointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                const auto value = data[sample];
                stats.allFinite = stats.allFinite && std::isfinite(value);
                blockMaxAbs = std::max(blockMaxAbs, std::abs(value));
            }
        }

        stats.maxAbs = std::max(stats.maxAbs, blockMaxAbs);
        stats.finalBlockMaxAbs = blockMaxAbs;
    }

    return stats;
}

void testProcessorPresetStateRestoresProgramAndParameters()
{
    RMWestVoiceAudioProcessor processor;
    processor.setPlayConfigDetails(0, 2, 44100.0, 256);
    processor.prepareToPlay(44100.0, 256);

    processor.setCurrentProgram(4);

    juce::MemoryBlock state;
    processor.getStateInformation(state);

    RMWestVoiceAudioProcessor restored;
    restored.setPlayConfigDetails(0, 2, 44100.0, 256);
    restored.prepareToPlay(44100.0, 256);
    restored.setStateInformation(state.getData(), static_cast<int>(state.getSize()));

    expect(restored.getCurrentProgram() == 4, "factory program index restores through processor state");
    expect(getRawParameterValue(restored, RMWestVoiceParameters::ID::bendRange) == 2.0f, "choice bend range restores as preset index");
    expect(getRawParameterValue(restored, RMWestVoiceParameters::ID::notePriority) == 0.0f, "note priority restores as preset index");
}

void testProcessorRendersStableMidiPhrase()
{
    RMWestVoiceAudioProcessor processor;
    processor.setPlayConfigDetails(0, 2, 44100.0, 256);
    processor.prepareToPlay(44100.0, 256);

    setParameter(processor, RMWestVoiceParameters::ID::notePriority, 1.0f);
    setParameter(processor, RMWestVoiceParameters::ID::bendRange, 3.0f);
    setParameter(processor, RMWestVoiceParameters::ID::delayMix, 0.0f);
    setParameter(processor, RMWestVoiceParameters::ID::reverbMix, 0.0f);

    const auto stats = renderPhrase(processor);

    expect(stats.allFinite, "rendered processor output contains only finite samples");
    expect(stats.maxAbs > 0.0001f, "rendered processor output is nonzero");
    expect(stats.maxAbs < 2.5f, "rendered processor output stays within a bounded gain range");
    expect(stats.finalBlockMaxAbs < 0.001f, "processor release tail returns close to silence");
}
} // namespace

int main()
{
    juce::ScopedJuceInitialiser_GUI juceInitialiser;

    testProcessorPresetStateRestoresProgramAndParameters();
    testProcessorRendersStableMidiPhrase();

    if (failures != 0)
        return 1;

    std::cout << "Processor render tests passed\n";
    return 0;
}
