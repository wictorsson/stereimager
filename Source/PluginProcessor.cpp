/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
StereoimagerAudioProcessor::StereoimagerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    apvts.addParameterListener("width", this);
    apvts.addParameterListener("gain", this);
}

StereoimagerAudioProcessor::~StereoimagerAudioProcessor()
{
    apvts.removeParameterListener("width", this);
    apvts.removeParameterListener("gain", this);
}



juce::AudioProcessorValueTreeState::ParameterLayout StereoimagerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto width = std::make_unique<juce::AudioParameterFloat> ("width","Width", juce::NormalisableRange<float>(-100,100, 1),0);
    params.push_back(std::move(width));
    
    auto gain = std::make_unique<juce::AudioParameterFloat> ("gain", "Gain", juce::NormalisableRange<float>(-24, 6, 0.1),0);
    params.push_back(std::move(gain));
    return {params.begin(), params.end()};
}

void StereoimagerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "width")
    {
        //Kolla hur det blir utan att ha rört parametern!
        // if newValue > 0 jmappa 1-1.2, annars mappa 0-1.0
        float scaledValue;
        if(newValue > 0)
        {
            scaledValue = juce::jmap(newValue, 0.0f, 100.0f, 1.0f, 1.2f);
        }
        else
        {
            scaledValue = juce::jmap(newValue, -100.0f, 0.0f, 0.0f, 1.0f);
        }
        
        widthValue.setTargetValue(scaledValue);
    }
    
    if (parameterID == "gain")
    {
        // two channels
        gainValue.setTargetValue(juce::Decibels::decibelsToGain(newValue * 0.5));
      
    }
}


//==============================================================================
const juce::String StereoimagerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoimagerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool StereoimagerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool StereoimagerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double StereoimagerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int StereoimagerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int StereoimagerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoimagerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StereoimagerAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereoimagerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void StereoimagerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    widthValue.reset(sampleRate, 0.03);
    gainValue.reset(sampleRate, 0.03);
    
    gainValue = juce::Decibels::decibelsToGain(static_cast<float>(*apvts.getRawParameterValue("gain")));
    
  
    float newValue = static_cast<float>(*apvts.getRawParameterValue("width"));
    if(newValue > 0)
       {
        widthValue = juce::jmap(newValue, 0.0f, 100.0f, 1.0f, 1.2f);
        }
    else
    {
        widthValue = juce::jmap(newValue, -100.0f, 0.0f, 0.0f, 1.0f);
    }
   
    
}

void StereoimagerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoimagerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void StereoimagerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

 
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelDataL = buffer.getWritePointer (0);
        auto* channelDataR = buffer.getWritePointer (1);
        
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
        //Create mid side channels
            
            const auto side = 0.5 * (channelDataL[sample] - channelDataR[sample]);
            const auto mid = 0.5 * (channelDataL[sample] + channelDataR[sample]);

            const auto sideNew = widthValue.getNextValue() * side;
            const auto midNew = (2 - widthValue.getNextValue()) * mid;

            const auto newLeft = midNew + sideNew;
            const auto newRight = midNew - sideNew;
            
            auto gain = gainValue.getNextValue();
            if(widthValue.getNextValue() >= 1.0)
            {
                const auto widthScale = juce::Decibels::decibelsToGain(juce::jmap(widthValue.getNextValue(),1.0f, 2.0f, 0.0f, 6.0f));
                channelDataL[sample] = newLeft * widthScale * gain;
                channelDataR[sample] = newRight * widthScale * gain;
            }
            else
            {
                const auto widthScale = juce::Decibels::decibelsToGain(juce::jmap(widthValue.getNextValue(),1.0f, 0.0f, 0.0f, -6.0f));
                channelDataL[sample] = newLeft * widthScale * gain;
                channelDataR[sample] = newRight * widthScale * gain;
            }
            
        }
        
        
        // ..do something to the data...
    }
}

//==============================================================================
bool StereoimagerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* StereoimagerAudioProcessor::createEditor()
{
    return new StereoimagerAudioProcessorEditor (*this);
    
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void StereoimagerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
   
    //Save
    juce::MemoryOutputStream stream(destData, false);
    apvts.state.writeToStream(stream);
    
}

void StereoimagerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //call the save state
    auto tree = juce::ValueTree::readFromData(data, size_t (sizeInBytes));
    
    if(tree.isValid())
    {
        apvts.state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoimagerAudioProcessor();
}
