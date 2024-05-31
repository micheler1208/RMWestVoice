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
    : AudioProcessorEditor (&p), audioProcessor (p), osc(audioProcessor.apvts, "OSC1WAVETYPE"), adsr(audioProcessor.apvts)
{
    // WINDOWS SIZE
    setSize(800, 600);

    // OSC ENABLE
    addAndMakeVisible(osc);

    // ADSR ENABLE
    addAndMakeVisible(adsr);

    // Slider di Cutoff
    cutoffSlider.setLookAndFeel(&customLookAndFeelViolet);
    cutoffSlider.setName("Cutoff");
    cutoffSlider.setSliderStyle(juce::Slider::Rotary);
    cutoffSlider.setRange(20.0, 20000.0, 1.0);
    cutoffSlider.setTextValueSuffix(" Hz");
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    cutoffSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    cutoffSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(235, 47, 144));
    cutoffSlider.setNumDecimalPlacesToDisplay(0); // Imposta il numero di cifre decimali da visualizzare
    addAndMakeVisible(&cutoffSlider);
    //cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", cutoffSlider);

    // Slider di Volume
    volumeSlider.setLookAndFeel(&customLookAndFeelViolet);
    volumeSlider.setName("Volume");
    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30); 
    volumeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    volumeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(235, 47, 144));
    addAndMakeVisible(&volumeSlider);
    //volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUME", volumeSlider);

    // Etichette
    juce::Font labelFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::timegoing_ttf, BinaryData::timegoing_ttfSize));
    labelFont.setHeight(48.0f); // Aumenta la dimensione del font

    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setFont(labelFont);
    attackLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    attackLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&attackLabel);

    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setFont(labelFont);
    decayLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    decayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&decayLabel);

    sustainLabel.setText("Sustain", juce::dontSendNotification);
    sustainLabel.setFont(labelFont);
    sustainLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    sustainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&sustainLabel);

    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setFont(labelFont);
    releaseLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    releaseLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&releaseLabel);

    cutoffLabel.setText("Cutoff", juce::dontSendNotification);
    cutoffLabel.setFont(labelFont);
    cutoffLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(235, 47, 144));
    cutoffLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&cutoffLabel);

    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setFont(labelFont);
    volumeLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(235, 47, 144));
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&volumeLabel);

}

// DESTRUCTOR
RMWestVoiceAudioProcessorEditor::~RMWestVoiceAudioProcessorEditor()
{
    cutoffSlider.setLookAndFeel(nullptr);
    volumeSlider.setLookAndFeel(nullptr);
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

    osc.setBounds(initialX + 4 * (111), yPosition + 13 , selectorWidth, selectorHeight);

    adsr.setBounds(initialX, yPosition, getWidth() / 2, getHeight());


    attackLabel.setBounds(initialX, labelYPosition, sliderWidth, labelHeight);
    decayLabel.setBounds(initialX + sliderWidth + margin, labelYPosition, sliderWidth, labelHeight);
    sustainLabel.setBounds(initialX + 2 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);
    releaseLabel.setBounds(initialX + 3 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);

    cutoffSlider.setBounds(getWidth() - 2 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    cutoffLabel.setBounds(getWidth() - 2 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);

    volumeSlider.setBounds(getWidth() - (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    volumeLabel.setBounds(getWidth() - (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);
}
