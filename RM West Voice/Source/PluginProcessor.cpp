/*
  ==============================================================================

    PluginProcessor.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"

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
    apvts(*this, nullptr, "PARAMETERS", RMWestVoiceParameters::createParameterLayout())
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
            voice->getOscillator().prepareLFO(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
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
            auto& wave = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::wave);
            auto& detuneCents = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::detuneCents);

            auto& attack = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::ampAttack);
            auto& decay = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::ampDecay);
            auto& sustain = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::ampSustain);
            auto& release = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::ampRelease);
            
            voice->getOscillator().setWaveType(static_cast<int>(wave.load()));
            voice->getOscillator().setDetuneCents(detuneCents.load());
            voice->update (attack.load(), decay.load(), sustain.load(), release.load());
        }
    }
    
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    // LP Filter
    auto& lowPassCutoff = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::filterCutoff);
    auto& lowPassResonance = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::filterResonance);
    lowPassFilter.updateParameters(
        RMWestVoiceParameters::FilterType::lowPass,
        lowPassCutoff.load(),
        lowPassResonance.load());

    lowPassFilter.process(buffer);

    //HP Filter
    auto& highPassCutoff = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::outputHighpassCutoff);
    auto& highPassResonance = *apvts.getRawParameterValue(RMWestVoiceParameters::ID::outputHighpassResonance);
    highPassFilter.updateParameters(
        RMWestVoiceParameters::FilterType::highPass,
        highPassCutoff.load(),
        highPassResonance.load());

    highPassFilter.process(buffer);

    // Master Volume
    float volume = apvts.getRawParameterValue(RMWestVoiceParameters::ID::outputGain)->load();
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
void RMWestVoiceAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    if (xml != nullptr)
        copyXmlToBinary(*xml, destData);
}

// SET STATE INFORMATION
void RMWestVoiceAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr && xmlState->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

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
void RMWestVoiceAudioProcessor::setCurrentProgram(int index) { juce::ignoreUnused (index); }

// GET PROGRAM NAME
const juce::String RMWestVoiceAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused (index);
    return {};
}

// CHANGE PROGRAM NAME
void RMWestVoiceAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

// START INSTANCE
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RMWestVoiceAudioProcessor();
}
