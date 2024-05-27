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
                     ), apvts(*this, nullptr, "Parameters", createParameters()), currentMidiNoteNumber(-1)
#endif
{
    oscillator.setWaveform(Oscillator::Saw); // Start with Sawtooth
    filter.setMode(juce::dsp::LadderFilterMode::LPF12); // Set filter to LPF 12
    filter.setDrive(1.2f); // Set drive to 20% (1.0 is no drive, 1.2 is 20% drive)
    oscillator.enableGlide(true); // Enable glide by default
}

// DESTRUCTOR
RMWestVoiceAudioProcessor::~RMWestVoiceAudioProcessor()
{
}

// SLIDERS TREE STATE
juce::AudioProcessorValueTreeState::ParameterLayout RMWestVoiceAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.01f, 2000.0f, 0.25f)); // Default 0.25ms
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.01f, 2000.0f, 1000.0f)); // Default 1000ms
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", -5.0f, 5.0f, -1.5f)); // Default -1.5dB
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.01f, 3000.0f, 200.0f)); // Default 200ms
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CUTOFF", "Cutoff", 20.0f, 20000.0f, 4800.0f)); // Default 4800hz
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO_RATE", "LFO Rate", 0.1f, 20.0f, 1.0f)); // LFO Rate
    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", 0.0f, 1.0f, 0.6f)); // Volume
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DETUNE", "Detune", -0.1f, 0.1f, 0.06f)); // Detune, default 6%

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
    lfo.prepare(spec);

    lfo.setFrequency(apvts.getRawParameterValue("LFO_RATE")->load(), sampleRate); // Imposta la frequenza dell'LFO
}

// PROCESS BLOCK
void RMWestVoiceAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
            currentMidiNoteNumber = msg.getNoteNumber();
            adsr.noteOn();
            oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz(currentMidiNoteNumber));
        }
        else if (msg.isNoteOff())
        {
            if (currentMidiNoteNumber == msg.getNoteNumber())
            {
                adsr.noteOff();
                currentMidiNoteNumber = -1;
            }
        }
    }

    juce::dsp::AudioBlock<float> block(buffer);
    oscillator.process(block);

    // Aggiorna i parametri dell'ADSR con i valori attuali
    adsrParams.attack = apvts.getRawParameterValue("ATTACK")->load() / 1000.0f; // Converti ms in secondi
    adsrParams.decay = apvts.getRawParameterValue("DECAY")->load() / 1000.0f; // Converti ms in secondi
    adsrParams.sustain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("SUSTAIN")->load()); // Converti dB in guadagno lineare
    adsrParams.release = apvts.getRawParameterValue("RELEASE")->load() / 1000.0f; // Converti ms in secondi
    adsr.setParameters(adsrParams);

    adsr.applyEnvelopeToBuffer(buffer, 0, buffer.getNumSamples());

    // Modula la frequenza di cutoff del filtro con il LFO
    float lfoRate = apvts.getRawParameterValue("LFO_RATE")->load();
    lfo.setFrequency(lfoRate, getSampleRate()); // Aggiorna la frequenza dell'LFO

    // Applica il valore di detune
    float detuneValue = apvts.getRawParameterValue("DETUNE")->load();
    oscillator.setDetune(detuneValue);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float lfoValue = lfo.getNextSample() * 500.0f; // Scala il valore del LFO

        // Applica il filtro al buffer
        float cutoff = apvts.getRawParameterValue("CUTOFF")->load() + lfoValue;

        // Clamping della frequenza di cutoff per evitare valori non validi
        cutoff = std::fmax(20.0f, std::fmin(cutoff, getSampleRate() * 0.5f - 1.0f));
        filter.setCutoffFrequencyHz(cutoff);

        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            buffer.setSample(channel, sample, buffer.getSample(channel, sample));
        }
    }

    juce::dsp::ProcessContextReplacing<float> context(block);
    filter.process(context);

    // Applica il controllo del volume
    float volume = apvts.getRawParameterValue("VOLUME")->load();
    buffer.applyGain(volume * 0.1f);
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
