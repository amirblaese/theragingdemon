/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistortionAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Slider::Listener
{
public:
    DistortionAudioProcessorEditor (DistortionAudioProcessor&);
    ~DistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
  
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    juce::Slider TypeSlider;
    juce::Slider DriveSlider;
    juce::Slider LPCSlider;
    juce::Slider HPCSlider;
    juce::Slider InputSlider;



    DistortionAudioProcessor& audioProcessor;

public:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> TypeSliderValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> DriveSliderValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> LPCSliderValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> HPCSliderValue;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> InputSliderValue;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessorEditor)
};
