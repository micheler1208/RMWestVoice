/*
  ==============================================================================

    AdsrData.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AdsrData : public juce::ADSR
{
public:
    void updateADSR(const float attack, const float decay, const float sustain, const float release);
private:
    juce::ADSR::Parameters adsrParams;
};