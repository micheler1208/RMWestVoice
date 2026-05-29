/*
  ==============================================================================

    OscData.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "OscData.h"

namespace
{
constexpr auto pi = juce::MathConstants<float>::pi;
constexpr auto twoPi = juce::MathConstants<float>::twoPi;
constexpr auto defaultWaveType = 2;
}

// PREPARE TO PLAY
void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate > 0.0 ? spec.sampleRate : 44100.0;

    prepare(spec);
    secondaryOsc.prepare(spec);
    hybridCarrierOsc.prepare(spec);
    hybridShimmerOsc.prepare(spec);
    glideState.prepare(spec.sampleRate);
    pitchModulation.prepare(spec.sampleRate);
    hybridCarrierOsc.initialise(hybridCarrierWave);
    hybridShimmerOsc.initialise(hybridShimmerWave);
    setWaveType(defaultWaveType);
}

void OscData::reset()
{
    juce::dsp::Oscillator<float>::reset();
    secondaryOsc.reset();
    hybridCarrierOsc.reset();
    hybridShimmerOsc.reset();
    glideState.reset();
    pitchModulation.reset();
    characterDriftPhaseA = 0.0f;
    characterDriftPhaseB = 0.0f;
    lastMidiNote = 0;
}

// SET WAVE TYPE
void OscData::setWaveType(const int waveType)
{
    if (waveType == currentWaveType)
        return;

    currentWaveType = waveType;

    switch (waveType)
    {
        case 1:
            initialise(triangleWave);
            secondaryOsc.initialise(triangleWave);
            break;

        case 2:
            initialise(sawWave);
            secondaryOsc.initialise(triangleWave);
            break;

        case 3:
            initialise(sawWave);
            secondaryOsc.initialise(pulseWave);
            break;

        case 0:
        default:
            initialise(sawWave);
            secondaryOsc.initialise(sawWave);
            break;
    }
}

void OscData::setCharacterMode(int characterMode)
{
    characterSettings = RMWestVoice::CharacterState::getSettingsForMode(
        RMWestVoice::CharacterState::modeFromIndex(characterMode));
}

void OscData::setOscMix(float mix)
{
    oscMix = juce::jlimit(0.0f, 1.0f, mix);
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

void OscData::setPitchBendRangeSemitones(float semitones)
{
    pitchModulation.setPitchBendRangeSemitones(semitones);
}

void OscData::setPitchWheel(float normalizedBipolar)
{
    pitchModulation.setPitchWheel(normalizedBipolar);
}

void OscData::setModWheel(float normalized)
{
    pitchModulation.setModWheel(normalized);
}

void OscData::setAftertouch(float normalized)
{
    pitchModulation.setAftertouch(normalized);
}

void OscData::setVibratoDepthCents(float cents)
{
    pitchModulation.setVibratoDepthCents(cents);
}

void OscData::setVibratoRateHz(float hz)
{
    pitchModulation.setVibratoRateHz(hz);
}

void OscData::setVibratoFadeSeconds(float seconds)
{
    pitchModulation.setVibratoFadeSeconds(seconds);
}

void OscData::setVibratoAftertouchAmount(float normalized)
{
    pitchModulation.setVibratoAftertouchAmount(normalized);
}

void OscData::noteStarted(bool isLegatoTransition)
{
    pitchModulation.noteStarted(isLegatoTransition);
}

void OscData::noteStopped()
{
    pitchModulation.noteStopped();
}

// GET NEXT AUDIO BLOCK
void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    const auto numSamples = static_cast<int> (block.getNumSamples());
    const auto numChannels = static_cast<int> (block.getNumChannels());

    for (int s = 0; s < numSamples; ++s)
    {
        const auto currentFreq = glideState.getNextFrequency() * pitchModulation.getNextPitchRatio();
        const auto driftRatio = getCharacterDriftRatio();
        const auto primaryFrequency = currentFreq * driftRatio;
        const auto secondaryFrequency = applyDetune(currentFreq / driftRatio, detuneCents);

        setFrequency(primaryFrequency);
        secondaryOsc.setFrequency(secondaryFrequency);
        hybridCarrierOsc.setFrequency(primaryFrequency);
        hybridShimmerOsc.setFrequency(currentFreq * 2.01f);

        const auto primarySample = processSample(0.0f);
        const auto secondarySample = secondaryOsc.processSample(0.0f);
        const auto hybridSample = (hybridCarrierOsc.processSample(0.0f) * 0.72f)
            + (hybridShimmerOsc.processSample(0.0f) * 0.28f);
        const auto oscillatorSample = primarySample * (1.0f - oscMix) + secondarySample * oscMix;
        const auto hybridBlend = characterSettings.hybridBlend;
        const auto sourceSample = oscillatorSample * (1.0f - hybridBlend) + hybridSample * hybridBlend;
        const auto outputSample = applyCharacterDrive(sourceSample, characterSettings);

        advanceCharacterDrift();

        for (int ch = 0; ch < numChannels; ++ch)
        {
            block.setSample(ch, s, outputSample);
        }
    }
}

float OscData::sawWave(float phase) noexcept
{
    return phase / pi;
}

float OscData::triangleWave(float phase) noexcept
{
    return (2.0f / pi) * std::asin(std::sin(phase));
}

float OscData::pulseWave(float phase) noexcept
{
    return std::sin(phase) >= 0.0f ? 1.0f : -1.0f;
}

float OscData::hybridCarrierWave(float phase) noexcept
{
    return std::sin(phase + (0.28f * std::sin(phase * 2.0f)));
}

float OscData::hybridShimmerWave(float phase) noexcept
{
    return (std::sin(phase) * 0.70f) + (std::sin(phase * 2.0f) * 0.30f);
}

float OscData::applyDetune(float sourceFrequency, float cents) noexcept
{
    return sourceFrequency * std::pow(2.0f, cents / 1200.0f);
}

float OscData::applyCharacterDrive(float sample, const RMWestVoice::CharacterSettings& settings) noexcept
{
    const auto drive = juce::jmax(1.0f, settings.sourceDrive);
    const auto normalization = 1.0f / std::tanh(drive);

    return std::tanh(sample * drive) * normalization * settings.sourceTrim;
}

float OscData::getCharacterDriftRatio() noexcept
{
    const auto driftCents = characterSettings.driftDepthCents
        * ((0.65f * std::sin(characterDriftPhaseA)) + (0.35f * std::sin(characterDriftPhaseB)));

    return std::pow(2.0f, driftCents / 1200.0f);
}

void OscData::advanceCharacterDrift() noexcept
{
    characterDriftPhaseA += static_cast<float>((twoPi * 0.17) / sampleRate);
    characterDriftPhaseB += static_cast<float>((twoPi * 0.097) / sampleRate);

    if (characterDriftPhaseA >= twoPi)
        characterDriftPhaseA -= twoPi;

    if (characterDriftPhaseB >= twoPi)
        characterDriftPhaseB -= twoPi;
}
