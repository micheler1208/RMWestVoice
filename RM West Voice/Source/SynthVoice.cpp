/*
  ==============================================================================

    SynthVoice.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "SynthVoice.h"

// CAN PLAY SOUND
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    osc.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
}

// STOP NOTE
void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
    {
        clearCurrentNote();
    }
}

// CONTROLLER MOVED
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

// PITCH WHEEL MOVED
void SynthVoice::pitchWheelMoved(int newPitchWheelValue) 
{
}

// PREPARE TO PLAY
void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc.prepareToPlay(spec);
    gain.prepare(spec);

    gain.setGainLinear(0.3f);

    isPrepared = true;

}

// RENDER NEXT BLOCK
void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared);
    
    if (! isVoiceActive())
        return;
    
    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    osc.getNextAudioBlock (audioBlock);
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    adsr.applyEnvelopeToBuffer (synthBuffer, 0, synthBuffer.getNumSamples());
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom (channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if (! adsr.isActive())
            clearCurrentNote();
    }
}

// UPDATE ADSR VALUES
void SynthVoice::update(const float attack, const float decay, const float sustain, const float release)
{
    adsr.updateADSR(attack, decay, sustain, release);
}