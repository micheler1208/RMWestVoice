/*
  ==============================================================================

    MonoLeadEngine.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "MonoLeadEngine.h"

namespace
{
constexpr auto modWheelControllerNumber = 1;
constexpr auto pitchWheelCentre = 8192;
constexpr auto pitchWheelMinimum = 0;
constexpr auto pitchWheelMaximum = 16383;

float normalize7BitValue(int value) noexcept
{
    return juce::jlimit(0.0f, 1.0f, static_cast<float>(value) / 127.0f);
}

float normalizePitchWheelValue(int value) noexcept
{
    const auto clampedValue = juce::jlimit(pitchWheelMinimum, pitchWheelMaximum, value);

    if (clampedValue >= pitchWheelCentre)
        return static_cast<float>(clampedValue - pitchWheelCentre) / static_cast<float>(pitchWheelMaximum - pitchWheelCentre);

    return static_cast<float>(clampedValue - pitchWheelCentre) / static_cast<float>(pitchWheelCentre - pitchWheelMinimum);
}
} // namespace

namespace RMWestVoice
{
void MonoLeadEngine::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.sampleRate = sampleRate;
    spec.numChannels = static_cast<juce::uint32>(outputChannels);

    osc.prepareToPlay(spec);

    gain.prepare(spec);
    gain.setGainLinear(0.3f);

    synthBuffer.setSize(outputChannels, samplesPerBlock);
    reset();

    isPrepared = true;
}

void MonoLeadEngine::reset()
{
    noteStack.clear();
    adsr.reset();
    osc.reset();
    gain.reset();
    synthBuffer.clear();
}

void MonoLeadEngine::updateParameters(const Parameters& newParameters)
{
    parameters = newParameters;

    osc.setWaveType(parameters.waveType);
    osc.setOscMix(parameters.oscMix);
    osc.setDetuneCents(parameters.detuneCents);
    osc.setGlideMode(parameters.glideMode);
    osc.setGlideTimeSecondsPerOctave(parameters.glideTimeSecondsPerOctave);
    osc.setPitchBendRangeSemitones(parameters.pitchBendRangeSemitones);
    osc.setVibratoDepthCents(parameters.vibratoDepthCents);
    osc.setVibratoRateHz(parameters.vibratoRateHz);
    osc.setVibratoFadeSeconds(parameters.vibratoFadeSeconds);
    osc.setVibratoAftertouchAmount(parameters.vibratoAftertouchAmount);
    adsr.updateADSR(
        parameters.ampAttack,
        parameters.ampDecay,
        parameters.ampSustain,
        parameters.ampRelease);
}

void MonoLeadEngine::renderNextBlock(
    juce::AudioBuffer<float>& outputBuffer,
    const juce::MidiBuffer& midiMessages,
    int startSample,
    int numSamples)
{
    jassert(isPrepared);

    if (numSamples <= 0)
        return;

    auto renderedSamples = 0;

    for (const auto metadata : midiMessages)
    {
        const auto eventSample = juce::jlimit(0, numSamples, metadata.samplePosition);

        if (eventSample > renderedSamples)
        {
            renderAudioRange(outputBuffer, startSample + renderedSamples, eventSample - renderedSamples);
            renderedSamples = eventSample;
        }

        handleMidiMessage(metadata.getMessage());
    }

    if (renderedSamples < numSamples)
        renderAudioRange(outputBuffer, startSample + renderedSamples, numSamples - renderedSamples);
}

void MonoLeadEngine::renderAudioRange(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (numSamples <= 0 || (! noteStack.hasActiveNote() && ! adsr.isActive()))
        return;

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    osc.getNextAudioBlock(audioBlock);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
}

void MonoLeadEngine::handleMidiMessage(const juce::MidiMessage& message)
{
    if (message.isPitchWheel())
    {
        handlePitchWheel(message);
        return;
    }

    if (message.isController() && message.getControllerNumber() == modWheelControllerNumber)
    {
        handleModWheel(message);
        return;
    }

    if (message.isChannelPressure() || message.isAftertouch())
    {
        handleAftertouch(message);
        return;
    }

    if (message.isNoteOn())
    {
        startActiveNote(noteStack.noteOn(message.getNoteNumber()));
        return;
    }

    if (message.isNoteOff())
    {
        stopOrFallbackFromActiveNote(noteStack.noteOff(message.getNoteNumber()));
        return;
    }

    if (message.isAllNotesOff() || message.isAllSoundOff())
    {
        noteStack.clear();
        adsr.noteOff();
        osc.noteStopped();
    }
}

void MonoLeadEngine::handlePitchWheel(const juce::MidiMessage& message)
{
    osc.setPitchWheel(normalizePitchWheelValue(message.getPitchWheelValue()));
}

void MonoLeadEngine::handleModWheel(const juce::MidiMessage& message)
{
    osc.setModWheel(normalize7BitValue(message.getControllerValue()));
}

void MonoLeadEngine::handleAftertouch(const juce::MidiMessage& message)
{
    if (message.isChannelPressure())
    {
        osc.setAftertouch(normalize7BitValue(message.getChannelPressureValue()));
        return;
    }

    if (noteStack.hasActiveNote() && message.getNoteNumber() == noteStack.getActiveNote())
        osc.setAftertouch(normalize7BitValue(message.getAfterTouchValue()));
}

void MonoLeadEngine::startActiveNote(const MonoNoteStack::UpdateResult& noteUpdate)
{
    if (! noteUpdate.hasActiveNote)
        return;

    osc.setWaveFrequency(noteUpdate.activeNote, noteUpdate.isLegatoNoteOn);
    osc.noteStarted(noteUpdate.isLegatoNoteOn);

    if (! noteUpdate.isLegatoNoteOn || ! adsr.isActive())
        adsr.noteOn();
}

void MonoLeadEngine::stopOrFallbackFromActiveNote(const MonoNoteStack::UpdateResult& noteUpdate)
{
    if (noteUpdate.hasActiveNote)
    {
        if (noteUpdate.activeNoteChanged)
            osc.setWaveFrequency(noteUpdate.activeNote, noteUpdate.hadActiveNote);

        return;
    }

    if (noteUpdate.hadActiveNote)
    {
        adsr.noteOff();
        osc.noteStopped();
    }
}
} // namespace RMWestVoice
