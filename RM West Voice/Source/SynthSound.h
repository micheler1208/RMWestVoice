/*
  ==============================================================================

    SynthSound.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    // Constructor
    SynthSound() {}

    // Destructor
    ~SynthSound() override {}

    // This sound is always valid to play any note.
    bool appliesToNote (int /*midiNoteNumber*/) override
    {
        return true;
    }

    // This sound is always valid to play on any channel.
    bool appliesToChannel (int /*midiChannel*/) override
    {
        return true;
    }
};
