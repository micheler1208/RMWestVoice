/*
  ==============================================================================

    OscData.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "OscData.h"

// PREPARE TO PLAY
void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    prepare(spec);
    fmOsc.prepare (spec);
    glideState.prepare(spec.sampleRate);
}

// PREPARE LFO
void OscData::prepareLFO(double playbackSampleRate, int samplesPerBlock, int numChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.sampleRate = playbackSampleRate;
    spec.numChannels = static_cast<juce::uint32> (numChannels);
    lfo.prepare(spec);
    lfo.setFrequency(5.0f); // 5Hz
}

void OscData::reset()
{
    juce::dsp::Oscillator<float>::reset();
    fmOsc.reset();
    lfo.reset();
    glideState.reset();
    fmMod = 0.0f;
    lfoMod = 0.0f;
    lastMidiNote = 0;
}

// SET WAVE TYPE
void OscData::setWaveType(const int waveType)
{
    if (waveType == 0)
        initialise([](float x) { return std::abs(2.0f * (x - std::floor(x + 0.5f))); });
    else
        initialise([](float x) {return x / juce::MathConstants<float>::pi; });
}

// SET WAVE FREQUENCY
void OscData::setWaveFrequency(const int midiNoteNumber)
{
    setWaveFrequency(midiNoteNumber, false);
}

void OscData::setWaveFrequency(const int midiNoteNumber, bool isLegatoTransition)
{
    const auto noteFrequency = static_cast<float>(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    glideState.startTransition(noteFrequency, isLegatoTransition);
    lastMidiNote = midiNoteNumber;
}

// SET DETUNE
void OscData::setDetuneCents(float cents)
{
    detuneCents = cents;
}

void OscData::setGlideMode(RMWestVoice::GlideState::Mode mode)
{
    glideState.setMode(mode);
}

void OscData::setGlideTimeSecondsPerOctave(float secondsPerOctave)
{
    glideState.setTimeSecondsPerOctave(secondsPerOctave);
}


// GET NEXT AUDIO BLOCK
void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    processFmOsc(block);

    // Applica la modulazione dell'LFO in modo continuo
    const auto numSamples = static_cast<int> (block.getNumSamples());
    const auto numChannels = static_cast<int> (block.getNumChannels());

    for (int s = 0; s < numSamples; ++s)
    {
        lfoMod = lfo.processSample(0.0f) * 5.0f;

        auto currentFreq = applyDetune(glideState.getNextFrequency());
        setFrequency(static_cast<float> (currentFreq) + fmMod + lfoMod);

        for (int ch = 0; ch < numChannels; ++ch)
        {
            block.setSample(ch, s, processSample(block.getSample(ch, s)));
        }
    }
}

// PROCESS FM OSC
void OscData::processFmOsc (juce::dsp::AudioBlock<float>& block)
{
    const auto numChannels = static_cast<int> (block.getNumChannels());
    const auto numSamples = static_cast<int> (block.getNumSamples());

    for (int ch = 0; ch < numChannels; ++ch)
    {
        for (int s = 0; s < numSamples; ++s)
        {
            fmMod = fmOsc.processSample (block.getSample (ch, s)) * fmDepth;
        }
    }
}

// UPDATE FM
void OscData::updateFm (const float freq, const float depth)
{
    fmOsc.setFrequency (freq);
    fmDepth = depth;
    auto currentFreq = static_cast<float> (juce::MidiMessage::getMidiNoteInHertz(lastMidiNote)) + fmMod;
    setFrequency (currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}

float OscData::applyDetune(float sourceFrequency) const
{
    return sourceFrequency * std::pow(2.0f, detuneCents / 1200.0f);
}
