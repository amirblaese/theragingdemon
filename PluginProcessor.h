/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class DistortionAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DistortionAudioProcessor();
    ~DistortionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void DistortionAudioProcessor::updateFilter();

    int Type{ 0 };
    float Drive{ 0.25 };
    int LPC{ 500 };
    int HPC{ 10 };
    float Input{ 0.0 };


    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>,
        juce::dsp::StateVariableFilter::Parameters<float>> LPF_L;
    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>,
        juce::dsp::StateVariableFilter::Parameters<float>> LPF_R;

    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>,
        juce::dsp::StateVariableFilter::Parameters<float>> HPF_L;
    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>,
        juce::dsp::StateVariableFilter::Parameters<float>> HPF_R;

    float Pi{ 3.14159265359 };
    float lastSampleRate;


    juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessor)
};
