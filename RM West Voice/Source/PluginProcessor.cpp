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
    apvts(*this, nullptr, "PARAMETERS", createParams()) 
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
    synth.setCurrentPlaybackSampleRate (sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

    highPassFilter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    lowPassFilter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());

}

// PROCESS BLOCK
void RMWestVoiceAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            auto& oscWaveChoice = *apvts.getRawParameterValue ("OSC1WAVETYPE");
            
            auto& fmFreq = *apvts.getRawParameterValue ("OSC1FMFREQ");
            auto& fmDepth = *apvts.getRawParameterValue ("OSC1FMDEPTH");
            
            auto& attack = *apvts.getRawParameterValue ("ATTACK");
            auto& decay = *apvts.getRawParameterValue ("DECAY");
            auto& sustain = *apvts.getRawParameterValue ("SUSTAIN");
            auto& release = *apvts.getRawParameterValue ("RELEASE");
            
            voice->getOscillator().setWaveType (oscWaveChoice);
            //voice->getOscillator().updateFm (fmFreq, fmDepth);
            voice->getOscillator().setDetune(0.06f);
            voice->update (attack.load(), decay.load(), sustain.load(), release.load());
        }
    }
    
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    // LP Filter
    auto& lowPassFilterType = *apvts.getRawParameterValue("LP_FILTERTYPE");
    auto& lowPassCutoff = *apvts.getRawParameterValue("LP_FILTERFREQ");
    auto& lowPassResonance = *apvts.getRawParameterValue("LP_FILTERRES");
    lowPassFilter.updateParameters(lowPassFilterType, lowPassCutoff, lowPassResonance);

    lowPassFilter.process(buffer);

    //HP Filter
    auto& highPassFilterType = *apvts.getRawParameterValue("HP_FILTERTYPE");
    auto& highPassCutoff = *apvts.getRawParameterValue("HP_FILTERFREQ");
    auto& highPassResonance = *apvts.getRawParameterValue("HP_FILTERRES");
    highPassFilter.updateParameters(highPassFilterType, highPassCutoff, highPassResonance);

    highPassFilter.process(buffer);

    // Master Volume
    float volume = apvts.getRawParameterValue("VOLUME")->load();
    buffer.applyGain(volume);
}


// RELEASE RESOURCES
void RMWestVoiceAudioProcessor::releaseResources() {}

// ISBUSESLAYOUTSUPPORTED
#ifndef JucePlugin_PreferredChannelConfigurations
bool RMWestVoiceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


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
    
    // OSC select
    params.push_back (std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray { "Triangle", "Saw"}, 0));
    
    // FM
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC1FMFREQ", "Osc 1 FM Frequency", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.3f }, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC1FMDEPTH", "Osc 1 FM Depth", juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.01f, 0.3f }, 0.0f));
    
    // ADSR
    params.push_back (std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> { 0.1f, 1.0f, }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> { 0.1f, 1.0f, }, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> { 0.1f, 1.0f, }, 0.8f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> { 0.1f, 3.0f, }, 0.2f));

    // Filter LP
    params.push_back (std::make_unique<juce::AudioParameterChoice>("LP_FILTERTYPE", "Low Pass Filter Type", juce::StringArray { "Low-Pass", "Band-Pass", "High-Pass" }, 0));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("LP_FILTERFREQ", "Low Pass Filter Freq", juce::NormalisableRange<float> { 20.0f, 20000.0f, 0.1f, 0.6f }, 4800.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("LP_FILTERRES", "Low PassFilter Resonance", juce::NormalisableRange<float> { 1.0f, 10.0f, 0.1f }, 1.0f));

    // FILTER HP
    params.push_back(std::make_unique<juce::AudioParameterChoice>("HP_FILTERTYPE", "High Pass Filter Type", juce::StringArray{ "Low-Pass", "Band-Pass", "High-Pass" }, 2));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HP_FILTERFREQ", "High Pass Filter Freq", juce::NormalisableRange<float> { 20.0f, 20000.0f, 0.1f, 0.6f }, 250.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HP_FILTERRES", "High Pass Filter Resonance", juce::NormalisableRange<float> { 1.0f, 10.0f, 0.1f }, 1.0f));

    //params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO_RATE", "LFO Rate", 0.1f, 20.0f, 1.0f)); // LFO Rate

    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", 0.0f, 1.0f, 0.6f)); // Volume
 
  
    return { params.begin(), params.end() };
}