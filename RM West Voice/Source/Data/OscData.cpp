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
void OscData::prepareLFO(double sampleRate, int samplesPerBlock, int numChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;
    lfo.prepare(spec);
    lfo.setFrequency(5.0f); // 5Hz
}

// SET WAVE TYPE
void OscData::setWaveType(const int choice)
{
    switch (choice)
    {
    case 0:
        //TRIANGLE
        initialise([](float x) { return std::abs(2.0f * (x - std::floor(x + 0.5f))); });
        break;
    case 1:
        //SAW
        initialise([](float x) {return x / juce::MathConstants<float>::pi; });
        break;
    default:
        jassertfalse;
        break;
    }    
}

// SET WAVE FREQUENCY
void OscData::setWaveFrequency(const int midiNoteNumber)
{
    auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    frequency *= (1.0f + detune);
    setFrequency(frequency + fmMod + lfoMod);
    lastMidiNote = midiNoteNumber;
}

// SET DETUNE
void OscData::setDetune(float detuneAmount)
{
    detune = detuneAmount;
}


// GET NEXT AUDIO BLOCK
void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    processFmOsc(block);

    // Applica la modulazione dell'LFO in modo continuo
    for (int s = 0; s < block.getNumSamples(); ++s)
    {
        lfoMod = lfo.processSample(0.0f) * 5.0f;

        auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote);
        currentFreq *= (1.0f + detune);
        setFrequency(currentFreq + fmMod + lfoMod);

        for (int ch = 0; ch < block.getNumChannels(); ++ch)
        {
            block.setSample(ch, s, processSample(block.getSample(ch, s)));
        }
    }
}

// PROCESS FM OSC
void OscData::processFmOsc (juce::dsp::AudioBlock<float>& block)
{
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        for (int s = 0; s < block.getNumSamples(); ++s)
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
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;
    setFrequency (currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}