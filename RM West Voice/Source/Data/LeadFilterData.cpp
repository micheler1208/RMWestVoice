#include "LeadFilterData.h"

void LeadFilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.sampleRate = sampleRate;
    spec.numChannels = static_cast<juce::uint32>(outputChannels);

    ladderFilter.prepare(spec);
    ladderFilter.setMode(juce::dsp::LadderFilterMode::LPF24);
    filterEnvelope.setSampleRate(sampleRate);
    modulationState.prepare(sampleRate);

    reset();
    isPrepared = true;
}

void LeadFilterData::reset()
{
    ladderFilter.reset();
    filterEnvelope.reset();
}

void LeadFilterData::updateParameters(const Parameters& newParameters)
{
    modulationState.setBaseCutoffHz(newParameters.cutoffHz);
    modulationState.setKeyTracking(newParameters.keyTracking);
    modulationState.setEnvelopeAmountOctaves(newParameters.envelopeAmountOctaves);

    ladderFilter.setResonance(juce::jlimit(0.0f, 1.0f, newParameters.resonance));
    ladderFilter.setDrive(juce::jmax(1.0f, newParameters.drive));

    envelopeParameters.attack = newParameters.envelopeAttack;
    envelopeParameters.decay = newParameters.envelopeDecay;
    envelopeParameters.sustain = newParameters.envelopeSustain;
    envelopeParameters.release = newParameters.envelopeRelease;
    filterEnvelope.setParameters(envelopeParameters);
}

void LeadFilterData::process(juce::AudioBuffer<float>& buffer)
{
    jassert(isPrepared);

    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const auto envelopeLevel = filterEnvelope.getNextSample();
        ladderFilter.setCutoffFrequencyHz(modulationState.getCutoffHz(envelopeLevel));
        ladderFilter.beginSample();

        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            channelData[sample] = ladderFilter.processSingleSample(channelData[sample], static_cast<std::size_t>(channel));
        }
    }
}

void LeadFilterData::noteStarted(int midiNoteNumber, bool isLegatoTransition)
{
    modulationState.setCurrentMidiNote(midiNoteNumber);

    if (! isLegatoTransition || ! filterEnvelope.isActive())
        filterEnvelope.noteOn();
}

void LeadFilterData::noteStopped()
{
    filterEnvelope.noteOff();
}
