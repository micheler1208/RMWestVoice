/*
  ==============================================================================

    PluginEditor.cpp
    Author:  micheler1208

  ==============================================================================
*/


#include "PluginProcessor.h"
#include "PluginEditor.h"

// CONSTRUCTOR
RMWestVoiceAudioProcessorEditor::RMWestVoiceAudioProcessorEditor (RMWestVoiceAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), 
    osc(audioProcessor.apvts), 
    adsr(audioProcessor.apvts), 
    filters(audioProcessor.apvts)
{
    // WINDOWS SIZE
    setSize(800, 600);

    // OSC ENABLE
    addAndMakeVisible(osc);

    // ADSR ENABLE
    addAndMakeVisible(adsr);

    // FILTERS ENABLE
    addAndMakeVisible(filters);

}

// DESTRUCTOR
RMWestVoiceAudioProcessorEditor::~RMWestVoiceAudioProcessorEditor()
{
}

// GUI - PAINT
void RMWestVoiceAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Background
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize);
    if (!backgroundImage.isNull())
    {
        g.drawImageAt(backgroundImage, 0, 0);
    }
    else
    {
        // Fallback color
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    // Title Text
    juce::Font titleFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::wholecar_ttf, BinaryData::wholecar_ttfSize));
    titleFont.setHeight(72.0f);
    g.setFont(titleFont);
    g.setColour(juce::Colour::fromRGB(255, 228, 163));
    g.drawFittedText("{_RM-WEST#VOICE_}", getLocalBounds().removeFromTop(35), juce::Justification::centred, 1);
}

// GUI - RESIZE - POSITIONING
void RMWestVoiceAudioProcessorEditor::resized()
{
    const int margin = 25;
    const int sliderWidth = 75;
    const int sliderHeight = 75;
    const int labelHeight = 40; 
    const int selectorWidth = 100;
    const int selectorHeight = 30;
    const int initialX = 25;
    const int yPosition = getHeight() - sliderHeight - 12;
    const int labelYPosition = yPosition - labelHeight;

    adsr.setBounds(initialX, yPosition - 40, getWidth() / 2, getHeight());

    osc.setBounds(getWidth() / 2 +33, getHeight() -100, 130, getHeight());

    filters.setBounds(getWidth()/2 +185, 0, getWidth() / 2, getHeight());
}
