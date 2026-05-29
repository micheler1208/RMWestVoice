/*
  ==============================================================================

    PluginEditor.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "PluginEditor.h"

#include "PluginParameters.h"

#include <BinaryData.h>

namespace
{
enum class SliderDisplay
{
    plain,
    percent,
    seconds,
    hertz,
    cents,
    semitones,
    octaves,
    multiplier,
    q
};

juce::String formatSliderValue(double value, SliderDisplay display)
{
    switch (display)
    {
        case SliderDisplay::percent:
            return juce::String(value * 100.0, 0) + "%";

        case SliderDisplay::seconds:
            return juce::String(value, value < 1.0 ? 2 : 1) + " s";

        case SliderDisplay::hertz:
            return value >= 1000.0
                ? juce::String(value / 1000.0, 2) + " kHz"
                : juce::String(value, 0) + " Hz";

        case SliderDisplay::cents:
            return juce::String(value, 1) + " ct";

        case SliderDisplay::semitones:
            return juce::String(value, 0) + " st";

        case SliderDisplay::octaves:
            return juce::String(value, 2) + " oct";

        case SliderDisplay::multiplier:
            return juce::String(value, 2) + "x";

        case SliderDisplay::q:
            return juce::String(value, 1) + " Q";

        case SliderDisplay::plain:
        default:
            return juce::String(value, 2);
    }
}

double parseSliderValue(const juce::String& text, SliderDisplay display)
{
    auto value = text.getDoubleValue();

    if (display == SliderDisplay::percent)
        value /= 100.0;

    if (display == SliderDisplay::hertz && text.containsIgnoreCase("k"))
        value *= 1000.0;

    return value;
}

juce::Font getUiFont(float height)
{
    auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::caviar_ttf, BinaryData::caviar_ttfSize);
    return juce::Font { juce::FontOptions(typeface).withHeight(height) };
}

class SliderControl final : public juce::Component
{
public:
    SliderControl(
        juce::AudioProcessorValueTreeState& apvts,
        const juce::String& parameterID,
        const juce::String& labelText,
        SliderDisplay display,
        juce::Colour accentColour)
    {
        label.setText(labelText, juce::dontSendNotification);
        label.setFont(getUiFont(14.0f));
        label.setColour(juce::Label::textColourId, juce::Colour::fromRGB(232, 235, 241));
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);

        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 74, 18);
        slider.setColour(juce::Slider::rotarySliderFillColourId, accentColour);
        slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour::fromRGB(35, 39, 54));
        slider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(247, 226, 154));
        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(238, 240, 244));
        slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour::fromRGB(10, 12, 18).withAlpha(0.82f));
        slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        slider.textFromValueFunction = [display](double value) { return formatSliderValue(value, display); };
        slider.valueFromTextFunction = [display](const juce::String& text) { return parseSliderValue(text, display); };
        addAndMakeVisible(slider);

        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            parameterID,
            slider);

        slider.textFromValueFunction = [display](double value) { return formatSliderValue(value, display); };
        slider.valueFromTextFunction = [display](const juce::String& text) { return parseSliderValue(text, display); };
        slider.updateText();
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(2);
        label.setBounds(area.removeFromTop(18));
        slider.setBounds(area);
    }

private:
    juce::Label label;
    juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderControl)
};

class ChoiceControl final : public juce::Component
{
public:
    ChoiceControl(
        juce::AudioProcessorValueTreeState& apvts,
        const juce::String& parameterID,
        const juce::String& labelText,
        const juce::StringArray& choices,
        juce::Colour accentColour)
    {
        label.setText(labelText, juce::dontSendNotification);
        label.setFont(getUiFont(14.0f));
        label.setColour(juce::Label::textColourId, juce::Colour::fromRGB(232, 235, 241));
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);

        for (int index = 0; index < choices.size(); ++index)
            comboBox.addItem(choices[index], index + 1);

        comboBox.setColour(juce::ComboBox::backgroundColourId, juce::Colour::fromRGB(10, 12, 18).withAlpha(0.84f));
        comboBox.setColour(juce::ComboBox::outlineColourId, accentColour.withAlpha(0.72f));
        comboBox.setColour(juce::ComboBox::textColourId, juce::Colour::fromRGB(238, 240, 244));
        comboBox.setColour(juce::ComboBox::arrowColourId, accentColour);
        comboBox.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(comboBox);

        attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts,
            parameterID,
            comboBox);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(2);
        label.setBounds(area.removeFromTop(18));
        area.removeFromTop(12);
        comboBox.setBounds(area.withSizeKeepingCentre(juce::jmin(area.getWidth(), 118), 26));
    }

private:
    juce::Label label;
    juce::ComboBox comboBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChoiceControl)
};
} // namespace

class RMWestVoiceAudioProcessorEditor::ParameterSection final : public juce::Component
{
public:
    ParameterSection(juce::String sectionTitle, juce::Colour sectionAccent, int minimumCellWidth = 88)
        : title(std::move(sectionTitle)),
          accent(sectionAccent),
          minCellWidth(minimumCellWidth)
    {
    }

    void addSlider(
        juce::AudioProcessorValueTreeState& apvts,
        const juce::String& parameterID,
        const juce::String& labelText,
        SliderDisplay display)
    {
        auto control = std::make_unique<SliderControl>(apvts, parameterID, labelText, display, accent);
        addAndMakeVisible(*control);
        controls.push_back(std::move(control));
    }

    void addChoice(
        juce::AudioProcessorValueTreeState& apvts,
        const juce::String& parameterID,
        const juce::String& labelText,
        const juce::StringArray& choices)
    {
        auto control = std::make_unique<ChoiceControl>(apvts, parameterID, labelText, choices, accent);
        addAndMakeVisible(*control);
        controls.push_back(std::move(control));
    }

    void paint(juce::Graphics& g) override
    {
        const auto bounds = getLocalBounds().toFloat().reduced(0.5f);

        g.setColour(juce::Colour::fromRGB(7, 9, 14).withAlpha(0.78f));
        g.fillRoundedRectangle(bounds, 8.0f);

        g.setColour(accent.withAlpha(0.68f));
        g.drawRoundedRectangle(bounds, 8.0f, 1.2f);

        auto titleArea = getLocalBounds().reduced(14, 8).removeFromTop(22);
        g.setFont(getUiFont(17.0f));
        g.setColour(accent);
        g.drawFittedText(title, titleArea, juce::Justification::centredLeft, 1);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(12);
        area.removeFromTop(28);

        if (controls.empty())
            return;

        const auto columns = juce::jlimit(1, static_cast<int>(controls.size()), juce::jmax(1, area.getWidth() / minCellWidth));
        const auto rows = static_cast<int>((controls.size() + columns - 1) / columns);
        const auto cellWidth = area.getWidth() / columns;
        const auto cellHeight = area.getHeight() / rows;

        for (int index = 0; index < static_cast<int>(controls.size()); ++index)
        {
            const auto column = index % columns;
            const auto row = index / columns;
            auto cell = juce::Rectangle<int>(
                area.getX() + (column * cellWidth),
                area.getY() + (row * cellHeight),
                cellWidth,
                cellHeight);

            controls[static_cast<size_t>(index)]->setBounds(cell.reduced(3));
        }
    }

private:
    juce::String title;
    juce::Colour accent;
    int minCellWidth { 88 };
    std::vector<std::unique_ptr<juce::Component>> controls;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterSection)
};

RMWestVoiceAudioProcessorEditor::RMWestVoiceAudioProcessorEditor(RMWestVoiceAudioProcessor& p)
    : AudioProcessorEditor(&p),
      audioProcessor(p)
{
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize);

    const auto performanceAccent = juce::Colour::fromRGB(96, 214, 196);
    const auto toneAccent = juce::Colour::fromRGB(247, 226, 154);
    const auto filterAccent = juce::Colour::fromRGB(235, 92, 164);
    const auto fxAccent = juce::Colour::fromRGB(124, 166, 255);
    const auto outputAccent = juce::Colour::fromRGB(255, 147, 95);

    performanceSection = std::make_unique<ParameterSection>("Performance", performanceAccent, 74);
    performanceSection->addChoice(audioProcessor.apvts, RMWestVoiceParameters::ID::glideMode, "Glide", { "Off", "Always", "Auto-Legato" });
    performanceSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::glideTime, "Glide Time", SliderDisplay::seconds);
    performanceSection->addChoice(audioProcessor.apvts, RMWestVoiceParameters::ID::notePriority, "Priority", { "Last", "Low" });
    performanceSection->addChoice(audioProcessor.apvts, RMWestVoiceParameters::ID::bendRange, "Bend", { "2", "5", "7", "12" });
    performanceSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::vibratoDepth, "Vib Depth", SliderDisplay::cents);
    performanceSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::vibratoRate, "Vib Rate", SliderDisplay::hertz);
    performanceSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::vibratoFade, "Vib Fade", SliderDisplay::seconds);
    performanceSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::vibratoAftertouch, "Aftertouch", SliderDisplay::percent);
    performanceSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::ampAttack, "Attack", SliderDisplay::seconds);
    performanceSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::ampDecay, "Decay", SliderDisplay::seconds);
    performanceSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::ampSustain, "Sustain", SliderDisplay::percent);
    performanceSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::ampRelease, "Release", SliderDisplay::seconds);
    addAndMakeVisible(*performanceSection);

    toneSection = std::make_unique<ParameterSection>("Tone", toneAccent, 86);
    toneSection->addChoice(audioProcessor.apvts, RMWestVoiceParameters::ID::character, "Character", { "Analog", "Worm", "Hybrid" });
    toneSection->addChoice(audioProcessor.apvts, RMWestVoiceParameters::ID::wave, "Wave", { "Saw", "Tri", "Saw+Tri", "Saw+Pulse" });
    toneSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::oscMix, "Osc Mix", SliderDisplay::percent);
    toneSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::detuneCents, "Detune", SliderDisplay::cents);
    addAndMakeVisible(*toneSection);

    filterSection = std::make_unique<ParameterSection>("Filter", filterAccent, 84);
    filterSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::filterCutoff, "Cutoff", SliderDisplay::hertz);
    filterSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::filterResonance, "Resonance", SliderDisplay::percent);
    filterSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::filterDrive, "Drive", SliderDisplay::multiplier);
    filterSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::filterKeyTracking, "Keytrack", SliderDisplay::percent);
    filterSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::filterEnvelopeAmount, "Env Amt", SliderDisplay::octaves);
    addAndMakeVisible(*filterSection);

    fxSection = std::make_unique<ParameterSection>("FX", fxAccent, 78);
    fxSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::width, "Width", SliderDisplay::percent);
    fxSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::delayMix, "Delay Mix", SliderDisplay::percent);
    fxSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::delayTime, "Delay Time", SliderDisplay::seconds);
    fxSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::delayFeedback, "Feedback", SliderDisplay::percent);
    fxSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::reverbMix, "Reverb Mix", SliderDisplay::percent);
    fxSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::reverbSize, "Size", SliderDisplay::percent);
    fxSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::reverbDamping, "Damping", SliderDisplay::percent);
    addAndMakeVisible(*fxSection);

    outputSection = std::make_unique<ParameterSection>("Output", outputAccent, 116);
    outputSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::outputHighpassCutoff, "Highpass", SliderDisplay::hertz);
    outputSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::outputHighpassResonance, "HP Res", SliderDisplay::q);
    outputSection->addSlider(audioProcessor.apvts, RMWestVoiceParameters::ID::outputGain, "Gain", SliderDisplay::percent);
    addAndMakeVisible(*outputSection);

    setSize(960, 640);
    setResizable(false, false);
}

RMWestVoiceAudioProcessorEditor::~RMWestVoiceAudioProcessorEditor() = default;

void RMWestVoiceAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(7, 8, 13));

    if (! backgroundImage.isNull())
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat(), juce::RectanglePlacement::fillDestination);
        g.setColour(juce::Colour::fromRGB(3, 5, 10).withAlpha(0.68f));
        g.fillAll();
    }

    auto header = getLocalBounds().reduced(18, 14).removeFromTop(54);
    auto titleTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::wholecar_ttf, BinaryData::wholecar_ttfSize);
    auto titleFont = juce::Font { juce::FontOptions(titleTypeface).withHeight(46.0f) };

    g.setFont(titleFont);
    g.setColour(juce::Colour::fromRGB(247, 226, 154));
    g.drawFittedText("RM-WEST VOICE", header, juce::Justification::centredLeft, 1);

    g.setFont(getUiFont(14.0f));
    g.setColour(juce::Colour::fromRGB(96, 214, 196));
    g.drawFittedText("MONO LEAD SYNTH", header.removeFromRight(220), juce::Justification::centredRight, 1);
}

void RMWestVoiceAudioProcessorEditor::resized()
{
    constexpr auto margin = 16;
    constexpr auto gap = 10;

    auto area = getLocalBounds().reduced(margin);
    area.removeFromTop(62);

    auto performanceArea = area.removeFromTop(juce::jlimit(156, 186, getHeight() / 4));
    performanceSection->setBounds(performanceArea);

    area.removeFromTop(gap);

    auto outputArea = area.removeFromBottom(128);
    area.removeFromBottom(gap);

    const auto mainWidth = area.getWidth();
    auto toneArea = area.removeFromLeft(static_cast<int>(mainWidth * 0.34f));
    area.removeFromLeft(gap);
    auto filterArea = area.removeFromLeft(static_cast<int>(mainWidth * 0.29f));
    area.removeFromLeft(gap);
    auto fxArea = area;

    toneSection->setBounds(toneArea);
    filterSection->setBounds(filterArea);
    fxSection->setBounds(fxArea);
    outputSection->setBounds(outputArea);
}
