/*
  ==============================================================================

    PluginProcessor.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

// CONSTRUCTOR
RMWestVoiceAudioProcessor::RMWestVoiceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
          #if ! JucePlugin_IsMidiEffect
           #if ! JucePlugin_IsSynth
            .withInput("Input", juce::AudioChannelSet::stereo(), true)
           #endif
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)
          #endif
            ),
    apvts(*this, nullptr, "PARAMETERS", createParams())  // Initialize AudioProcessorValueTreeState
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

// DESTRUCTOR
RMWestVoiceAudioProcessor::~RMWestVoiceAudioProcessor() {}

// PREPARE TO PLAY
void RMWestVoiceAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels()) ;
        }
    }
}

// PROCESS BLOCK
void RMWestVoiceAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; i++) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    for (int i = 0; i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            //osc
            //adsr
            //lfo
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release =  *apvts.getRawParameterValue("RELEASE");

            auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
            auto& fmFreq = *apvts.getRawParameterValue("FMFREQ");
            auto& fmDepth = *apvts.getRawParameterValue("FMDEPTH");

            voice->getOscillator().setWaveType(oscWaveChoice);
            voice->getOscillator().setFmParams(fmDepth,fmFreq);
            voice->update(attack.load(), decay.load(), sustain.load(), release.load());
        }
    }
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

/*{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    // Update parameters for each voice
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->updateParameters(apvts);
        }
    }

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Apply master gain
    float volume = apvts.getRawParameterValue("VOLUME")->load();
    buffer.applyGain(volume * 0.1f);
}*/



// RELEASE RESOURCES
void RMWestVoiceAudioProcessor::releaseResources() {}

// GET STATE INFORMATION
void RMWestVoiceAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {}

// SET STATE INFORMATION
void RMWestVoiceAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {}

// CREATE EDITOR
juce::AudioProcessorEditor* RMWestVoiceAudioProcessor::createEditor() { return new RMWestVoiceAudioProcessorEditor(*this); }

// HAS EDITOR
bool RMWestVoiceAudioProcessor::hasEditor() const { return true; }

// GET NAME
const juce::String RMWestVoiceAudioProcessor::getName() const { return JucePlugin_Name; }

// ACCEPT MIDI
bool  RMWestVoiceAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

// PRODUCES MIDI
bool  RMWestVoiceAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

// IS MIDI EFFECT
bool  RMWestVoiceAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

// GET TAIL LENGTH
double RMWestVoiceAudioProcessor::getTailLengthSeconds() const { return 0.0; }

// GET NUM PROGRAMS
int RMWestVoiceAudioProcessor::getNumPrograms() { return 1; }

// GET CURRENT PROGRAM
int RMWestVoiceAudioProcessor::getCurrentProgram() { return 0; }

// SET CURRENT PROGRAM
void RMWestVoiceAudioProcessor::setCurrentProgram(int index) {}

// GET PROGRAM NAME
const juce::String RMWestVoiceAudioProcessor::getProgramName(int index) { return {}; }

// CHANGE PROGRAM NAME
void RMWestVoiceAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

// START INSTANCE
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RMWestVoiceAudioProcessor();
}

// CREATE PARAMETERS
juce::AudioProcessorValueTreeState::ParameterLayout RMWestVoiceAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //OSC
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator", juce::StringArray{ "Triangle", "Saw" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{ "Triangle", "Saw" }, 0));

    //FM
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ", "FM Frequency", 0.0f, 1000.0f, 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH", "FM Depth", 0.0f, 1000.0f, 500.0f));

    //ASDR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.01f, 2000.0f, 0.25f)); // Default 0.25ms
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.01f, 2000.0f, 1000.0f)); // Default 1000ms
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", -5.0f, 5.0f, -1.5f)); // Default -1.5dB
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.01f, 3000.0f, 200.0f)); // Default 200ms
   
    //params.push_back(std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff", 20.0f, 20000.0f, 4800.0f)); // Default 4800hz
    //params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO_RATE", "LFO Rate", 0.1f, 20.0f, 1.0f)); // LFO Rate
  
    //params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", 0.0f, 1.0f, 0.6f)); // Volume
 
    //params.push_back(std::make_unique<juce::AudioParameterFloat>("DETUNE", "Detune", -0.1f, 0.1f, 0.06f)); // Detune, default 6%

    return { params.begin(), params.end() };
}