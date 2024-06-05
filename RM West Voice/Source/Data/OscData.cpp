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
    setFrequency(frequency + fmMod);
    lastMidiNote = midiNoteNumber;
}

// SET DETUNE
void OscData::setDetune(float detuneAmount)
{
    detune = detuneAmount;
}


// GET NEXT AUDIO BLOCK
void OscData::getNextAudioBlock (juce::dsp::AudioBlock<float>& block)
{
    processFmOsc (block);
    process (juce::dsp::ProcessContextReplacing<float> (block));
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