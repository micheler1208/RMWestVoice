/*
  ==============================================================================

    OscData.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "OscData.h"

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

