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

// SET WAVE TYPE
void OscData::setWaveType(const bool dayValue)
{
    if(dayValue)
        initialise([](float x) { return std::abs(2.0f * (x - std::floor(x + 0.5f))); });
    else
        initialise([](float x) {return x / juce::MathConstants<float>::pi; });
}

// SET WAVE FREQUENCY
void OscData::setWaveFrequency(const int midiNoteNumber)
{
    auto noteFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    noteFrequency *= (1.0f + detune);
    setFrequency(static_cast<float> (noteFrequency) + fmMod + lfoMod);
    lastMidiNote = midiNoteNumber;
}

// SET DETUNE
void OscData::setDetune(bool isDetuned)
{
    detuneActive = isDetuned;
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

        auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote);
        if (detuneActive)
            currentFreq *= (1.0f - detune);
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
