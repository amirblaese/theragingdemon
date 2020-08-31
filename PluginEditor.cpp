/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    TypeSliderValue = std::make_unique
        <juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.treeState, "type", TypeSlider);
    DriveSliderValue = std::make_unique
        <juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.treeState, "drive", DriveSlider);
    LPCSliderValue = std::make_unique
        <juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.treeState, "lpc", LPCSlider);
    HPCSliderValue = std::make_unique
        <juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.treeState, "hpc", HPCSlider);
    InputSliderValue = std::make_unique
        <juce::AudioProcessorValueTreeState::SliderAttachment>
        (audioProcessor.treeState, "input", InputSlider);


    TypeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    TypeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    TypeSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::lightpink);
    TypeSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::lightslategrey);
    TypeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::deepskyblue);
    TypeSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black);
    TypeSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::navy);
    TypeSlider.setRange(0, 3, 1);
    TypeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::gold);
    TypeSlider.setValue(*audioProcessor.treeState.getRawParameterValue("type"));
    TypeSlider.addListener(this);
    addAndMakeVisible(TypeSlider);

    InputSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    InputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    InputSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::steelblue);
    InputSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::lightslategrey);
    InputSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::deepskyblue);
    InputSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black);
    InputSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::navy);
    InputSlider.setRange(0.0f, 50.0f, 0.01f);
    InputSlider.setTextValueSuffix("dB");
    InputSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::gold);
    InputSlider.setValue(*audioProcessor.treeState.getRawParameterValue("input"));
    InputSlider.addListener(this);
    addAndMakeVisible(InputSlider);


    DriveSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    DriveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    DriveSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::indianred);
    DriveSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::deepskyblue);
    DriveSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::lightslategrey);
    DriveSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black);
    DriveSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::navy);
    DriveSlider.setRange(0.0f, 0.99f, 0.01f);
    DriveSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::gold);
    DriveSlider.setValue(*audioProcessor.treeState.getRawParameterValue("drive"));
    DriveSlider.addListener(this);
    addAndMakeVisible(DriveSlider);

    LPCSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    LPCSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    LPCSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::forestgreen);
    LPCSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::deepskyblue);
    LPCSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::lightslategrey);
    LPCSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black);
    LPCSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::navy);
    LPCSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::gold);
    LPCSlider.setTextValueSuffix("Hz");
    LPCSlider.setValue(*audioProcessor.treeState.getRawParameterValue("lpc"));
    LPCSlider.setRange(5, 20000, 1);
    LPCSlider.addListener(this);
    LPCSlider.setSkewFactorFromMidPoint(1000);
    LPCSlider.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible(LPCSlider);

    HPCSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    HPCSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    HPCSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::deepskyblue);
    HPCSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::darkviolet);
    HPCSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::lightslategrey);
    HPCSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black);
    HPCSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::navy);
    HPCSlider.setRange(5, 20000, 1);
    HPCSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::gold);
    HPCSlider.setValue(*audioProcessor.treeState.getRawParameterValue("hpc"));
    HPCSlider.setTextValueSuffix("Hz");
    HPCSlider.addListener(this);
    HPCSlider.setSkewFactorFromMidPoint(1000);
    HPCSlider.setNumDecimalPlacesToDisplay(0);
    addAndMakeVisible(HPCSlider);

    setSize(550, 210);
}

DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor()
{
}

//==============================================================================
void DistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // (Our component is opaque, so we must completely fill the background with a solid colour)

    g.setFont(juce::Font("Arial", 12, juce::Font::bold));

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::deepskyblue);

    //g.fillAll(juce::Colours::lightyellow);
    //g.fillAll(juce::Colour (0xffF2C063));
    g.setColour(juce::Colours::gold);
    //g.setOpacity(0);
    g.setFont(55.0f);
    //g.drawFittedText ("compressor.", getLocalBounds(), juce::Justification::topLeft, 1);
    g.drawText("raging demon.", 35, 0, 500, 100, juce::Justification::topLeft, false);
    g.setFont(juce::Font("Arial", 20, juce::Font::bold));
    g.drawText("mode", 30, (getHeight() / 2) + 6, 100, 100, juce::Justification::centred, false);
    g.drawText("input", 130, (getHeight() / 2) +6 , 100, 100, juce::Justification::centred, false);
    g.drawText("edge", 230, (getHeight() / 2) +6, 100, 100, juce::Justification::centred, false);
    g.drawText("lpf", 330, (getHeight() / 2) +6, 100, 100, juce::Justification::centred, false);
    g.drawText("hpf", 430, (getHeight() / 2) +6, 100, 100, juce::Justification::centred, false);
    g.setColour(juce::Colour(0xff41e2ba));
    //g.drawText("bypass", 420, -22, 100, 100, juce::Justification::centred, false);


}

void DistortionAudioProcessorEditor::resized()
{
    TypeSlider.setBounds(30, getHeight() / 2 - 60, 100, 150);
    InputSlider.setBounds(130, getHeight() / 2 - 60, 100, 150);
    DriveSlider.setBounds(230, getHeight() / 2 - 60, 100, 150);
    LPCSlider.setBounds(330, getHeight() / 2 - 60, 100, 150);
    HPCSlider.setBounds(430, getHeight() / 2 - 60, 100, 150);
}

void DistortionAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &TypeSlider)
    { 
        audioProcessor.Type = TypeSlider.getValue();
    }
    if (slider == &InputSlider)
    {
        audioProcessor.Input = InputSlider.getValue();
    }


    if (slider == &DriveSlider)
    {
        audioProcessor.Drive = DriveSlider.getValue();
    }

    if (slider == &LPCSlider)
    {
        audioProcessor.LPC = LPCSlider.getValue();
    }

    if (slider == &HPCSlider)
    {
        audioProcessor.HPC = HPCSlider.getValue();
    }


}
