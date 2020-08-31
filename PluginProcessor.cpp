/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "cmath"

//==============================================================================
DistortionAudioProcessor::DistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    treeState(*this, nullptr, "PARAMETER",
        { std::make_unique<juce::AudioParameterInt>("type","Type",0,3,1) ,
        std::make_unique<juce::AudioParameterFloat>("drive","Drive",0.0f,0.99f,0.25f),
        std::make_unique<juce::AudioParameterFloat>("input","Input",0.0f,50.0f,0.0f),
        std::make_unique<juce::AudioParameterInt>("lpc","Lpc",5,20000,20000),
        std::make_unique<juce::AudioParameterInt>("hpc","Hpc",5,20000,5)
        })

#endif
{
}

DistortionAudioProcessor::~DistortionAudioProcessor()
{
}
juce::AudioProcessorValueTreeState::ParameterLayout DistortionAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>>params;

    auto TypeParam = std::make_unique<juce::AudioParameterFloat>("type", "Type", 0, 3, 1);
    auto DriveParam = std::make_unique<juce::AudioParameterFloat>("drive", "Drive", 0.0f, 0.99f, 0.25f);
    auto InputParam = std::make_unique<juce::AudioParameterFloat>("input", "Input", 0.0f, 50.0f, 0.0f);
    auto LPCParam = std::make_unique<juce::AudioParameterInt>("lpc", "Lpc", 5, 20000, 20000);
    auto HPCParam = std::make_unique<juce::AudioParameterInt>("hpc", "Hpc", 5, 20000, 5);



    params.push_back(std::move(TypeParam));
    params.push_back(std::move(InputParam));
    params.push_back(std::move(DriveParam));
    params.push_back(std::move(LPCParam));
    params.push_back(std::move(HPCParam));



    return{ params.begin(),params.end() };

}
//==============================================================================
const juce::String DistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //LPF_L.s
    //LPF_R.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, LPC));
    lastSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
    
    LPF_L.reset();
    LPF_R.reset();
    HPF_L.reset();
    HPF_R.reset();

    updateFilter();
    LPF_L.prepare(spec);
    LPF_R.prepare(spec);
    HPF_L.prepare(spec);
    HPF_R.prepare(spec);



}

void DistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void DistortionAudioProcessor::updateFilter()
{
    int cutoffLPF = *treeState.getRawParameterValue("lpc");
    int cutoffHPF = *treeState.getRawParameterValue("hpc");

    LPF_L.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
    LPF_L.state->setCutOffFrequency(lastSampleRate, cutoffLPF);

    LPF_R.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
    LPF_R.state->setCutOffFrequency(lastSampleRate, cutoffLPF);

    HPF_L.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
    HPF_L.state->setCutOffFrequency(lastSampleRate, cutoffHPF);

    HPF_R.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
    HPF_R.state->setCutOffFrequency(lastSampleRate, cutoffHPF);
}



#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


       juce::dsp::AudioBlock<float> block (buffer);
       updateFilter();
       LPF_L.process(juce::dsp::ProcessContextReplacing<float>(block));
       HPF_L.process(juce::dsp::ProcessContextReplacing<float>(block));

       for (int channel = 0; channel < totalNumInputChannels; ++channel)
       {
           auto* channelData = buffer.getWritePointer(channel);

           for (int s = 0; s < buffer.getNumSamples(); ++s)
           {

               float cur_dB = -144.0f;

               if (channelData[s] != 0.0f)
               {
                   cur_dB = 20.0f * log10(fabs(channelData[s])) + Input;
               }

               if (channelData[s] < 0.0f)
               {
                   channelData[s] = -1 * pow(10.0, cur_dB / 20.0f);
               }

               else
               {
                   channelData[s] = pow(10.0, cur_dB / 20.0f);
               }


               if (Type == 0)
               {
                   channelData[s] = tanh(channelData[s] / (1 - Drive));
               }
               if (Type == 1)
               {
                   channelData[s] =  (2 / Pi) * (atan((Pi / (2 * (1.0 - Drive))) * channelData[s]));
               }
               if (Type == 2)
               {
                   channelData[s]=(channelData[s] / (1.0 + (Drive * abs(channelData[s]))));
               }
               if (Type == 3)
               {
                   if (fabs(channelData[s]) > 0.03)
                   {
                       channelData[s]=0.1*channelData[s] / ((1 - Drive) * abs(channelData[s]));
                   }
               }
           }
       }
}

//==============================================================================
bool DistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionAudioProcessor::createEditor()
{
    return new DistortionAudioProcessorEditor (*this);
}

//==============================================================================
void DistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionAudioProcessor();
}
