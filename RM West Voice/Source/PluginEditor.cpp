/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


// CONSTRUCTORE
RMWestVoiceAudioProcessorEditor::RMWestVoiceAudioProcessorEditor (RMWestVoiceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Slider di Attack
    attackSlider.setLookAndFeel(&customLookAndFeel);
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    attackSlider.setRange(1.0, 3000.0, 1.0); // Intervallo in ms
    attackSlider.setTextValueSuffix(" ms");
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    attackSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    attackSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&attackSlider);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);


    // Slider di Decay
    decaySlider.setLookAndFeel(&customLookAndFeel);
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setRange(0.0, 100.0, 1.0); // Intervallo in %
    decaySlider.setTextValueSuffix(" %");
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    decaySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    decaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&decaySlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);

    // Slider di Sustain
    sustainSlider.setLookAndFeel(&customLookAndFeel);
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    sustainSlider.setRange(0.0, 100.0, 1.0); // Intervallo in %
    sustainSlider.setTextValueSuffix(" %");
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    sustainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    sustainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&sustainSlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);

    // Slider di Release
    releaseSlider.setLookAndFeel(&customLookAndFeel);
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setRange(1.0, 5000.0, 1.0); // Intervallo in ms
    releaseSlider.setTextValueSuffix(" ms");
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    releaseSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    releaseSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&releaseSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);

    // Slider di Cutoff
    cutoffSlider.setLookAndFeel(&customLookAndFeel);
    cutoffSlider.setSliderStyle(juce::Slider::Rotary);
    cutoffSlider.setRange(20.0, 20000.0, 1.0);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    cutoffSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    cutoffSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&cutoffSlider);
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", cutoffSlider);

    
    // Slider di Volume
    volumeSlider.setLookAndFeel(&customLookAndFeel);
    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    volumeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    volumeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    addAndMakeVisible(&volumeSlider);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUME", volumeSlider);

    
    // Etichette
    juce::Font labelFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::timegoing_ttf, BinaryData::timegoing_ttfSize));
    labelFont.setHeight(31.0f);
    
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
    cutoffLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    cutoffLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&cutoffLabel);

    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setFont(labelFont);
    volumeLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(255, 228, 163));
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(&volumeLabel);
    
    setSize(800, 600);
}



// DESTRUCTOR
RMWestVoiceAudioProcessorEditor::~RMWestVoiceAudioProcessorEditor()
{
    attackSlider.setLookAndFeel(nullptr);
    decaySlider.setLookAndFeel(nullptr);
    sustainSlider.setLookAndFeel(nullptr);
    releaseSlider.setLookAndFeel(nullptr);
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
    const int margin = 45;
    const int sliderWidth = 50;
    const int sliderHeight = 80;
    const int labelHeight = 20;
    const int initialX = 25;
    const int yPosition = getHeight() - sliderHeight - 12; 
    const int labelYPosition = yPosition - labelHeight;

    attackSlider.setBounds(initialX, yPosition, sliderWidth, sliderHeight);
    attackLabel.setBounds(initialX, labelYPosition, sliderWidth, labelHeight);

    decaySlider.setBounds(initialX + sliderWidth + margin, yPosition, sliderWidth, sliderHeight);
    decayLabel.setBounds(initialX + sliderWidth + margin, labelYPosition, sliderWidth, labelHeight);

    sustainSlider.setBounds(initialX + 2 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    sustainLabel.setBounds(initialX + 2 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);

    releaseSlider.setBounds(initialX + 3 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    releaseLabel.setBounds(initialX + 3 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);

    cutoffSlider.setBounds(initialX + 4 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    cutoffLabel.setBounds(initialX + 4 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);
    
    volumeSlider.setBounds(initialX + 5 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    volumeLabel.setBounds(initialX + 5 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);

}