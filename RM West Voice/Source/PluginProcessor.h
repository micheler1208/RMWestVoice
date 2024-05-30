/*
  ==============================================================================

    PluginProcessor.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

class RMWestVoiceAudioProcessor  : public juce::AudioProcessor
{
public:
    RMWestVoiceAudioProcessor();
    ~RMWestVoiceAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams(); 


    juce::Synthesiser synth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RMWestVoiceAudioProcessor)
};

