/*
  ==============================================================================

    PluginProcessor.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"

namespace
{
RMWestVoice::GlideState::Mode getGlideModeFromParameterValue(float value)
{
    switch (static_cast<int>(value))
    {
        case RMWestVoiceParameters::GlideMode::always:
            return RMWestVoice::GlideState::Mode::always;

        case RMWestVoiceParameters::GlideMode::autoLegato:
            return RMWestVoice::GlideState::Mode::autoLegato;

        case RMWestVoiceParameters::GlideMode::off:
        default:
            return RMWestVoice::GlideState::Mode::off;
    }
}
} // namespace

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
}

// DESTRUCTOR
RMWestVoiceAudioProcessor::~RMWestVoiceAudioProcessor() {}

// PREPARE TO PLAY
void RMWestVoiceAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    monoLeadEngine.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());

    highPassFilter.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());

}

// PROCESS BLOCK
void RMWestVoiceAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    RMWestVoice::MonoLeadEngine::Parameters monoParameters;
    monoParameters.waveType = static_cast<int>(apvts.getRawParameterValue(RMWestVoiceParameters::ID::wave)->load());
    monoParameters.oscMix = apvts.getRawParameterValue(RMWestVoiceParameters::ID::oscMix)->load();
    monoParameters.detuneCents = apvts.getRawParameterValue(RMWestVoiceParameters::ID::detuneCents)->load();
    monoParameters.glideMode = getGlideModeFromParameterValue(apvts.getRawParameterValue(RMWestVoiceParameters::ID::glideMode)->load());
    monoParameters.glideTimeSecondsPerOctave = apvts.getRawParameterValue(RMWestVoiceParameters::ID::glideTime)->load();
    monoParameters.pitchBendRangeSemitones = apvts.getRawParameterValue(RMWestVoiceParameters::ID::bendRange)->load();
    monoParameters.vibratoDepthCents = apvts.getRawParameterValue(RMWestVoiceParameters::ID::vibratoDepth)->load();
    monoParameters.vibratoRateHz = apvts.getRawParameterValue(RMWestVoiceParameters::ID::vibratoRate)->load();
    monoParameters.vibratoFadeSeconds = apvts.getRawParameterValue(RMWestVoiceParameters::ID::vibratoFade)->load();
    monoParameters.vibratoAftertouchAmount = apvts.getRawParameterValue(RMWestVoiceParameters::ID::vibratoAftertouch)->load();
    monoParameters.ampAttack = apvts.getRawParameterValue(RMWestVoiceParameters::ID::ampAttack)->load();
    monoParameters.ampDecay = apvts.getRawParameterValue(RMWestVoiceParameters::ID::ampDecay)->load();
    monoParameters.ampSustain = apvts.getRawParameterValue(RMWestVoiceParameters::ID::ampSustain)->load();
    monoParameters.ampRelease = apvts.getRawParameterValue(RMWestVoiceParameters::ID::ampRelease)->load();
    monoParameters.filterCutoffHz = apvts.getRawParameterValue(RMWestVoiceParameters::ID::filterCutoff)->load();
    monoParameters.filterResonance = apvts.getRawParameterValue(RMWestVoiceParameters::ID::filterResonance)->load();
    monoParameters.filterDrive = apvts.getRawParameterValue(RMWestVoiceParameters::ID::filterDrive)->load();
    monoParameters.filterKeyTracking = apvts.getRawParameterValue(RMWestVoiceParameters::ID::filterKeyTracking)->load();
    monoParameters.filterEnvelopeAmountOctaves = apvts.getRawParameterValue(RMWestVoiceParameters::ID::filterEnvelopeAmount)->load();

    monoLeadEngine.updateParameters(monoParameters);
    monoLeadEngine.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

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
void RMWestVoiceAudioProcessor::releaseResources()
{
    monoLeadEngine.reset();
    highPassFilter.reset();
}

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
