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
                     ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    oscillator.setWaveform(Oscillator::Triangle); // Start with Triangle waveform
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass); // Default to lowpass filter
}

// DESTRUCTOR
RMWestVoiceAudioProcessor::~RMWestVoiceAudioProcessor()
{
}

// SLIDERS TREE STATE
juce::AudioProcessorValueTreeState::ParameterLayout RMWestVoiceAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.01f, 2000.0f, 0.02f)); // Default 5ms
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 100.0f, 10.0f)); // Default 10%
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 100.0f, 15.0f)); // Default 15%
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 1.0f, 3000.0f, 400.0f)); // Default 300ms
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff", 20.0f, 20000.0f, 2000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO_RATE", "LFO Rate", 0.1f, 20.0f, 1.0f)); // LFO Rate
    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", 0.0f, 1.0f, 0.8f)); // Volume

    return {params.begin(), params.end()};
}


// GET NAME OF APPLICATION
const juce::String RMWestVoiceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

// MIDI PROPERTIES
bool RMWestVoiceAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RMWestVoiceAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RMWestVoiceAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RMWestVoiceAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

// PROGRAM PROPERTIES
int RMWestVoiceAudioProcessor::getNumPrograms()
{
    return 1;   
}

int RMWestVoiceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RMWestVoiceAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RMWestVoiceAudioProcessor::getProgramName (int index)
{
    return {};
}

void RMWestVoiceAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

// INIZIALIZATION PROPERTIES
void RMWestVoiceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    oscillator.prepare(spec);
    adsr.setSampleRate(sampleRate);
    filter.prepare(spec);
}

// PROCESS BLOCK
void RMWestVoiceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Processa gli eventi MIDI
    for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            adsr.noteOn();
            oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz(msg.getNoteNumber()));
        }
        else if (msg.isNoteOff())
        {
            adsr.noteOff();
        }
    }

    juce::dsp::AudioBlock<float> block(buffer);
    oscillator.process(block);

    // Aggiorna i parametri dell'ADSR con i valori attuali
    adsrParams.attack = apvts.getRawParameterValue("ATTACK")->load()/ 1000.0f; // Converti ms in secondi
    adsrParams.decay = apvts.getRawParameterValue("DECAY")->load()/ 100.0f; // Converti percentuale
    adsrParams.sustain = apvts.getRawParameterValue("SUSTAIN")->load()/ 100.0f; // Converti percentuale
    adsrParams.release = apvts.getRawParameterValue("RELEASE")->load()/ 1000.0f; // Converti ms in secondi
    adsr.setParameters(adsrParams);

    adsr.applyEnvelopeToBuffer(buffer, 0, buffer.getNumSamples());

    // Modula la frequenza di cutoff del filtro con il LFO
    float lfoRate = apvts.getRawParameterValue("LFO_RATE")->load();
    oscillator.setLfoFrequency(lfoRate); // Imposta la frequenza del LFO

    // Ottieni il valore dell'LFO
    juce::AudioBuffer<float> lfoBuffer(1, buffer.getNumSamples());
    lfoBuffer.clear();
    juce::dsp::AudioBlock<float> lfoBlock(lfoBuffer);
    oscillator.processLFO(lfoBlock);
    float lfoValue = lfoBlock.getSample(0, 0) * 1000.0f; // Scala il valore del LFO

    // Applica il filtro al buffer
    float cutoff = apvts.getRawParameterValue("CUTOFF")->load() + lfoValue;

    // Clamping della frequenza di cutoff per evitare valori non validi
    cutoff = std::fmax(20.0f, std::fmin(cutoff, getSampleRate() * 0.5f - 1.0f));
    filter.setCutoffFrequency(cutoff);

    juce::dsp::ProcessContextReplacing<float> context(block);
    filter.process(context);

    // Applica il controllo del volume
    float volume = apvts.getRawParameterValue("VOLUME")->load();
    buffer.applyGain(volume);
}

// STOP PROPERTIES
void RMWestVoiceAudioProcessor::releaseResources()
{
}

// OUTPUT PROPERTIES
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

// EDITOR PROPERTIES
bool RMWestVoiceAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RMWestVoiceAudioProcessor::createEditor()
{
    return new RMWestVoiceAudioProcessorEditor (*this);
}

// STATE PROPERTIES
void RMWestVoiceAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void RMWestVoiceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

// START INSTANCE
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RMWestVoiceAudioProcessor();
}
