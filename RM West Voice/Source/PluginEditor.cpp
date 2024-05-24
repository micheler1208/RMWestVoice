#include "PluginProcessor.h"
#include "PluginEditor.h"

// CONSTRUCTOR
RMWestVoiceAudioProcessorEditor::RMWestVoiceAudioProcessorEditor (RMWestVoiceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Slider di Attack
    attackSlider.setLookAndFeel(&customLookAndFeelYellow);
    attackSlider.setName("Attack");
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    attackSlider.setRange(0.01, 2000.0, 0.01); // Intervallo in ms
    attackSlider.setTextValueSuffix(" ms");
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30); 
    attackSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    attackSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    attackSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(&attackSlider);
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);

    // Slider di Decay
    decaySlider.setLookAndFeel(&customLookAndFeelYellow);
    decaySlider.setName("Decay");
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setRange(0.0, 100.0, 1.0); // Intervallo in %
    decaySlider.setTextValueSuffix(" %");
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30); 
    decaySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    decaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    decaySlider.setNumDecimalPlacesToDisplay(0); // Imposta il numero di cifre decimali da visualizzare
    addAndMakeVisible(&decaySlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);

    // Slider di Sustain
    sustainSlider.setLookAndFeel(&customLookAndFeelYellow);
    sustainSlider.setName("Sustain");
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    sustainSlider.setRange(0.0, 100.0, 1.0); // Intervallo in %
    sustainSlider.setTextValueSuffix(" %");
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30); 
    sustainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    sustainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    sustainSlider.setNumDecimalPlacesToDisplay(0); // Imposta il numero di cifre decimali da visualizzare
    addAndMakeVisible(&sustainSlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);

    // Slider di Release
    releaseSlider.setLookAndFeel(&customLookAndFeelYellow);
    releaseSlider.setName("Release");
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setRange(1.0, 3000.0, 0.01); // Intervallo in ms
    releaseSlider.setTextValueSuffix(" ms");
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    releaseSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    releaseSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    releaseSlider.setNumDecimalPlacesToDisplay(2); // Imposta il numero di cifre decimali da visualizzare
    addAndMakeVisible(&releaseSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);

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
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUTOFF", cutoffSlider);

    // Slider di Volume
    volumeSlider.setLookAndFeel(&customLookAndFeelViolet);
    volumeSlider.setName("Volume");
    volumeSlider.setSliderStyle(juce::Slider::Rotary);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30); 
    volumeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    volumeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(235, 47, 144));

    addAndMakeVisible(&volumeSlider);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUME", volumeSlider);

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
    const int margin = 25; // Riduce lo spazio tra i rotatory slider
    const int sliderWidth = 75; // Riduce le dimensioni dei rotatory slider
    const int sliderHeight = 75;
    const int labelHeight = 40; // Aumenta l'altezza delle etichette
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

    cutoffSlider.setBounds(getWidth() - 2 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    cutoffLabel.setBounds(getWidth() - 2 * (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);

    volumeSlider.setBounds(getWidth() - (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    volumeLabel.setBounds(getWidth() - (sliderWidth + margin), labelYPosition, sliderWidth, labelHeight);
}